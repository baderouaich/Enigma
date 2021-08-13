#include "pch.hpp"
#include "RSA.h"
#include "RSA.hpp"

NS_ENIGMA_BEGIN

RSA::RSA(const Algorithm::Intent intent) noexcept
	:
	Algorithm(Algorithm::Type::RSA, intent),
	m_key_size(3072) // Default RSA key size
{}

String RSA::Encrypt(const String&, const String& buffer)
{
	// Generate random rsa key
	m_params.reset(new CryptoPP::InvertibleRSAFunction());
	m_params->GenerateRandomWithKeySize(*m_auto_seeded_random_pool, m_key_size);

	// Make private key with the generated random key
	m_private_key.reset(new CryptoPP::RSA::PrivateKey(*m_params));

	// Make public key with the generated random key
	m_public_key.reset(new CryptoPP::RSA::PublicKey(*m_params));

	// Make RSA Encryptor with public key
	m_rsa_encryptor.reset(new RSA_Encryptor(*m_public_key));
	// Initialize rsa encryptor with public key
	ENIGMA_ASSERT_OR_THROW(m_rsa_encryptor, "RSA Encryptor was not initialized properly");

	// Encrypt
	String cipher{};
	[[maybe_unused]] const auto ss = CryptoPP::StringSource(buffer, true,
		new CryptoPP::PK_EncryptorFilter(*m_auto_seeded_random_pool,
			*m_rsa_encryptor,
			new CryptoPP::StringSink(cipher)
		) // PK_EncryptorFilter
	); // StringSource
	//NOTE: StringSource will auto clean the allocated memory

	// Output will be algotype + cipher
	return static_cast<char>(this->GetType()) + cipher;
}

String RSA::Decrypt(const String&, const String& cipher)
{
	// Make sure we have private key to decrypt
	ENIGMA_ASSERT_OR_THROW(m_private_key, "RSA private key is empty! RSA::SetPrivateKey was never called");

	// Make RSA Decryptor with private key
	m_rsa_decryptor.reset(new RSA_Decryptor(*m_private_key));
	ENIGMA_ASSERT_OR_THROW(m_rsa_decryptor, "RSA Decryptor was not initialized properly");

	// Erase algo type first character from cipher
	auto s1 = cipher.size();
	const_cast<String&>(cipher).erase(cipher.begin() + 0);
	auto s2 = cipher.size();
	ENIGMA_ASSERT(s1 > s2, "const_cast failed");

	// Decrypt
	String recovered{};
	[[maybe_unused]] const auto ss = CryptoPP::StringSource(cipher, true,
		new CryptoPP::PK_DecryptorFilter(*m_auto_seeded_random_pool,
			*m_rsa_decryptor,
			new CryptoPP::StringSink(recovered)
		) // PK_DecryptorFilter
	); // StringSource

	return recovered;
}

void RSA::SetPrivateKey(const String& private_key_base64)
{
	ENIGMA_ASSERT_OR_THROW(private_key_base64.size() > RSA_PRIVATE_KEY_HEADING.size() + RSA_PRIVATE_KEY_TRAILING.size(), "RSA private key is too short");

	if (!m_private_key)
		m_private_key.reset(new CryptoPP::RSA::PrivateKey());

	// Remove heading and trailing from private key -----BEGIN RSA PRIVATE KEY----- ...base64... -----END RSA PRIVATE KEY-----
	const String private_key_base64_stripped = private_key_base64.substr(RSA_PRIVATE_KEY_HEADING.size(), private_key_base64.size() - RSA_PRIVATE_KEY_TRAILING.size());
	
	// Decode private key base64
	const String private_key = Base64::Decode(private_key_base64_stripped);
	
	// Load private key
	CryptoPP::StringSource ss(private_key, true);
	m_private_key->Load(ss);
}

String RSA::GetPrivateKey()
{
	String private_key{};
	CryptoPP::StringSink ss(private_key);
	m_private_key->Save(ss);
	private_key = Base64::Encode(private_key);
	return	RSA_PRIVATE_KEY_HEADING +
				private_key +
			RSA_PRIVATE_KEY_TRAILING;
}

void RSA::SetPublicKey(const String& public_key_base64)
{
	ENIGMA_ASSERT_OR_THROW(public_key_base64.size() > RSA_PUBLIC_KEY_HEADING.size() + RSA_PUBLIC_KEY_TRAILING.size(), "RSA public key is too short");

	if (!m_public_key)
		m_public_key.reset(new CryptoPP::RSA::PublicKey());

	// Remove heading and trailing from public key  -----BEGIN RSA PUBLIC KEY----- & -----END RSA PUBLIC KEY-----
	const String public_key_base64_stripped = public_key_base64.substr(RSA_PUBLIC_KEY_HEADING.size(), public_key_base64.size() - RSA_PUBLIC_KEY_TRAILING.size());
	
	// Decode public key base64
	const String public_key = Base64::Decode(public_key_base64_stripped);
	
	// Load public key
	CryptoPP::StringSource ss(public_key, true);
	m_public_key->Load(ss);

}

String RSA::GetPublicKey()
{
	String public_key{};
	CryptoPP::StringSink ss(public_key);
	m_public_key->Save(ss);
	public_key = Base64::Encode(public_key);
	return	RSA_PUBLIC_KEY_HEADING +
				public_key +
			RSA_PUBLIC_KEY_TRAILING;
}

size_t RSA::GetMaximumBufferSize() const noexcept
{
	/*
	https://stackoverflow.com/questions/25070949/cryptopp-plaintext-fixed-length-limit
	https://ietf.org/rfc/rfc2437.txt => Section 7.1.1
	7.1.1 Encryption operation

	   RSAES-OAEP-ENCRYPT ((n, e), M, P)

	   Input:
	   (n, e)    recipient's RSA public key

	   M         message to be encrypted, an octet string of length at
				 most k-2-2hLen, where k is the length in octets of the
				 modulus n and hLen is the length in octets of the hash
				 function output for EME-OAEP

	   P         encoding parameters, an octet string that may be empty
	*/

	// k - 2 - 2 * hLen
	const size_t k = static_cast<size_t>(m_key_size) / 8;
	constexpr const size_t hLen = RSA_Hash::DIGESTSIZE;
	return static_cast<size_t>(k - 2 - 2 * hLen);
}

NS_ENIGMA_END
