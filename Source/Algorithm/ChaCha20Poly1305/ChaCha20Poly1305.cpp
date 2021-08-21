#include "pch.hpp"
#include "ChaCha20Poly1305.hpp"

NS_ENIGMA_BEGIN

ChaCha20Poly1305::ChaCha20Poly1305(const Algorithm::Intent intent) noexcept
	:
	Algorithm(Algorithm::Type::ChaCha20Poly1305, intent)
{
	// Encrypting & Decrypting
	if (intent & Intent::Encrypt && intent & Intent::Decrypt)
	{
		m_chacha_encryptor = std::make_unique<CryptoPP::ChaCha20Poly1305::Encryption>();
		m_chacha_decryptor = std::make_unique<CryptoPP::ChaCha20Poly1305::Decryption>();
	}
	// Encrypting only
	else if (intent & Intent::Encrypt)
	{
		m_chacha_encryptor = std::make_unique<CryptoPP::ChaCha20Poly1305::Encryption>();
	}
	// Decrypting only
	else if (intent & Intent::Decrypt)
	{
		m_chacha_decryptor = std::make_unique<CryptoPP::ChaCha20Poly1305::Decryption>();
	}
}

ChaCha20Poly1305::~ChaCha20Poly1305() noexcept
{
}

String ChaCha20Poly1305::Encrypt(const String& password, const String& buffer)
{
	// Make sure encryption mode and the seeder are initialized & Validate Arguments
	{
		ENIGMA_ASSERT_OR_THROW(m_chacha_encryptor, "ChaCha20Poly1305 Encryptor is not initialized properly");
		// ChaCha20Poly1305 password length must be at least 9 for security reasons
		ENIGMA_ASSERT_OR_THROW(password.size() >= Constants::Algorithm::MINIMUM_PASSWORD_LENGTH, "ChaCha20Poly1305 Minimum Password Length is " + std::to_string(Constants::Algorithm::MINIMUM_PASSWORD_LENGTH));
		//No max password check since we using KDF SHA-256, this allows you to use a password smaller or larger than the cipher's key size: https://crypto.stackexchange.com/questions/68299/length-of-password-requirement-using-openssl-aes-256-cbc
	}
	
	// Randomly generated iv
	const String iv = Algorithm::GenerateRandomIV(m_chacha_encryptor->MaxIVLength()); 

	// Prepare key
	CryptoPP::SecByteBlock key(m_chacha_encryptor->MaxKeyLength() + m_chacha_encryptor->MaxIVLength());

	// Convert key to KDF SHA-256, which allows you to use a password smaller or larger than the cipher's key size
	CryptoPP::HKDF<CryptoPP::SHA256> hkdf{};
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
	m_chacha_encryptor->SetKeyWithIV(key, m_chacha_encryptor->MaxKeyLength(), key + m_chacha_encryptor->MaxKeyLength()); // key, kl, iv, ivl

	// Output encrypted buffer
	String cipher(buffer.size(), '\000'); 
	// Output calculated MAC
	String mac(16, '\000');

	// Encrypt & Authenticate (Thanks to @mcoret who mentioned this in https://github.com/BaderEddineOuaich/Enigma/issues/4)
	m_chacha_encryptor->EncryptAndAuthenticate(
		reinterpret_cast<byte*>(cipher.data()), // output cipher (encrypted buffer)
		reinterpret_cast<byte*>(mac.data()), mac.size(),  // output calculated MAC
		reinterpret_cast<const byte*>(iv.data()), static_cast<i32>(iv.size()), // iv
		nullptr, 0, // aad buffer (additional authenticated data)
		reinterpret_cast<const byte*>(buffer.data()), buffer.size() // buffer to encrypt
	);

	// Output will be (Algorithm Type + IV + MAC + Cipher)
	std::ostringstream output{};
	output 
		<< static_cast<char>(this->GetType()) // Append Algorithm Type (enum id)
		<< iv // Append IV
		<< mac // Append MAC
		<< cipher; // Append Cipher
	return output.str();
}

String ChaCha20Poly1305::Decrypt(const String& password, const String& algotype_iv_mac_cipher)
{
	// Make sure decryption mode is initialized
	ENIGMA_ASSERT_OR_THROW(m_chacha_decryptor, "ChaCha20Poly1305 Decryptor is not initialized properly");

	// Extract IV, mac and cipher from algotype_iv_mac_cipher (algotype_iv_mac_cipher is the output we got from encryption shipped with IV, MAC, Cipher, Algo type enum id)
	const String iv = algotype_iv_mac_cipher.substr(sizeof(Algorithm::Type), m_chacha_decryptor->MaxIVLength());
	ENIGMA_ASSERT_OR_THROW(!iv.empty(), "Failed to extract IV part from algotype_iv_mac_cipher");
		
	const String mac = algotype_iv_mac_cipher.substr(sizeof(Algorithm::Type) + iv.size(), 16); // mac is 16 bytes
	ENIGMA_ASSERT_OR_THROW(!mac.empty(), "Failed to extract MAC part from algotype_iv_mac_cipher");

	const String cipher = algotype_iv_mac_cipher.substr(sizeof(Algorithm::Type) + iv.size() + 16, algotype_iv_mac_cipher.size() - 1);
	ENIGMA_ASSERT_OR_THROW(!cipher.empty(), "Failed to extract cipher part from algotype_iv_mac_cipher");

	// Prepare Key
	CryptoPP::SecByteBlock key(m_chacha_decryptor->MaxKeyLength() + m_chacha_decryptor->MaxIVLength());

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
	m_chacha_decryptor->SetKeyWithIV(key, m_chacha_decryptor->MaxKeyLength(), key + m_chacha_decryptor->MaxKeyLength()); // key, kl, iv, ivl

	// Recovered cipher
	String decrypted(cipher.size(), '\000');

	// Decrypt and verify MAC
	const bool mac_verified = m_chacha_decryptor->DecryptAndVerify(
		reinterpret_cast<byte*>(decrypted.data()), // output buffer (decrypted cipher)
		reinterpret_cast<const byte*>(mac.data()), mac.size(), // input MAC (calculated in encryption)
		reinterpret_cast<const byte*>(iv.data()), static_cast<i32>(iv.size()), // input IV (generated in encryption)
		nullptr, 0, // aad buffer (additional authenticated data)
		reinterpret_cast<const byte*>(cipher.data()), cipher.size() // cipher to decrypt
	);
	// Throw an exception if mac was not verified (mac is auto calculated at encryption by encryptor->EncryptAndAuthenticate)
	ENIGMA_ASSERT_OR_THROW(mac_verified, ("Failed to verify MAC"));

	return decrypted;
}

NS_ENIGMA_END

