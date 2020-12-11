#include "pch.hpp"
#include "ChaCha.hpp"


NS_ENIGMA_BEGIN

ChaCha::ChaCha(Algorithm::Intent intent) noexcept
	:
	Algorithm(intent),
	m_chacha_encryption(intent == Algorithm::Intent::Encrypt ? std::make_unique<CryptoPP::ChaCha::Encryption>() : nullptr),
	m_chacha_decryption(intent == Algorithm::Intent::Decrypt ? std::make_unique<CryptoPP::ChaCha::Decryption>() : nullptr)
{
}


String ChaCha::Encrypt(const String& password, const String& buffer)
{
	// Make sure encryption mode and the seeder are initialized
	{
		ENIGMA_ASSERT(m_chacha_encryption, "ChaCha Encryption is not initialized properly");
		ENIGMA_ASSERT(m_auto_seeded_random_pool, "ChaCha Encryption seeder is not initialized properly");
	}

	// Validate Arguments
	{
		// ChaCha password length must be at least 9 for security reasons
		if (password.size() < Constants::Algorithm::ChaCha::CHACHA_MINIMUM_PASSWORD_LENGTH)
		{
			ENIGMA_ERROR_ALERT_CONSOLE_AND_UI(String("ChaCha Encryption Failure"), "ChaCha Minimum Password Length is " + std::to_string(Constants::Algorithm::ChaCha::CHACHA_MINIMUM_PASSWORD_LENGTH));
			return String();
		}
		//No max password check since we using KDF SHA-256, his allows you to use a password smaller or larger than the cipher's key size: https://crypto.stackexchange.com/questions/68299/length-of-password-requirement-using-openssl-aes-256-cbc
	}

	String iv = this->GenerateRandomIV(CryptoPP::ChaCha::IV_LENGTH); // Randomly generated 8 bytes ChaCha IV
	String encrypted; // Final encrypted buffer
	String output; // return value will be (iv + encrypted)
	try
	{
		// Prepare key
		CryptoPP::SecByteBlock key(CryptoPP::ChaCha::MAX_KEYLENGTH + CryptoPP::ChaCha::IV_LENGTH);
		
		// Convert key to KDF SHA-256, which allows you to use a password smaller or larger than the cipher's key size
		CryptoPP::HKDF<CryptoPP::SHA256> hkdf;
		hkdf.DeriveKey(
			key,
			key.size(),
			reinterpret_cast<const byte*>(password.data()),
			password.size(),
			reinterpret_cast<const byte*>(iv.data()),
			iv.size(),
			nullptr,
			0);

		// Set Key and IV to the encryptor
		m_chacha_encryption->SetKeyWithIV(key, CryptoPP::ChaCha::MAX_KEYLENGTH, key + CryptoPP::ChaCha::MAX_KEYLENGTH); // key, kl, iv, ivl

		// Encrypt
		std::unique_ptr<CryptoPP::StringSource> encryptor = std::make_unique<CryptoPP::StringSource>(
			buffer,
			true,
			new CryptoPP::StreamTransformationFilter(
				*m_chacha_encryption,
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
			String("ChaCha Encryption Failure ") + CryptoPPUtils::GetCryptoPPErrorString(e.GetErrorType()),
			e.GetWhat()
		);
	}

	return output;
}

String ChaCha::Decrypt(const String& password, const String& buffer)
{
	// Split IV and Cipher from buffer (we output encrypted buffers as String(iv + encrypted))
	const String iv = buffer.substr(0, CryptoPP::ChaCha::IV_LENGTH);
	const String encrypted = buffer.substr(CryptoPP::ChaCha::IV_LENGTH, buffer.size() - 1);

	// Final decrypted buffer
	String decrypted;
	try
	{
		// Prepare Key
		CryptoPP::SecByteBlock key(CryptoPP::ChaCha::MAX_KEYLENGTH + CryptoPP::ChaCha::IV_LENGTH);
		// Convert key to KDF SHA-256, which allows you to use a password smaller or larger than the cipher's key size
		CryptoPP::HKDF<CryptoPP::SHA256> hkdf;
		hkdf.DeriveKey(
			key,
			key.size(),
			reinterpret_cast<const byte*>(password.data()),
			password.size(),
			reinterpret_cast<const byte*>(iv.data()),
			iv.size(),
			nullptr,
			0);

		// Set Key and IV to the decryptor
		m_chacha_decryption->SetKeyWithIV(key, CryptoPP::ChaCha::MAX_KEYLENGTH, key + CryptoPP::ChaCha::MAX_KEYLENGTH); // key, kl, iv, ivl

		// Decrypt
		std::unique_ptr<CryptoPP::StringSource> decryptor = std::make_unique<CryptoPP::StringSource>(
			encrypted,
			true,
			new CryptoPP::StreamTransformationFilter(
				*m_chacha_decryption,
				new CryptoPP::StringSink(decrypted)
			)
			);

		//NOTE: StringSource will auto clean the allocated memory
	}
	catch (const CryptoPP::Exception& e)
	{
		ENIGMA_ERROR_ALERT_CONSOLE_AND_UI(
			String("ChaCha Decryption Failure ") + CryptoPPUtils::GetCryptoPPErrorString(e.GetErrorType()),
			e.GetWhat()
		);
	}

	return decrypted;
}


ChaCha::~ChaCha() noexcept
{
}

NS_ENIGMA_END

