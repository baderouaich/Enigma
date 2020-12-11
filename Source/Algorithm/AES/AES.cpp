#include "pch.hpp"
#include "AES.hpp"

NS_ENIGMA_BEGIN

AES::AES(Algorithm::Intent intent) noexcept
	:
	Algorithm(intent),
	m_aes_encryption(intent == Algorithm::Intent::Encrypt ? std::make_unique<CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption>() : nullptr),
	m_aes_decryption(intent == Algorithm::Intent::Decrypt ? std::make_unique<CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption>() : nullptr)
{
}

String AES::Encrypt(const String& password, const String& buffer)
{    
	// Make sure encryption mode and the seeder are initialized
	{
		ENIGMA_ASSERT(m_aes_encryption, "AES Encryption is not initialized properly");
		ENIGMA_ASSERT(m_auto_seeded_random_pool, "AES Encryption seeder is not initialized properly");
	}

	// Validate Arguments
	{
		// AES password length must be at least 9 for security reasons
		if (password.size() < Constants::Algorithm::AES::AES_MINIMUM_PASSWORD_LENGTH)
		{
			ENIGMA_ERROR_ALERT_CONSOLE_AND_UI(String("AES Encryption Failure"), "AES Minimum Password Length is " + std::to_string(Constants::Algorithm::AES::AES_MINIMUM_PASSWORD_LENGTH));
			return String();
		}
		//No max password check since we using KDF SHA-256, his allows you to use a password smaller or larger than the cipher's key size: https://crypto.stackexchange.com/questions/68299/length-of-password-requirement-using-openssl-aes-256-cbc
	}
	
	String iv = this->GenerateRandomIV(); // Random generated 16 bytes IV
	String encrypted; // Final encrypted buffer
	String output; // return value will be (iv + encrypted)
	try
	{
		// Prepare Key
		CryptoPP::SecByteBlock key(CryptoPP::AES::MAX_KEYLENGTH + CryptoPP::AES::BLOCKSIZE); // Encryption key to be generated from user password + IV
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
		m_aes_encryption->SetKeyWithIV(key, CryptoPP::AES::MAX_KEYLENGTH, key + CryptoPP::AES::MAX_KEYLENGTH); // key, kl, iv, ivl

		// Encrypt
		std::unique_ptr<CryptoPP::StringSource> encryptor = std::make_unique<CryptoPP::StringSource>(
			buffer,
			true,
			new CryptoPP::StreamTransformationFilter(
				*m_aes_encryption,
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
			String("AES Encryption Failure ") + CryptoPPUtils::GetCryptoPPErrorString(e.GetErrorType()),
			e.GetWhat()
		);
	}

	return output;
}

String AES::Decrypt(const String& password, const String& buffer)
{
	// Split IV and Cipher from buffer (we output encrypted buffers as String(iv + encrypted))
	const String iv = buffer.substr(0, CryptoPP::AES::BLOCKSIZE);
	const String encrypted = buffer.substr(CryptoPP::AES::BLOCKSIZE, buffer.size() - 1);

	// Final decrypted buffer
	String decrypted;
	try
	{
		// Prepare Key
		CryptoPP::SecByteBlock key(CryptoPP::AES::MAX_KEYLENGTH + CryptoPP::AES::BLOCKSIZE);
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
		m_aes_decryption->SetKeyWithIV(key, CryptoPP::AES::MAX_KEYLENGTH, key + CryptoPP::AES::MAX_KEYLENGTH); // key, kl, iv, ivl


		// Decrypt
		std::unique_ptr<CryptoPP::StringSource> decryptor = std::make_unique<CryptoPP::StringSource>(
			encrypted,
			true,
			new CryptoPP::StreamTransformationFilter(
				*m_aes_decryption,
				new CryptoPP::StringSink(decrypted)
			)
		);

		//NOTE: StringSource will auto clean the allocated memory
	}
	catch (const CryptoPP::Exception& e)
	{
		ENIGMA_ERROR_ALERT_CONSOLE_AND_UI(
			String("AES Decryption Failure ") + CryptoPPUtils::GetCryptoPPErrorString(e.GetErrorType()),
			e.GetWhat()
		);
	}

	return decrypted;
}

AES::~AES() noexcept
{
}

NS_ENIGMA_END