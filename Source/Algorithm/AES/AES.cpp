#include "pch.hpp"
#include "AES.hpp"
#include <Utility/GZip.hpp>

NS_ENIGMA_BEGIN

AES::AES(const Algorithm::Intent intent) noexcept
	: 
	Algorithm(Algorithm::Type::AES, intent)
{
	// Encrypting & Decrypting
	if (intent & Intent::Encrypt && intent & Intent::Decrypt)
	{
		m_aes_encryptor = std::make_unique<CryptoPP::GCM<CryptoPP::AES>::Encryption>();
		m_aes_decryptor = std::make_unique<CryptoPP::GCM<CryptoPP::AES>::Decryption>();
	}
	// Encrypting only
	else if (intent & Intent::Encrypt)
	{
		m_aes_encryptor = std::make_unique<CryptoPP::GCM<CryptoPP::AES>::Encryption>();
	}
	// Decrypting only
	else if (intent & Intent::Decrypt)
	{
		m_aes_decryptor = std::make_unique<CryptoPP::GCM<CryptoPP::AES>::Decryption>();
	}
}

AES::~AES() noexcept
{
}

String AES::EncryptText(const String& password, const String& text)
{
	// Make sure encryption mode and the seeder are initialized & Validate Arguments
	{
		ENIGMA_ASSERT_OR_THROW(m_aes_encryptor, GetTypeString() + " Encryptor is not initialized properly");
		// AES password length must be at least 6 for security reasons
		ENIGMA_ASSERT_OR_THROW(password.size() >= Constants::Algorithm::MINIMUM_PASSWORD_LENGTH, "AES Minimum Password Length is " + std::to_string(Constants::Algorithm::MINIMUM_PASSWORD_LENGTH));
		//No max password check since we using KDF SHA-256, this allows you to use a password smaller or larger than the cipher's key size: https://crypto.stackexchange.com/questions/68299/length-of-password-requirement-using-openssl-aes-256-cbc
	}

	// Compress text before encrypting
	const String compressed_text = GZip::Compress(text, GZip::DeflateLevel::MAX);

	// Randomly generated IV
	const String iv = Algorithm::GenerateRandomIV(CryptoPP::AES::BLOCKSIZE);

	// Prepare key
	CryptoPP::SecByteBlock key(CryptoPP::AES::MAX_KEYLENGTH + CryptoPP::AES::BLOCKSIZE); // Encryption key to be generated from user password + IV

	// Convert key to KDF SHA-256, which allows you to use a password smaller or larger than the cipher's key size
	CryptoPP::HKDF<CryptoPP::SHA256> hkdf{};
	hkdf.DeriveKey(
		key, key.size(),
		reinterpret_cast<const byte*>(password.data()), password.size(),
		reinterpret_cast<const byte*>(iv.data()), iv.size(),
		nullptr, 0
	);

	// Set Key and IV to the encryptor
	m_aes_encryptor->SetKeyWithIV(key, CryptoPP::AES::MAX_KEYLENGTH, key + CryptoPP::AES::MAX_KEYLENGTH); // key, kl, iv, ivl

	// Output encrypted buffer
	String cipher{};
	// Encrypt
	[[maybe_unused]] const CryptoPP::StringSource ss(
		compressed_text,
		true,
		new CryptoPP::AuthenticatedEncryptionFilter( // note: for GCM mode, use AuthenticatedEncryptionFilter instead of StreamTransformationFilter
			*m_aes_encryptor,
			new CryptoPP::StringSink(cipher)
		)
	); //NOTE: StringSource will auto clean the allocated memory
	

	// Output will be (Algorithm Type + IV + Cipher) since we need IV and Algorithm used for encryption later for decryption
	std::ostringstream output{};
	output
		<< static_cast<char>(this->GetType()) // Append Algorithm Type (enum id)
		<< iv // Append IV
		<< cipher; // Append Cipher
	return output.str();
}

String AES::DecryptText(const String& password, const String& algotype_iv_cipher_text)
{
	// Make sure decryption mode is initialized
	ENIGMA_ASSERT(m_aes_decryptor, "AES Decryptor is not initialized properly");

	// Extract IV and Cipher from algotype_iv_cipher (we output cipher as AlgoType + IV + Cipher)
	const String iv = algotype_iv_cipher_text.substr(sizeof(Algorithm::Type), CryptoPP::AES::BLOCKSIZE);
	ENIGMA_ASSERT_OR_THROW(!iv.empty(), "Failed to extract IV part from algotype_iv_cipher");
	const String cipher = algotype_iv_cipher_text.substr(sizeof(Algorithm::Type) + CryptoPP::AES::BLOCKSIZE, algotype_iv_cipher.size() - 1);
	ENIGMA_ASSERT_OR_THROW(!cipher.empty(), "Failed to extract cipher part from algotype_iv_cipher");

	// Prepare Key
	CryptoPP::SecByteBlock key(CryptoPP::AES::MAX_KEYLENGTH + CryptoPP::AES::BLOCKSIZE); // Encryption key to be generated from user password + IV

	// Convert key to KDF SHA-256, which allows you to use a password smaller or larger than the cipher's key size
	CryptoPP::HKDF<CryptoPP::SHA256> hkdf;
	hkdf.DeriveKey(
		key, key.size(),
		reinterpret_cast<const byte*>(password.data()), password.size(),
		reinterpret_cast<const byte*>(iv.data()), iv.size(),
		nullptr, 0);

	// Set Key and IV to the decryptor
	m_aes_decryptor->SetKeyWithIV(key, CryptoPP::AES::MAX_KEYLENGTH, key + CryptoPP::AES::MAX_KEYLENGTH); // key, kl, iv, ivl

	// Recovered cipher
	String decrypted{};
	// Decrypt
	[[maybe_unused]] const auto ss = CryptoPP::StringSource(
		cipher,
		true,
		new CryptoPP::AuthenticatedDecryptionFilter(
			*m_aes_decryptor,
			new CryptoPP::StringSink(decrypted)
		)
	); //NOTE: StringSource will auto clean the allocated memory

	// Decompress decrypted text
	decrypted = GZip::Decompress(decrypted);

	return decrypted;
}

void AES::EncryptFile(const String& password, const fs::path& in_filename, const fs::path& out_filename)
{
	// Validate arguments
	{ 
		// Make sure encryption mode and the seeder are initialized & Validate Arguments
		ENIGMA_ASSERT_OR_THROW(m_aes_encryptor, GetTypeString() + " Encryptor is not initialized properly");
		// AES password length must be at least 6 for security reasons
		ENIGMA_ASSERT_OR_THROW(password.size() >= Constants::Algorithm::MINIMUM_PASSWORD_LENGTH, "AES Minimum Password Length is " + std::to_string(Constants::Algorithm::MINIMUM_PASSWORD_LENGTH));
		// Check file existence
		ENIGMA_ASSERT_OR_THROW(fs::exists(in_filename), fmt::format("File {} doesn't exist", in_filename.string()));
		// Check file regularity
		ENIGMA_ASSERT_OR_THROW(fs::is_regular_file(in_filename), fmt::format("File {} is not a regular file", in_filename.string()));
		// Check file emptiness
		ENIGMA_ASSERT_OR_THROW(!fs::is_empty(in_filename), fmt::format("File {} is empty", in_filename.string()));
	}

	// Randomly generated IV
	const String iv = Algorithm::GenerateRandomIV(CryptoPP::AES::BLOCKSIZE);

	// Prepare key
	CryptoPP::SecByteBlock key(CryptoPP::AES::MAX_KEYLENGTH + CryptoPP::AES::BLOCKSIZE); // Encryption key to be generated from user password + IV

	// Convert key to KDF SHA-256, which allows you to use a password smaller or larger than the cipher's key size
	CryptoPP::HKDF<CryptoPP::SHA256> hkdf{};
	hkdf.DeriveKey(
		key, key.size(),
		reinterpret_cast<const byte*>(password.data()), password.size(),
		reinterpret_cast<const byte*>(iv.data()), iv.size(),
		nullptr, 0
	);

	// Set Key and IV to the encryptor
	m_aes_encryptor->SetKeyWithIV(key, CryptoPP::AES::MAX_KEYLENGTH, key + CryptoPP::AES::MAX_KEYLENGTH); // key, kl, iv, ivl

	// Encrypt file chunk by chunk (to support large files so not to be loaded all into memory)
	std::ofstream ofs{ out_filename, std::ios::binary };
	ENIGMA_ASSERT_OR_THROW(ofs, fmt::format("Failed to create output file {}", out_filename.string()));

	// Write first (Algorithm Type + IV + Cipher) since we need IV and Algorithm used for encryption later for decryption
	ofs
		<< static_cast<char>(this->GetType()) // Append Algorithm Type (enum id)
		<< iv; // Append IV;
	
	// And now, append Cipher
	FileUtils::ReadChunks(in_filename, Algorithm::FILE_IO_MAX_CHUNK_SIZE, [this, &ofs](std::vector<byte>&& chunk) -> bool
		{
			std::vector<byte> cipher{};

			// Compress chunk before encrypting
			chunk = GZip::Compress(chunk, GZip::DeflateLevel::MAX);

			// Encrypt chunk
			[[maybe_unused]] const CryptoPP::VectorSource ss(
				chunk,
				true,
				new CryptoPP::AuthenticatedEncryptionFilter( // note: for GCM mode, use AuthenticatedEncryptionFilter instead of StreamTransformationFilter
					*m_aes_encryptor,
					new CryptoPP::VectorSource(cipher, true)
				)
			);

			// Write cipher chunk
			ofs.write(reinterpret_cast<const char*>(cipher.data()), cipher.size()); 

			return false; // keep going don't stop...
		});

	// Close file
	ofs.close();
}

void AES::DecryptFile(const String& password, const fs::path& in_filename, const fs::path& out_filename)
{
	// Validate arguments
	{
		// Make sure encryption mode and the seeder are initialized & Validate Arguments
		ENIGMA_ASSERT_OR_THROW(m_aes_encryptor, GetTypeString() + " Decryptor is not initialized properly");
		// Check file existence
		ENIGMA_ASSERT_OR_THROW(fs::exists(in_filename), fmt::format("File {} doesn't exist", in_filename.string()));
		// Check file regularity
		ENIGMA_ASSERT_OR_THROW(fs::is_regular_file(in_filename), fmt::format("File {} is not a regular file", in_filename.string()));
		// Check file emptiness
		ENIGMA_ASSERT_OR_THROW(!fs::is_empty(in_filename), fmt::format("File {} is empty", in_filename.string()));
	}

	// Extract IV from in_filename
	String iv = [&in_filename]() -> String
	{
		String iv;
		std::ifstream ifs{ in_filename, std::ios::binary };
		if (ifs)
		{
			// skip algotype
			ifs.seekg(sizeof(Algorithm::Type), std::ios::beg);
			// read iv
			iv.resize(CryptoPP::AES::BLOCKSIZE, '\000');
			ifs.read(iv.data(), CryptoPP::AES::BLOCKSIZE);
			// close file
			ifs.close();
		}
		return iv;
	}();
	ENIGMA_ASSERT_OR_THROW(!iv.empty(), fmt::format("Failed to extract IV part from input file {}", in_filename.string()));

	// Prepare Key
	CryptoPP::SecByteBlock key(CryptoPP::AES::MAX_KEYLENGTH + CryptoPP::AES::BLOCKSIZE); // Encryption key to be generated from user password + IV

	// Convert key to KDF SHA-256, which allows you to use a password smaller or larger than the cipher's key size
	CryptoPP::HKDF<CryptoPP::SHA256> hkdf;
	hkdf.DeriveKey(
		key, key.size(),
		reinterpret_cast<const byte*>(password.data()), password.size(),
		reinterpret_cast<const byte*>(iv.data()), iv.size(),
		nullptr, 0);

	// Set Key and IV to the decryptor
	m_aes_decryptor->SetKeyWithIV(key, CryptoPP::AES::MAX_KEYLENGTH, key + CryptoPP::AES::MAX_KEYLENGTH); // key, kl, iv, ivl

	// Decrypt file chunk by chunk (to support large files so not to be loaded all into memory)
	std::ofstream ofs{ out_filename, std::ios::binary };
	ENIGMA_ASSERT_OR_THROW(ofs, fmt::format("Failed to create output file {}", out_filename.string()));

	// And now, append recovered buffer
	FileUtils::ReadChunks(in_filename, Algorithm::FILE_IO_MAX_CHUNK_SIZE, [this, &ofs](std::vector<byte>&& cipher_chunk) -> bool
		{
			std::vector<byte> recovered_chunk{};

			// Decrypt chunk
			[[maybe_unused]] const CryptoPP::VectorSource ss(
				cipher_chunk,
				true,
				new CryptoPP::AuthenticatedEncryptionFilter( // note: for GCM mode, use AuthenticatedEncryptionFilter instead of StreamTransformationFilter
					*m_aes_encryptor,
					new CryptoPP::VectorSource(recovered_chunk, true)
				)
			);

			// Decompress chunk after decrypting, then write to file
			recovered_chunk = GZip::Decompress(recovered_chunk);

			// Write recovered chunk
			ofs.write(reinterpret_cast<const char*>(recovered_chunk.data()), recovered_chunk.size());

			return false; // keep going don't stop...
		});

	// Close file
	ofs.close();
}

NS_ENIGMA_END

// TODO
//- Perform all compression & decompression in Algorithm derives, should spare you alot of writing downs GZip::Compress..
//- Apply changes to other classes
//-
//-
//-
