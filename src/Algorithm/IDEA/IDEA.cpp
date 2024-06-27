#include "pch.hpp"
#include "IDEA.hpp"

NS_ENIGMA_BEGIN

IDEA::IDEA(const Algorithm::Intent intent) noexcept
	:
	Algorithm(Algorithm::Type::IDEA, intent)
{
	// Encrypting & Decrypting
	if (intent & Intent::All)
	{
		m_idea_encryptor = std::make_unique<CryptoPP::EAX<CryptoPP::IDEA>::Encryption>();
		m_idea_decryptor = std::make_unique<CryptoPP::EAX<CryptoPP::IDEA>::Decryption>();
	}
	// Encrypting only
	else if (intent & Intent::Encrypt)
	{
		m_idea_encryptor = std::make_unique<CryptoPP::EAX<CryptoPP::IDEA>::Encryption>();
	}
	// Decrypting only
	else if (intent & Intent::Decrypt)
	{
		m_idea_decryptor = std::make_unique<CryptoPP::EAX<CryptoPP::IDEA>::Decryption>();
	}
}

IDEA::~IDEA() noexcept
{
}

String IDEA::Encrypt(const String& password, const String& buffer)
{
	// Make sure encryption mode and the seeder are initialized & Validate Arguments
	{
		ENIGMA_ASSERT_OR_THROW(m_idea_encryptor, "IDEA Encryptor is not initialized properly");
		ENIGMA_ASSERT_OR_THROW(m_auto_seeded_random_pool, "IDEA Encryptor seeder is not initialized properly");
		// Password length must be at least 9 for security reasons
		ENIGMA_ASSERT_OR_THROW(password.size() >= Constants::Algorithm::MINIMUM_PASSWORD_LENGTH, "IDEA Minimum Password Length is " + std::to_string(Constants::Algorithm::MINIMUM_PASSWORD_LENGTH));
		//No max password check since we using KDF SHA-256, his allows you to use a password smaller or larger than the cipher's key size: https://crypto.stackexchange.com/questions/68299/length-of-password-requirement-using-openssl-aes-256-cbc
	}


	// Randomly generated IV
	const String iv = Algorithm::GenerateRandomIV(CryptoPP::IDEA::BLOCKSIZE);

	// Prepare key
	CryptoPP::SecByteBlock key(static_cast<std::size_t>(CryptoPP::IDEA::MAX_KEYLENGTH) + static_cast<std::size_t>(CryptoPP::IDEA::BLOCKSIZE)); // Encryption key to be generated from user password + IV

	// Convert key to KDF SHA-256, which allows you to use a password smaller or larger than the cipher's key size
	CryptoPP::HKDF<CryptoPP::SHA256> hkdf{};
	hkdf.DeriveKey(
		key, key.size(),
		reinterpret_cast<const byte*>(password.data()), password.size(),
		reinterpret_cast<const byte*>(iv.data()), iv.size(),
		nullptr, 0
	);

	// Set Key and IV to the encryptor
	m_idea_encryptor->SetKeyWithIV(key, CryptoPP::IDEA::MAX_KEYLENGTH, key + CryptoPP::IDEA::MAX_KEYLENGTH); // key, kl, iv, ivl

	// Encrypt
	String cipher{}; // Final encrypted buffer
	[[maybe_unused]] const auto ss = CryptoPP::StringSource(
		buffer,
		true,
		//new CryptoPP::StreamTransformationFilter(
		new CryptoPP::AuthenticatedEncryptionFilter(
			*m_idea_encryptor,
			new CryptoPP::StringSink(cipher)
		)); //NOTE: StringSource will auto clean the allocated memory
	

	// Output will be (Algorithm Type + IV + Cipher) since we need IV and Algorithm used for encryption later for decryption
	std::ostringstream output{};
	output
		<< static_cast<char>(this->GetType()) // Append Algorithm Type (enum id)
		<< iv // Append IV
		<< cipher; // Append Cipher
	return output.str();
}

String IDEA::Decrypt(const String& password, const String& algotype_iv_cipher)
{
	// Make sure decryption mode is initialized
	ENIGMA_ASSERT_OR_THROW(m_idea_decryptor, "IDEA Decryptor is not initialized properly");

	// Extract IV and Cipher from algotype_iv_cipher (we output cipher as AlgoType + IV + Cipher)
	const String iv = algotype_iv_cipher.substr(sizeof(Algorithm::Type), CryptoPP::IDEA::BLOCKSIZE);
	ENIGMA_ASSERT_OR_THROW(!iv.empty(), "Failed to extract IV part from algotype_iv_cipher");
	const String cipher = algotype_iv_cipher.substr(sizeof(Algorithm::Type) + CryptoPP::IDEA::BLOCKSIZE, algotype_iv_cipher.size() - 1);
	ENIGMA_ASSERT_OR_THROW(!cipher.empty(), "Failed to extract cipher part from algotype_iv_cipher");

	// Prepare Key
	CryptoPP::SecByteBlock key(CryptoPP::IDEA::MAX_KEYLENGTH + CryptoPP::IDEA::BLOCKSIZE);

	// Convert key to KDF SHA-256, which allows you to use a password smaller or larger than the cipher's key size
	CryptoPP::HKDF<CryptoPP::SHA256> hkdf{};
	hkdf.DeriveKey(
		key, key.size(),
		reinterpret_cast<const byte*>(password.data()), password.size(),
		reinterpret_cast<const byte*>(iv.data()), iv.size(),
		nullptr, 0);

	// Set Key and IV to the decryptor
	m_idea_decryptor->SetKeyWithIV(key, CryptoPP::IDEA::MAX_KEYLENGTH, key + CryptoPP::IDEA::MAX_KEYLENGTH); // key, kl, iv, ivl

	// Decrypt
	String decrypted{}; // Recovered buffer
	const CryptoPP::StringSource ss(
		cipher,
		true,
		new CryptoPP::AuthenticatedDecryptionFilter(
			*m_idea_decryptor,
			new CryptoPP::StringSink(decrypted)
		)); //NOTE: StringSource will auto clean the allocated memory

	return decrypted;
}

NS_ENIGMA_END
