#include "pch.hpp"
#include "TripleDES.hpp"

NS_ENIGMA_BEGIN

TripleDES::TripleDES(Algorithm::Intent intent) noexcept
	:
	Algorithm(Algorithm::Type::TripleDES, intent),
	m_tripledes_encryptor(intent == Algorithm::Intent::Encrypt ? std::make_unique<CryptoPP::CBC_Mode<CryptoPP::DES_EDE3>::Encryption>() : nullptr),
	m_tripledes_decryptor(intent == Algorithm::Intent::Decrypt ? std::make_unique<CryptoPP::CBC_Mode<CryptoPP::DES_EDE3>::Decryption>() : nullptr)
{
}
TripleDES::~TripleDES() noexcept
{
}

String TripleDES::Encrypt(const String& password, const String& buffer)
{
	// Make sure encryption mode and the seeder are initialized
	{
		ENIGMA_ASSERT_OR_THROW(m_tripledes_encryptor, "TripleDES Encryptor is not initialized properly");
		ENIGMA_ASSERT_OR_THROW(m_auto_seeded_random_pool, "TripleDES Encryptor seeder is not initialized properly");
	}

	// Validate Arguments
	{
		// Password length must be at least 9 for security reasons
		ENIGMA_ASSERT_OR_THROW(password.size() >= Constants::Algorithm::MINIMUM_PASSWORD_LENGTH, "TripleDES Minimum Password Length is " + std::to_string(Constants::Algorithm::MINIMUM_PASSWORD_LENGTH));
		//No max password check since we using KDF SHA-256, his allows you to use a password smaller or larger than the cipher's key size: https://crypto.stackexchange.com/questions/68299/length-of-password-requirement-using-openssl-aes-256-cbc
	}

	String iv = this->GenerateRandomIV(CryptoPP::DES_EDE3::BLOCKSIZE); // Randomly generated 8 bytes IV
	String cipher{}; // Final encrypted buffer
	String output(sizeof(Algorithm::Type), static_cast<const byte>(this->GetType())); // return value will be (AlgoType + IV + Cipher)

	// Prepare key
	CryptoPP::SecByteBlock key(CryptoPP::DES_EDE3::MAX_KEYLENGTH + CryptoPP::DES_EDE3::BLOCKSIZE); // Encryption key to be generated from user password + IV

	// Convert key to KDF SHA-256, which allows you to use a password smaller or larger than the cipher's key size
	CryptoPP::HKDF<CryptoPP::SHA256> hkdf;
	hkdf.DeriveKey(
		key, key.size(),
		reinterpret_cast<const byte*>(password.data()), password.size(),
		reinterpret_cast<const byte*>(iv.data()), iv.size(),
		nullptr, 0
	);

	// Set Key and IV to the encryptor
	m_tripledes_encryptor->SetKeyWithIV(key, CryptoPP::DES_EDE3::MAX_KEYLENGTH, key + CryptoPP::DES_EDE3::MAX_KEYLENGTH); // key, kl, iv, ivl

	// Encrypt
	const CryptoPP::StringSource ss(
		buffer,
		true,
		new CryptoPP::StreamTransformationFilter(
			*m_tripledes_encryptor,
			new CryptoPP::StringSink(cipher)
		)
		);
	//NOTE: StringSource will auto clean the allocated memory

	// Output (AlgoType + IV + Cipher) since we need IV and Algorithm used for encryption later for decryption
	output += iv; // Append IV
	output += cipher; // Append Cipher

	return output;
}

String TripleDES::Decrypt(const String& password, const String& iv_cipher)
{
	// Make sure decryption mode is initialized
	ENIGMA_ASSERT_OR_THROW(m_tripledes_decryptor, "TripleDES Decryptor is not initialized properly");

	// Split IV and Cipher from buffer (we output encrypted buffers as String(AlgoType + IV + Cipher))
	const String iv = iv_cipher.substr(sizeof(Algorithm::Type), CryptoPP::DES_EDE3::BLOCKSIZE);
	ENIGMA_ASSERT_OR_THROW(!iv.empty(), "Failed to extract IV from cipher");
	const String cipher = iv_cipher.substr(sizeof(Algorithm::Type) + CryptoPP::DES_EDE3::BLOCKSIZE, iv_cipher.size() - 1);
	ENIGMA_ASSERT_OR_THROW(!cipher.empty(), "Failed to extract cipher from cipher");

	// Recovered buffer
	String decrypted{};
	
	// Prepare Key
	CryptoPP::SecByteBlock key(CryptoPP::DES_EDE3::MAX_KEYLENGTH + CryptoPP::DES_EDE3::BLOCKSIZE);

	// Convert key to KDF SHA-256, which allows you to use a password smaller or larger than the cipher's key size
	CryptoPP::HKDF<CryptoPP::SHA256> hkdf;
	hkdf.DeriveKey(
		key, key.size(),
		reinterpret_cast<const byte*>(password.data()), password.size(),
		reinterpret_cast<const byte*>(iv.data()), iv.size(),
		nullptr, 0);

	// Set Key and IV to the decryptor
	m_tripledes_decryptor->SetKeyWithIV(key, CryptoPP::DES_EDE3::MAX_KEYLENGTH, key + CryptoPP::DES_EDE3::MAX_KEYLENGTH); // key, kl, iv, ivl

	// Decrypt
	const CryptoPP::StringSource decryptor(
		cipher,
		true,
		new CryptoPP::StreamTransformationFilter(
			*m_tripledes_decryptor, 
			new CryptoPP::StringSink(decrypted)
		)
	);

	//NOTE: StringSource will auto clean the allocated memory

	return decrypted;
}

NS_ENIGMA_END
