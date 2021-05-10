#include "pch.hpp"
#include "AES.hpp"

NS_ENIGMA_BEGIN

AES::AES(Algorithm::Intent intent) noexcept
	:
	Algorithm(Algorithm::Type::AES, intent),
	m_aes_encryptor(intent == Algorithm::Intent::Encrypt ? std::make_unique<CryptoPP::GCM<CryptoPP::AES>::Encryption>() : nullptr),
	m_aes_decryptor(intent == Algorithm::Intent::Decrypt ? std::make_unique<CryptoPP::GCM<CryptoPP::AES>::Decryption>() : nullptr)
{
	
}

AES::~AES() noexcept
{
}


String AES::Encrypt(const String& password, const String& buffer)
{    
	// Make sure encryption mode and the seeder are initialized
	{
		ENIGMA_ASSERT_OR_THROW(m_aes_encryptor, "AES Encryptor is not initialized properly");
		ENIGMA_ASSERT_OR_THROW(m_auto_seeded_random_pool, "AES Encryptor seeder is not initialized properly");
	}

	// Validate Arguments
	{
		// AES password length must be at least 6 for security reasons
		ENIGMA_ASSERT_OR_THROW(password.size() >= Constants::Algorithm::MINIMUM_PASSWORD_LENGTH, "AES Minimum Password Length is " + std::to_string(Constants::Algorithm::MINIMUM_PASSWORD_LENGTH));
		//No max password check since we using KDF SHA-256, this allows you to use a password smaller or larger than the cipher's key size: https://crypto.stackexchange.com/questions/68299/length-of-password-requirement-using-openssl-aes-256-cbc
	}
	
	const String iv = this->GenerateRandomIV(CryptoPP::AES::BLOCKSIZE); // Randomly generated 16 bytes IV
	String cipher{}; // encrypted buffer
	String output(sizeof(Algorithm::Type), static_cast<const byte>(this->GetType())); // return value will be (AlgoType + IV + Cipher)
	
	// Prepare key
	CryptoPP::SecByteBlock key(CryptoPP::AES::MAX_KEYLENGTH + CryptoPP::AES::BLOCKSIZE); // Encryption key to be generated from user password + IV
		
	// Convert key to KDF SHA-256, which allows you to use a password smaller or larger than the cipher's key size
	CryptoPP::HKDF<CryptoPP::SHA256> hkdf;
	hkdf.DeriveKey(
		key, key.size(),
		reinterpret_cast<const byte*>(password.data()), password.size(),
		reinterpret_cast<const byte*>(iv.data()), iv.size(),
		nullptr, 0
	);

	// Set Key and IV to the encryptor
	m_aes_encryptor->SetKeyWithIV(key, CryptoPP::AES::MAX_KEYLENGTH, key + CryptoPP::AES::MAX_KEYLENGTH); // key, kl, iv, ivl
	
	// Encrypt
	 const CryptoPP::StringSource ss(
		buffer,
		true,
		new CryptoPP::AuthenticatedEncryptionFilter( // note: for GCM mode, use AuthenticatedEncryptionFilter instead of StreamTransformationFilter
			*m_aes_encryptor,
			new CryptoPP::StringSink(cipher)
		)
	);
	//NOTE: StringSource will auto clean the allocated memory

	// Output (AlgoType + IV + Cipher) since we need IV and Algorithm used for encryption later for decryption
	 output += iv; // Append IV
	 output += cipher; // Append Cipher

	return output;
}

String AES::Decrypt(const String& password, const String& iv_cipher)
{
	// Make sure decryption mode is initialized
	ENIGMA_ASSERT(m_aes_decryptor, "AES Decryptor is not initialized properly");

	// Split IV and Cipher from buffer (we output encrypted buffers as String(AlgoType + IV + Cipher))
	const String iv = iv_cipher.substr(sizeof(Algorithm::Type), CryptoPP::AES::BLOCKSIZE);
	ENIGMA_ASSERT_OR_THROW(!iv.empty(), "Failed to extract IV from cipher");
	const String cipher = iv_cipher.substr(sizeof(Algorithm::Type) + CryptoPP::AES::BLOCKSIZE, iv_cipher.size() - 1);
	ENIGMA_ASSERT_OR_THROW(!cipher.empty(), "Failed to extract cipher from cipher");

	// Recovered buffer
	String decrypted{};

	// Prepare Key
	CryptoPP::SecByteBlock key(CryptoPP::AES::MAX_KEYLENGTH + CryptoPP::AES::BLOCKSIZE);
		
	// Convert key to KDF SHA-256, which allows you to use a password smaller or larger than the cipher's key size
	CryptoPP::HKDF<CryptoPP::SHA256> hkdf;
	hkdf.DeriveKey(
		key, key.size(),
		reinterpret_cast<const byte*>(password.data()), password.size(),
		reinterpret_cast<const byte*>(iv.data()), iv.size(),
		nullptr, 0);
		
	// Set Key and IV to the decryptor
	m_aes_decryptor->SetKeyWithIV(key, CryptoPP::AES::MAX_KEYLENGTH, key + CryptoPP::AES::MAX_KEYLENGTH); // key, kl, iv, ivl

	 // Decrypt
	 const CryptoPP::StringSource ss(
		cipher,
		true,
		new CryptoPP::AuthenticatedDecryptionFilter(
			*m_aes_decryptor,
			new CryptoPP::StringSink(decrypted)
		)
	);
	 

	//NOTE: StringSource will auto clean the allocated memory

	return decrypted;
}


NS_ENIGMA_END
