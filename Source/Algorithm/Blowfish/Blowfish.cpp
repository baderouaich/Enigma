#include <pch.hpp>
#include "Blowfish.hpp"

NS_ENIGMA_BEGIN

Blowfish::Blowfish(const Algorithm::Intent intent) noexcept
	:
	Algorithm(Algorithm::Type::Blowfish, intent)
{
	// Encrypting & Decrypting
	if (intent & Intent::Encrypt && intent & Intent::Decrypt)
	{
		m_blowfish_encryptor = std::make_unique<CryptoPP::EAX<CryptoPP::Blowfish>::Encryption>();
		m_blowfish_decryptor = std::make_unique<CryptoPP::EAX<CryptoPP::Blowfish>::Decryption>();
	}
	// Encrypting only
	else if (intent & Intent::Encrypt)
	{
		m_blowfish_encryptor = std::make_unique<CryptoPP::EAX<CryptoPP::Blowfish>::Encryption>();
	}
	// Decrypting only
	else if (intent & Intent::Decrypt)
	{
		m_blowfish_decryptor = std::make_unique<CryptoPP::EAX<CryptoPP::Blowfish>::Decryption>();
	}
}

Blowfish::~Blowfish() noexcept
{
}


String Blowfish::EncryptText(const String& password, const String& buffer)
{
	// Make sure encryption mode and the seeder are initialized & Validate Arguments
	{
		ENIGMA_ASSERT_OR_THROW(m_blowfish_encryptor, GetTypeString() + " Encryptor is not initialized properly");
		ENIGMA_ASSERT_OR_THROW(m_auto_seeded_random_pool, GetTypeString() + " Encryptor seeder is not initialized properly");
		// Blowfish password length must be at least 6 for security reasons
		ENIGMA_ASSERT_OR_THROW(password.size() >= Constants::Algorithm::MINIMUM_PASSWORD_LENGTH, "Blowfish Minimum Password Length is " + std::to_string(Constants::Algorithm::MINIMUM_PASSWORD_LENGTH));
		//No max password check since we using KDF SHA-256, this allows you to use a password smaller or larger than the cipher's key size: https://crypto.stackexchange.com/questions/68299/length-of-password-requirement-using-openssl-aes-256-cbc
	}

	// Randomly generated IV
	const String iv = Algorithm::GenerateRandomIV(CryptoPP::Blowfish::BLOCKSIZE); 

	// Prepare key
	CryptoPP::SecByteBlock key(CryptoPP::Blowfish::MAX_KEYLENGTH + CryptoPP::Blowfish::BLOCKSIZE); // Encryption key to be generated from user password + IV

	// Convert key to KDF SHA-256, which allows you to use a password smaller or larger than the cipher's key size
	CryptoPP::HKDF<CryptoPP::SHA256> hkdf{};
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
	[[maybe_unused]] const auto ss = CryptoPP::StringSource(
		buffer,
		true,
		new CryptoPP::AuthenticatedEncryptionFilter(
			*m_blowfish_encryptor,
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

String Blowfish::DecryptText(const String& password, const String& algotype_iv_cipher)
{
	// Make sure decryption mode is initialized
	ENIGMA_ASSERT(m_blowfish_decryptor, GetTypeString() + " Decryptor is not initialized properly");

	// Extract IV and Cipher from algotype_iv_cipher (we output cipher as AlgoType + IV + Cipher)
	const String iv = algotype_iv_cipher.substr(sizeof(Algorithm::Type), CryptoPP::Blowfish::BLOCKSIZE);
	ENIGMA_ASSERT_OR_THROW(!iv.empty(), "Failed to extract IV part from algotype_iv_cipher");
	const String cipher = algotype_iv_cipher.substr(sizeof(Algorithm::Type) + CryptoPP::Blowfish::BLOCKSIZE, algotype_iv_cipher.size() - 1);
	ENIGMA_ASSERT_OR_THROW(!cipher.empty(), "Failed to extract cipher part from algotype_iv_cipher");

	// Prepare Key
	CryptoPP::SecByteBlock key(CryptoPP::Blowfish::MAX_KEYLENGTH + CryptoPP::Blowfish::BLOCKSIZE);

	// Convert key to KDF SHA-256, which allows you to use a password smaller or larger than the cipher's key size
	CryptoPP::HKDF<CryptoPP::SHA256> hkdf{};
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
	); //NOTE: StringSource will auto clean the allocated memory

	return decrypted;
}

void Blowfish::EncryptFile(const String& password, const fs::path& filename)
{
}

void Blowfish::DecryptFile(const String& password, const fs::path& filename)
{
}


NS_ENIGMA_END
