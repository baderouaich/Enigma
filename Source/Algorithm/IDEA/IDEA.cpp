#include "pch.hpp"
#include "IDEA.hpp"

NS_ENIGMA_BEGIN

IDEA::IDEA(Algorithm::Intent intent) noexcept
	:
	Algorithm(Algorithm::Type::IDEA, intent),
	m_idea_encryption(intent == Algorithm::Intent::Encrypt ? std::make_unique<CryptoPP::CBC_Mode<CryptoPP::IDEA>::Encryption>() : nullptr),
	m_idea_decryption(intent == Algorithm::Intent::Decrypt ? std::make_unique<CryptoPP::CBC_Mode<CryptoPP::IDEA>::Decryption>() : nullptr)
{
}


String IDEA::Encrypt(const String& password, const String& buffer)
{
	// Make sure encryption mode and the seeder are initialized
	{
		ENIGMA_ASSERT(m_idea_encryption, "IDEA Encryption is not initialized properly");
		ENIGMA_ASSERT(m_auto_seeded_random_pool, "IDEA Encryption seeder is not initialized properly");
	}

	// Validate Arguments
	{
		// Password length must be at least 9 for security reasons
		if (password.size() < Constants::Algorithm::MINIMUM_PASSWORD_LENGTH)
		{
			ENIGMA_ERROR_ALERT_CONSOLE_AND_UI(String("IDEA Encryption Failure"), "IDEA Minimum Password Length is " + std::to_string(Constants::Algorithm::MINIMUM_PASSWORD_LENGTH));
			return String();
		}
		//No max password check since we using KDF SHA-256, his allows you to use a password smaller or larger than the cipher's key size: https://crypto.stackexchange.com/questions/68299/length-of-password-requirement-using-openssl-aes-256-cbc
	}

	String iv = this->GenerateRandomIV(CryptoPP::IDEA::BLOCKSIZE); // Randomly generated 8 bytes IV
	String encrypted; // Final encrypted buffer
	String output; // return value will be (iv + encrypted)
	try
	{
		// Prepare key
		CryptoPP::SecByteBlock key(CryptoPP::IDEA::MAX_KEYLENGTH + CryptoPP::IDEA::BLOCKSIZE); // Encryption key to be generated from user password + IV

		// Convert key to KDF SHA-256, which allows you to use a password smaller or larger than the cipher's key size
		CryptoPP::HKDF<CryptoPP::SHA256> hkdf;
		hkdf.DeriveKey(
			key, key.size(),
			reinterpret_cast<const byte*>(password.data()), password.size(),
			reinterpret_cast<const byte*>(iv.data()), iv.size(),
			nullptr, 0
		);

		// Set Key and IV to the encryptor
		m_idea_encryption->SetKeyWithIV(key, CryptoPP::IDEA::MAX_KEYLENGTH, key + CryptoPP::IDEA::MAX_KEYLENGTH); // key, kl, iv, ivl

		// Encrypt
		std::unique_ptr<CryptoPP::StringSource> encryptor = std::make_unique<CryptoPP::StringSource>(
			buffer,
			true,
			new CryptoPP::StreamTransformationFilter(
				*m_idea_encryption,
				new CryptoPP::StringSink(encrypted)
			)
			);
		//NOTE: StringSource will auto clean the allocated memory

		// Output iv plus encrypted buffer since we need iv later for decryption
		output = std::move(iv + encrypted);
	}
	catch (const CryptoPP::Exception& e)
	{
		ENIGMA_ERROR_ALERT_CONSOLE_AND_UI(
			String("IDEA Encryption Failure "),
			CryptoPPUtils::GetCryptoPPErrorString(e.GetErrorType()) + '\n' + e.GetWhat()
		);
	}

	return output;
}

String IDEA::Decrypt(const String& password, const String& iv_cipher)
{
	// Make sure decryption mode is initialized
	ENIGMA_ASSERT(m_idea_decryption, "IDEA Decryption is not initialized properly");


	// Split IV and Cipher from buffer (we output encrypted buffers as String(iv + encrypted))
	const String iv = iv_cipher.substr(0, CryptoPP::IDEA::BLOCKSIZE);
	const String cipher = iv_cipher.substr(CryptoPP::IDEA::BLOCKSIZE, iv_cipher.size() - 1);

	// Recovered buffer
	String decrypted;
	try
	{
		// Prepare Key
		CryptoPP::SecByteBlock key(CryptoPP::IDEA::MAX_KEYLENGTH + CryptoPP::IDEA::BLOCKSIZE);

		// Convert key to KDF SHA-256, which allows you to use a password smaller or larger than the cipher's key size
		CryptoPP::HKDF<CryptoPP::SHA256> hkdf;
		hkdf.DeriveKey(
			key, key.size(),
			reinterpret_cast<const byte*>(password.data()), password.size(),
			reinterpret_cast<const byte*>(iv.data()), iv.size(),
			nullptr, 0);

		// Set Key and IV to the decryptor
		m_idea_decryption->SetKeyWithIV(key, CryptoPP::IDEA::MAX_KEYLENGTH, key + CryptoPP::IDEA::MAX_KEYLENGTH); // key, kl, iv, ivl

		// Decrypt
		std::unique_ptr<CryptoPP::StringSource> decryptor = std::make_unique<CryptoPP::StringSource>(
			cipher,
			true,
			new CryptoPP::StreamTransformationFilter(
				*m_idea_decryption,
				new CryptoPP::StringSink(decrypted)
			)
			);

		//NOTE: StringSource will auto clean the allocated memory
	}
	catch (const CryptoPP::Exception& e)
	{
		ENIGMA_ERROR_ALERT_CONSOLE_AND_UI(
			String("IDEA Decryption Failure "),
			CryptoPPUtils::GetCryptoPPErrorString(e.GetErrorType()) + '\n' + e.GetWhat()
		);
	}

	return decrypted;
}

NS_ENIGMA_END