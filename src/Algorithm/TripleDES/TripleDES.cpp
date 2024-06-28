#include "pch.hpp"
#include "TripleDES.hpp"

NS_ENIGMA_BEGIN

TripleDES::TripleDES(const Algorithm::Intent intent) noexcept
	:
	Algorithm(Algorithm::Type::TripleDES, intent)
{
	// Encrypting & Decrypting
	if (intent & Intent::All)
	{
		m_tripledes_encryptor = std::make_unique<CryptoPP::EAX<CryptoPP::DES_EDE3>::Encryption>();
		m_tripledes_decryptor = std::make_unique<CryptoPP::EAX<CryptoPP::DES_EDE3>::Decryption>();
	}
	// Encrypting only
	else if (intent & Intent::Encrypt)
	{
		m_tripledes_encryptor = std::make_unique<CryptoPP::EAX<CryptoPP::DES_EDE3>::Encryption>();
	}
	// Decrypting only
	else if (intent & Intent::Decrypt)
	{
		m_tripledes_decryptor = std::make_unique<CryptoPP::EAX<CryptoPP::DES_EDE3>::Decryption>();
	}
}
TripleDES::~TripleDES() noexcept
{
}

std::string TripleDES::Encrypt(const std::string& password, const std::string& buffer)
{
	// Make sure encryption mode and the seeder are initialized & Validate Arguments
	{
		ENIGMA_ASSERT_OR_THROW(m_tripledes_encryptor, "TripleDES Encryptor is not initialized properly");
		ENIGMA_ASSERT_OR_THROW(m_auto_seeded_random_pool, "TripleDES Encryptor seeder is not initialized properly");
		// Password length must be at least 9 for security reasons
		ENIGMA_ASSERT_OR_THROW(password.size() >= Constants::Algorithm::MINIMUM_PASSWORD_LENGTH, "TripleDES Minimum Password Length is " + std::to_string(Constants::Algorithm::MINIMUM_PASSWORD_LENGTH));
		//No max password check since we using KDF SHA-256, this allows you to use a password smaller or larger than the cipher's key size: https://crypto.stackexchange.com/questions/68299/length-of-password-requirement-using-openssl-aes-256-cbc
	}

	// Randomly generated IV
	const std::string iv = Algorithm::GenerateRandomIV(CryptoPP::DES_EDE3::BLOCKSIZE);

	// Prepare key
	CryptoPP::SecByteBlock key(static_cast<std::size_t>(CryptoPP::DES_EDE3::MAX_KEYLENGTH) + static_cast<std::size_t>(CryptoPP::DES_EDE3::BLOCKSIZE)); // Encryption key to be generated from user password + IV

	// Convert key to KDF SHA-256, which allows you to use a password smaller or larger than the cipher's key size
	CryptoPP::HKDF<CryptoPP::SHA256> hkdf{};
	hkdf.DeriveKey(
		key, key.size(),
		reinterpret_cast<const byte*>(password.data()), password.size(),
		reinterpret_cast<const byte*>(iv.data()), iv.size(),
		nullptr, 0
	);

	// Set Key and IV to the encryptor
	m_tripledes_encryptor->SetKeyWithIV(key, CryptoPP::DES_EDE3::MAX_KEYLENGTH, key + CryptoPP::DES_EDE3::MAX_KEYLENGTH); // key, kl, iv, ivl

	// Encrypt
	std::string cipher{}; // Final encrypted buffer
	[[maybe_unused]] const auto ss = CryptoPP::StringSource(
		buffer,
		true,
		//new CryptoPP::StreamTransformationFilter(
		new CryptoPP::AuthenticatedEncryptionFilter(
			*m_tripledes_encryptor,
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

std::string TripleDES::Decrypt(const std::string& password, const std::string& algotype_iv_cipher)
{
	// Make sure decryption mode is initialized
	ENIGMA_ASSERT_OR_THROW(m_tripledes_decryptor, "TripleDES Decryptor is not initialized properly");

	// Extract IV and Cipher from algotype_iv_cipher (we output cipher as AlgoType + IV + Cipher)
	const std::string iv = algotype_iv_cipher.substr(sizeof(Algorithm::Type), CryptoPP::DES_EDE3::BLOCKSIZE);
	ENIGMA_ASSERT_OR_THROW(!iv.empty(), "Failed to extract IV part from algotype_iv_cipher");
	const std::string cipher = algotype_iv_cipher.substr(sizeof(Algorithm::Type) + CryptoPP::DES_EDE3::BLOCKSIZE, algotype_iv_cipher.size() - 1);
	ENIGMA_ASSERT_OR_THROW(!cipher.empty(), "Failed to extract cipher part from algotype_iv_cipher");

	// Prepare Key
	CryptoPP::SecByteBlock key(CryptoPP::DES_EDE3::MAX_KEYLENGTH + CryptoPP::DES_EDE3::BLOCKSIZE);

	// Convert key to KDF SHA-256, which allows you to use a password smaller or larger than the cipher's key size
	CryptoPP::HKDF<CryptoPP::SHA256> hkdf{};
	hkdf.DeriveKey(
		key, key.size(),
		reinterpret_cast<const byte*>(password.data()), password.size(),
		reinterpret_cast<const byte*>(iv.data()), iv.size(),
		nullptr, 0);

	// Set Key and IV to the decryptor
	m_tripledes_decryptor->SetKeyWithIV(key, CryptoPP::DES_EDE3::MAX_KEYLENGTH, key + CryptoPP::DES_EDE3::MAX_KEYLENGTH); // key, kl, iv, ivl

	// Decrypt
	std::string decrypted{}; // Recovered buffer
	const CryptoPP::StringSource decryptor(
		cipher,
		true,
		new CryptoPP::AuthenticatedDecryptionFilter(
			*m_tripledes_decryptor, 
			new CryptoPP::StringSink(decrypted)
		)); //NOTE: StringSource will auto clean the allocated memory
	return decrypted;
}

NS_ENIGMA_END
