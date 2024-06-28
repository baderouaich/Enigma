#include <pch.hpp>
#include "AES.hpp"

NS_ENIGMA_BEGIN

AES::AES(const Algorithm::Intent intent) noexcept
	: 
	Algorithm(Algorithm::Type::AES, intent)
{
	// Encrypting & Decrypting
	if (intent & Intent::All)
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

std::string AES::Encrypt(const std::string& password, const std::string& buffer)
{
	// Make sure encryption mode and the seeder are initialized & Validate Arguments
	{
		ENIGMA_ASSERT_OR_THROW(m_aes_encryptor, "AES Encryptor is not initialized properly");
		// AES password length must be at least 6 for security reasons
		ENIGMA_ASSERT_OR_THROW(password.size() >= Constants::Algorithm::MINIMUM_PASSWORD_LENGTH, "AES Minimum Password Length is " + std::to_string(Constants::Algorithm::MINIMUM_PASSWORD_LENGTH));
		//No max password check since we using KDF SHA-256, this allows you to use a password smaller or larger than the cipher's key size: https://crypto.stackexchange.com/questions/68299/length-of-password-requirement-using-openssl-aes-256-cbc
	}

	// Randomly generated IV
	const std::string iv = Algorithm::GenerateRandomIV(CryptoPP::AES::BLOCKSIZE);

	// Prepare key
	CryptoPP::SecByteBlock key(static_cast<std::size_t>(CryptoPP::AES::MAX_KEYLENGTH) + static_cast<std::size_t>(CryptoPP::AES::BLOCKSIZE)); // Encryption key to be generated from user password + IV

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
	std::string cipher{};
	// Encrypt
	[[maybe_unused]] const CryptoPP::StringSource ss(
		buffer,
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

std::string AES::Decrypt(const std::string& password, const std::string& algotype_iv_cipher)
{
	// Make sure decryption mode is initialized
	ENIGMA_ASSERT(m_aes_decryptor, "AES Decryptor is not initialized properly");

	// Extract IV and Cipher from algotype_iv_cipher (we output cipher as AlgoType + IV + Cipher)
	const std::string iv = algotype_iv_cipher.substr(sizeof(Algorithm::Type), CryptoPP::AES::BLOCKSIZE);
	ENIGMA_ASSERT_OR_THROW(!iv.empty(), "Failed to extract IV part from algotype_iv_cipher");
	const std::string cipher = algotype_iv_cipher.substr(sizeof(Algorithm::Type) + CryptoPP::AES::BLOCKSIZE, algotype_iv_cipher.size() - 1);
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
	std::string decrypted{};
	// Decrypt
	[[maybe_unused]] const auto ss = CryptoPP::StringSource(
		cipher,
		true,
		new CryptoPP::AuthenticatedDecryptionFilter(
			*m_aes_decryptor,
			new CryptoPP::StringSink(decrypted)
		)
	); //NOTE: StringSource will auto clean the allocated memory

	return decrypted;
}


NS_ENIGMA_END
