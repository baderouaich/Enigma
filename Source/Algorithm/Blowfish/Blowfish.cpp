#include <pch.hpp>
#include "Blowfish.hpp"

NS_ENIGMA_BEGIN

Blowfish::Blowfish(Algorithm::Intent intent) noexcept
	:
	Algorithm(Algorithm::Type::Blowfish, intent),
	m_blowfish_encryptor(intent == Algorithm::Intent::Encrypt ? std::make_unique<CryptoPP::EAX<CryptoPP::Blowfish>::Encryption>() : nullptr),
	m_blowfish_decryptor(intent == Algorithm::Intent::Decrypt ? std::make_unique<CryptoPP::EAX<CryptoPP::Blowfish>::Decryption>() : nullptr)
{

}

Blowfish::~Blowfish() noexcept
{
}


String Blowfish::Encrypt(const String& password, const String& buffer)
{
	// Make sure encryption mode and the seeder are initialized
	{
		ENIGMA_ASSERT_OR_THROW(m_blowfish_encryptor, "Blowfish Encryptor is not initialized properly");
		ENIGMA_ASSERT_OR_THROW(m_auto_seeded_random_pool, "Blowfish Encryptor seeder is not initialized properly");
	}

	// Validate Arguments
	{
		// Blowfish password length must be at least 6 for security reasons
		ENIGMA_ASSERT_OR_THROW(password.size() >= Constants::Algorithm::MINIMUM_PASSWORD_LENGTH, "Blowfish Minimum Password Length is " + std::to_string(Constants::Algorithm::MINIMUM_PASSWORD_LENGTH));
		//No max password check since we using KDF SHA-256, this allows you to use a password smaller or larger than the cipher's key size: https://crypto.stackexchange.com/questions/68299/length-of-password-requirement-using-openssl-aes-256-cbc
	}

	const String iv = this->GenerateRandomIV(CryptoPP::Blowfish::BLOCKSIZE); // Randomly generated 16 bytes IV
	String output(sizeof(Algorithm::Type), static_cast<const byte>(this->GetType())); // return value will be (AlgoType + IV + Cipher)

	// Prepare key
	CryptoPP::SecByteBlock key(CryptoPP::Blowfish::MAX_KEYLENGTH + CryptoPP::Blowfish::BLOCKSIZE); // Encryption key to be generated from user password + IV

	// Convert key to KDF SHA-256, which allows you to use a password smaller or larger than the cipher's key size
	CryptoPP::HKDF<CryptoPP::SHA256> hkdf;
	hkdf.DeriveKey(
		key, key.size(),
		reinterpret_cast<const byte*>(password.data()), password.size(),
		reinterpret_cast<const byte*>(iv.data()), iv.size(),
		nullptr, 0
	);

	// Set Key and IV to the encryptor
	m_blowfish_encryptor->SetKeyWithIV(key, CryptoPP::Blowfish::MAX_KEYLENGTH, key + CryptoPP::Blowfish::MAX_KEYLENGTH); // key, kl, iv, ivl

	// Encrypt
	String cipher{}; // encrypted buffer
	const CryptoPP::StringSource ss(
		buffer,
		true,
		new CryptoPP::AuthenticatedEncryptionFilter(
			*m_blowfish_encryptor,
			new CryptoPP::StringSink(cipher)
		)
	);
	//NOTE: StringSource will auto clean the allocated memory

	// Output (AlgoType + IV + Cipher) since we need IV and Algorithm used for encryption later for decryption
	output += iv; // Append IV
	output += cipher; // Append Cipher

	return output;
}

String Blowfish::Decrypt(const String& password, const String& iv_cipher)
{
	// Make sure decryption mode is initialized
	ENIGMA_ASSERT(m_blowfish_decryptor, "Blowfish Decryptor is not initialized properly");

	// Split IV and Cipher from buffer (we output encrypted buffers as String(AlgoType + IV + Cipher))
	const String iv = iv_cipher.substr(sizeof(Algorithm::Type), CryptoPP::Blowfish::BLOCKSIZE);
	ENIGMA_ASSERT_OR_THROW(!iv.empty(), "Failed to extract IV from cipher");
	const String cipher = iv_cipher.substr(sizeof(Algorithm::Type) + CryptoPP::Blowfish::BLOCKSIZE, iv_cipher.size() - 1);
	ENIGMA_ASSERT_OR_THROW(!cipher.empty(), "Failed to extract cipher from cipher");

	// Prepare Key
	CryptoPP::SecByteBlock key(CryptoPP::Blowfish::MAX_KEYLENGTH + CryptoPP::Blowfish::BLOCKSIZE);

	// Convert key to KDF SHA-256, which allows you to use a password smaller or larger than the cipher's key size
	CryptoPP::HKDF<CryptoPP::SHA256> hkdf;
	hkdf.DeriveKey(
		key, key.size(),
		reinterpret_cast<const byte*>(password.data()), password.size(),
		reinterpret_cast<const byte*>(iv.data()), iv.size(),
		nullptr, 0);

	// Set Key and IV to the decryptor
	m_blowfish_decryptor->SetKeyWithIV(key, CryptoPP::Blowfish::MAX_KEYLENGTH, key + CryptoPP::Blowfish::MAX_KEYLENGTH); // key, kl, iv, ivl

	// Decrypt
	String decrypted{}; // Recovered buffer
	const CryptoPP::StringSource ss(
		cipher,
		true,
		new CryptoPP::AuthenticatedDecryptionFilter(
			*m_blowfish_decryptor,
			new CryptoPP::StringSink(decrypted)
		)
	);


	//NOTE: StringSource will auto clean the allocated memory

	return decrypted;
}


NS_ENIGMA_END
