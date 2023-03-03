#include "pch.hpp"
#include "Twofish.hpp"

NS_ENIGMA_BEGIN

Twofish::Twofish(const Algorithm::Intent intent) noexcept
	:
	Algorithm(Algorithm::Type::Twofish, intent)
{
	// Encrypting & Decrypting
	if (intent & Intent::All)
	{
		m_twofish_encryptor = std::make_unique<CryptoPP::GCM<CryptoPP::Twofish>::Encryption>();
		m_twofish_decryptor = std::make_unique<CryptoPP::GCM<CryptoPP::Twofish>::Decryption>();
	}
	// Encrypting only
	else if (intent & Intent::Encrypt)
	{
		m_twofish_encryptor = std::make_unique<CryptoPP::GCM<CryptoPP::Twofish>::Encryption>();
	}
	// Decrypting only
	else if (intent & Intent::Decrypt)
	{
		m_twofish_decryptor = std::make_unique<CryptoPP::GCM<CryptoPP::Twofish>::Decryption>();
	}
}
Twofish::~Twofish() noexcept
{
}

String Twofish::Encrypt(const String& password, const String& buffer)
{
	// Make sure encryption mode and the seeder are initialized &  Validate Arguments
	{
		ENIGMA_ASSERT_OR_THROW(m_twofish_encryptor, "Twofish Encryptor is not initialized properly");
		ENIGMA_ASSERT_OR_THROW(m_auto_seeded_random_pool, "Twofish Encryptor seeder is not initialized properly");
		// Twofish password length must be at least 9 for security reasons
		ENIGMA_ASSERT_OR_THROW(password.size() >= Constants::Algorithm::MINIMUM_PASSWORD_LENGTH, "Twofish Minimum Password Length is " + std::to_string(Constants::Algorithm::MINIMUM_PASSWORD_LENGTH));
		//No max password check since we using KDF SHA-256, this allows you to use a password smaller or larger than the cipher's key size: https://crypto.stackexchange.com/questions/68299/length-of-password-requirement-using-openssl-aes-256-cbc
	}

	// Randomly generated IV
	const String iv = Algorithm::GenerateRandomIV(CryptoPP::Twofish::BLOCKSIZE); 
	
	// Prepare key
	CryptoPP::SecByteBlock key(static_cast<std::size_t>(CryptoPP::Twofish::MAX_KEYLENGTH) + static_cast<std::size_t>(CryptoPP::Twofish::BLOCKSIZE)); // Encryption key to be generated from user password + IV

	// Convert key to KDF SHA-256, which allows you to use a password smaller or larger than the cipher's key size
	CryptoPP::HKDF<CryptoPP::SHA256> hkdf{};
	hkdf.DeriveKey(
		key, key.size(),
		reinterpret_cast<const byte*>(password.data()), password.size(),
		reinterpret_cast<const byte*>(iv.data()), iv.size(),
		nullptr, 0
	);

	// Set Key and IV to the encryptor
	m_twofish_encryptor->SetKeyWithIV(key, CryptoPP::Twofish::MAX_KEYLENGTH, key + CryptoPP::Twofish::MAX_KEYLENGTH); // key, kl, iv, ivl

	// Encrypt
	String cipher{}; // Final encrypted buffer
	const CryptoPP::StringSource ss(
		buffer,
		true,
		new CryptoPP::AuthenticatedEncryptionFilter( // note: for GCM mode, use AuthenticatedEncryptionFilter instead of StreamTransformationFilter
			*m_twofish_encryptor,
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

String Twofish::Decrypt(const String& password, const String& algotype_iv_cipher)
{
	// Make sure decryption mode is initialized
	ENIGMA_ASSERT_OR_THROW(m_twofish_decryptor, "Twofish Decryptor is not initialized properly");

	// Extract IV and Cipher from algotype_iv_cipher (we output cipher as AlgoType + IV + Cipher)
	const String iv = algotype_iv_cipher.substr(sizeof(Algorithm::Type), CryptoPP::Twofish::BLOCKSIZE);
	ENIGMA_ASSERT_OR_THROW(!iv.empty(), "Failed to extract IV part from algotype_iv_cipher");
	const String cipher = algotype_iv_cipher.substr(sizeof(Algorithm::Type) + CryptoPP::Twofish::BLOCKSIZE, algotype_iv_cipher.size() - 1);
	ENIGMA_ASSERT_OR_THROW(!cipher.empty(), "Failed to extract cipher part from algotype_iv_cipher");

	// Prepare Key
	CryptoPP::SecByteBlock key(CryptoPP::Twofish::MAX_KEYLENGTH + CryptoPP::Twofish::BLOCKSIZE);

	// Convert key to KDF SHA-256, which allows you to use a password smaller or larger than the cipher's key size
	CryptoPP::HKDF<CryptoPP::SHA256> hkdf{};
	hkdf.DeriveKey(
		key, key.size(),
		reinterpret_cast<const byte*>(password.data()), password.size(),
		reinterpret_cast<const byte*>(iv.data()), iv.size(),
		nullptr, 0);

	// Set Key and IV to the decryptor
	m_twofish_decryptor->SetKeyWithIV(key, CryptoPP::Twofish::MAX_KEYLENGTH, key + CryptoPP::Twofish::MAX_KEYLENGTH); // key, kl, iv, ivl

	// Decrypt
	String decrypted{}; // Recovered buffer
	[[maybe_unused]] const auto ss = CryptoPP::StringSource(
		cipher,
		true,
		new CryptoPP::AuthenticatedDecryptionFilter(
			*m_twofish_decryptor,
			new CryptoPP::StringSink(decrypted)
		)); //NOTE: StringSource will auto clean the allocated memory

	return decrypted;
}

NS_ENIGMA_END
