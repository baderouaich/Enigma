#include "pch.hpp"
#include "Twofish.hpp"

NS_ENIGMA_BEGIN

Twofish::Twofish(Algorithm::Intent intent) noexcept
	:
	Algorithm(Algorithm::Type::Twofish, intent),
	m_twofish_encryption(intent == Algorithm::Intent::Encrypt ? std::make_unique<CryptoPP::GCM<CryptoPP::Twofish>::Encryption>() : nullptr),
	m_twofish_decryption(intent == Algorithm::Intent::Decrypt ? std::make_unique<CryptoPP::GCM<CryptoPP::Twofish>::Decryption>() : nullptr)
{
}
Twofish::~Twofish() noexcept
{
}

String Twofish::Encrypt(const String& password, const String& buffer)
{
	// Make sure encryption mode and the seeder are initialized
	{
		ENIGMA_ASSERT_OR_THROW(m_twofish_encryption, "Twofish Encryption is not initialized properly");
		ENIGMA_ASSERT_OR_THROW(m_auto_seeded_random_pool, "Twofish Encryption seeder is not initialized properly");
	}

	// Validate Arguments
	{
		// Twofish password length must be at least 9 for security reasons
		ENIGMA_ASSERT_OR_THROW(password.size() >= Constants::Algorithm::MINIMUM_PASSWORD_LENGTH, "Twofish Minimum Password Length is " + std::to_string(Constants::Algorithm::MINIMUM_PASSWORD_LENGTH));
		//No max password check since we using KDF SHA-256, his allows you to use a password smaller or larger than the cipher's key size: https://crypto.stackexchange.com/questions/68299/length-of-password-requirement-using-openssl-aes-256-cbc
	}

	String iv = this->GenerateRandomIV(CryptoPP::Twofish::BLOCKSIZE); // Randomly generated 16 bytes IV
	String cipher{}; // Final encrypted buffer
	String output(sizeof(Algorithm::Type), static_cast<const byte>(this->GetType())); // return value will be (AlgoType + IV + Cipher)
	
	// Prepare key
	CryptoPP::SecByteBlock key(CryptoPP::Twofish::MAX_KEYLENGTH + CryptoPP::Twofish::BLOCKSIZE); // Encryption key to be generated from user password + IV

	// Convert key to KDF SHA-256, which allows you to use a password smaller or larger than the cipher's key size
	CryptoPP::HKDF<CryptoPP::SHA256> hkdf;
	hkdf.DeriveKey(
		key, key.size(),
		reinterpret_cast<const byte*>(password.data()), password.size(),
		reinterpret_cast<const byte*>(iv.data()), iv.size(),
		nullptr, 0
	);

	// Set Key and IV to the encryptor
	m_twofish_encryption->SetKeyWithIV(key, CryptoPP::Twofish::MAX_KEYLENGTH, key + CryptoPP::Twofish::MAX_KEYLENGTH); // key, kl, iv, ivl

	// Encrypt
	std::unique_ptr<CryptoPP::StringSource> encryptor = std::make_unique<CryptoPP::StringSource>(
		buffer,
		true,
		new CryptoPP::AuthenticatedEncryptionFilter( // note: for GCM mode, use AuthenticatedEncryptionFilter instead of StreamTransformationFilter
			*m_twofish_encryption,
			new CryptoPP::StringSink(cipher)
		)
		);
	//NOTE: StringSource will auto clean the allocated memory

	// Output (AlgoType + IV + Cipher) since we need IV and Algorithm used for encryption later for decryption
	output.append(std::move(iv + cipher));

	return output;
}

String Twofish::Decrypt(const String& password, const String& iv_cipher)
{
	// Make sure decryption mode is initialized
	ENIGMA_ASSERT_OR_THROW(m_twofish_decryption, "Twofish Decryption is not initialized properly");

	// Split IV and Cipher from buffer (we output encrypted buffers as String(AlgoType + IV + Cipher))
	const String iv = iv_cipher.substr(sizeof(Algorithm::Type), CryptoPP::Twofish::BLOCKSIZE);
	ENIGMA_ASSERT_OR_THROW(!iv.empty(), "Failed to extract IV from cipher");
	const String cipher = iv_cipher.substr(sizeof(Algorithm::Type) + CryptoPP::Twofish::BLOCKSIZE, iv_cipher.size() - 1);
	ENIGMA_ASSERT_OR_THROW(!cipher.empty(), "Failed to extract cipher from cipher");

	// Recovered buffer
	String decrypted{};

	// Prepare Key
	CryptoPP::SecByteBlock key(CryptoPP::Twofish::MAX_KEYLENGTH + CryptoPP::Twofish::BLOCKSIZE);

	// Convert key to KDF SHA-256, which allows you to use a password smaller or larger than the cipher's key size
	CryptoPP::HKDF<CryptoPP::SHA256> hkdf;
	hkdf.DeriveKey(
		key, key.size(),
		reinterpret_cast<const byte*>(password.data()), password.size(),
		reinterpret_cast<const byte*>(iv.data()), iv.size(),
		nullptr, 0);

	// Set Key and IV to the decryptor
	m_twofish_decryption->SetKeyWithIV(key, CryptoPP::Twofish::MAX_KEYLENGTH, key + CryptoPP::Twofish::MAX_KEYLENGTH); // key, kl, iv, ivl

	// Decrypt
	std::unique_ptr<CryptoPP::StringSource> decryptor = std::make_unique<CryptoPP::StringSource>(
		cipher,
		true,
		new CryptoPP::AuthenticatedDecryptionFilter(
			*m_twofish_decryption,
			new CryptoPP::StringSink(decrypted)
		)
		);

	//NOTE: StringSource will auto clean the allocated memory
	
	return decrypted;
}

NS_ENIGMA_END
