#pragma once
#ifndef ENIGMA_RSA_H
#define ENIGMA_RSA_H

#include <Algorithm/Algorithm.hpp>
#include <Utility/Base64.hpp>

NS_ENIGMA_BEGIN
/**
*	RSA OAEP SHA256 Algorithm Encryptor/Decryptor
*/
class RSA : public Algorithm
{
	using RSA_Hash = CryptoPP::SHA256;
	template <typename Hash> 
	using RSA_Padding = CryptoPP::OAEP<Hash>;
	using RSA_Encryptor = CryptoPP::RSAES<RSA_Padding<RSA_Hash>>::Encryptor;
	using RSA_Decryptor = CryptoPP::RSAES<RSA_Padding<RSA_Hash>>::Decryptor;

	/*
	https://www.javamex.com/tutorials/cryptography/rsa_key_length.shtml
	https://stackoverflow.com/questions/39068473/what-is-the-maximum-length-of-private-and-public-rsa-keys
	Other authors have been more conservative. Ferguson & Schneier (2003) in 
	Practical Cryptography implied that 2048 bits would only be sufficient to
	keep data confidential for around 20 years,
	and were already recommending 4096 bit keys:
	"The absolute minimum size for n is 2048 bits or so if you want to protect your data for 20 years.
	[...] If you can afford it in your application, let n be 4096 bits long, or as close to this size as you can get it." (p. 233)
	*/

	// Supported RSA Key Sizes
	inline static constexpr std::array<ui32, 6> RSA_KEY_SIZES =
	{
		512,  // Weak and fast, Generates in about 0.00s (Intel i7-4770K 4 Physical CPUs 8 Logical CPUs) 3.40GHz
		1024, // Medium and fast, Generates in about 0.01s  
		2048, // Strong and fast, Generates in about 0.09s | Lifetime of data : Up to 2030
		3072, // Very Strong and fast, Generates in about 0.25s | Lifetime of data : Up to 2031 onwards
		4096, // Ultimate but slow, Generates in about 0.40s
		8192  // Insane but slow, Generates in about 15.0s
	};

	inline static const String RSA_PRIVATE_KEY_HEADING  = "-----BEGIN RSA PRIVATE KEY-----\n";
	inline static const String RSA_PRIVATE_KEY_TRAILING = "-----END RSA PRIVATE KEY-----";
	inline static const String RSA_PUBLIC_KEY_HEADING   = "-----BEGIN RSA PUBLIC KEY-----\n";
	inline static const String RSA_PUBLIC_KEY_TRAILING  = "-----END RSA PUBLIC KEY-----";

public:
	/** Returns length of maximum buffer RSA can encrypt with the selected key size */
	constexpr size_t GetMaximumBufferSize() const noexcept
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

public:
	/**
	*	@param intent: Operation, Encrypt or Decrypt
	*/
	explicit RSA(const Algorithm::Intent intent) noexcept
		:
		Algorithm(Algorithm::Type::RSA, intent),
		m_key_size(3072) // Default RSA key size
	{}
	virtual ~RSA() noexcept = default;

public:
	String Encrypt([[maybe_unused]] const String&, const String& buffer) override
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

		// Output will be cipher only, since we know its rsa and we send treat encryption & decryption in different way than other algos
		return cipher;
	}

	String Decrypt([[maybe_unused]] const String&, const String& cipher) override
	{		
		// Make sure we have private key to decrypt
		ENIGMA_ASSERT_OR_THROW(m_private_key, "RSA private key is empty! RSA::SetPrivateKey was never called");

		// Make RSA Decryptor with private key
		m_rsa_decryptor.reset(new RSA_Decryptor(*m_private_key));
		ENIGMA_ASSERT_OR_THROW(m_rsa_decryptor, "RSA Decryptor was not initialized properly");

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

public:
	void SetKeySize(const ui32 key_size) noexcept { m_key_size = key_size; }
	size_t GetKeySize() const noexcept { return m_key_size; }

public:
	/** Sets private key */
	void SetPrivateKey(const String& private_key_base64)
	{
		ENIGMA_ASSERT_OR_THROW(private_key_base64.size() > RSA_PRIVATE_KEY_HEADING.size() + RSA_PRIVATE_KEY_TRAILING.size(), "RSA private key is too short");

		if(!m_private_key)
			m_private_key.reset(new CryptoPP::RSA::PrivateKey());
		
		// Remove heading and trailing from private key -----BEGIN RSA PRIVATE KEY----- ...base64... -----END RSA PRIVATE KEY-----
		const String private_key_base64_stripped = private_key_base64.substr(RSA_PRIVATE_KEY_HEADING.size(), private_key_base64.size() - RSA_PRIVATE_KEY_TRAILING.size());
		// Decode private key base64
		const String private_key = Base64::Decode(private_key_base64_stripped);
		CryptoPP::StringSource ss(private_key, true);
		m_private_key->Load(ss);
	}

	/** Returns RSA Private Key in Base64 */
	String GetPrivateKey()
	{
		String private_key{};
		CryptoPP::StringSink ss(private_key);
		m_private_key->Save(ss);
		private_key = Base64::Encode(private_key);
		return	RSA_PRIVATE_KEY_HEADING +
						private_key +
				RSA_PRIVATE_KEY_TRAILING;
	}

	/** Sets public key  */
	void SetPublicKey(const String& public_key_base64)
	{
		ENIGMA_ASSERT_OR_THROW(public_key_base64.size() > RSA_PUBLIC_KEY_HEADING.size() + RSA_PUBLIC_KEY_TRAILING.size(), "RSA public key is too short");

		if (!m_public_key)
			m_public_key.reset(new CryptoPP::RSA::PublicKey());

		// Remove heading and trailing from public key  -----BEGIN RSA PUBLIC KEY----- & -----END RSA PUBLIC KEY-----
		const String public_key_base64_stripped = public_key_base64.substr(RSA_PUBLIC_KEY_HEADING.size(), public_key_base64.size() - RSA_PUBLIC_KEY_TRAILING.size());
		// Decode public key base64
		const String public_key = Base64::Decode(public_key_base64_stripped);
		CryptoPP::StringSource ss(public_key, true);
		m_public_key->Load(ss);

	}

	/** Returns RSA Public Key in Base64 */
	String GetPublicKey()
	{
		String public_key{};
		CryptoPP::StringSink ss(public_key);
		m_public_key->Save(ss);
		public_key = Base64::Encode(public_key);
		return	RSA_PUBLIC_KEY_HEADING +
						public_key +
				RSA_PUBLIC_KEY_TRAILING;
	}


private:
	std::unique_ptr<RSA_Encryptor> m_rsa_encryptor;// Encryption Scheme(OAEP using hash RSA_Hash)
	std::unique_ptr<RSA_Decryptor> m_rsa_decryptor;
	std::unique_ptr<CryptoPP::InvertibleRSAFunction> m_params;
	std::unique_ptr<CryptoPP::RSA::PrivateKey> m_private_key;
	std::unique_ptr<CryptoPP::RSA::PublicKey> m_public_key;
	ui32 m_key_size;
	
};


NS_ENIGMA_END
#endif // !ENIGMA_RSA_H



#if 0
public:
	void SetPrivateKey(const String& private_key_base64)
	{
		ENIGMA_ASSERT_OR_THROW(private_key_base64.size() > RSA_PRIVATE_KEY_HEADING.size() + RSA_PRIVATE_KEY_TRAILING.size(), "RSA private key is too short");

		if (!m_private_key)
			m_private_key.reset(new CryptoPP::RSA::PrivateKey());

		// Remove heading and trailing from private key -----BEGIN RSA PRIVATE KEY----- ...base64... -----END RSA PRIVATE KEY-----
		const String private_key_base64_stripped = private_key_base64.substr(RSA_PRIVATE_KEY_HEADING.size(), private_key_base64.size() - RSA_PRIVATE_KEY_TRAILING.size());
		// Decode private key base64
		const String private_key = Base64::Decode(private_key_base64_stripped);
		CryptoPP::StringSource ss(private_key, true);
		m_private_key->Load(ss);

#if Hex
		CryptoPP::HexDecoder hd(new CryptoPP::StringSource(private_key, true));
		m_private_key->Load(hd);
#endif
#if 0
		CryptoPP::StringSource ss(private_key, true);
		m_private_key->Load(ss);
#endif
	}
	/** Returns RSA Private Key in Base64 */
	String GetPrivateKey()
	{
		String private_key{};
		CryptoPP::StringSink ss(private_key);
		m_private_key->Save(ss);
		private_key = Base64::Encode(private_key);
		return	RSA_PRIVATE_KEY_HEADING +
			private_key +
			RSA_PRIVATE_KEY_TRAILING;
#if Hex
		String private_key{ "-----BEGIN RSA PRIVATE KEY-----\n" };
		CryptoPP::HexEncoder he(new CryptoPP::StringSink(private_key));
		m_private_key->Save(he);
		return private_key += "\n-----END RSA PRIVATE KEY-----";
#endif
#if HexNormal
		String private_key;
		CryptoPP::HexEncoder he(new CryptoPP::StringSink(private_key));
		m_private_key->Save(he);
		return private_key;
#endif

#if Normal
		String private_key;
		CryptoPP::StringSink ss(private_key);
		m_private_key->Save(ss);
		return private_key;
#endif
	}
	void SetPublicKey(const String& public_key_base64)
	{
		ENIGMA_ASSERT_OR_THROW(public_key_base64.size() > RSA_PUBLIC_KEY_HEADING.size() + RSA_PUBLIC_KEY_TRAILING.size(), "RSA public key is too short");

		if (!m_public_key)
			m_public_key.reset(new CryptoPP::RSA::PublicKey());

		// Remove heading and trailing from public key  -----BEGIN RSA PUBLIC KEY----- & -----END RSA PUBLIC KEY-----
		const String public_key_base64_stripped = public_key_base64.substr(RSA_PUBLIC_KEY_HEADING.size(), public_key_base64.size() - RSA_PUBLIC_KEY_TRAILING.size());
		// Decode public key base64
		const String public_key = Base64::Decode(public_key_base64_stripped);
		CryptoPP::StringSource ss(public_key, true);
		m_public_key->Load(ss);

#if Hex
		CryptoPP::HexDecoder hd(new CryptoPP::StringSource(public_key, true));
		m_public_key->Load(hd);
#endif
#if Normal
		CryptoPP::StringSource ss(public_key, true);
		m_public_key->Load(ss);
#endif
	}
	/** Returns RSA Public Key in Base64 */
	String GetPublicKey()
	{
		String public_key{};
		CryptoPP::StringSink ss(public_key);
		m_public_key->Save(ss);
		public_key = Base64::Encode(public_key);
		return	RSA_PUBLIC_KEY_HEADING +
			public_key +
			RSA_PUBLIC_KEY_TRAILING;

#if Hex
		String public_key{ "-----BEGIN RSA PUBLIC KEY-----\n" };
		CryptoPP::HexEncoder he(new CryptoPP::StringSink(public_key));
		m_public_key->Save(he);
		return public_key += "\n-----END RSA PUBLIC KEY-----";
#endif
#if 0
		String public_key;
		CryptoPP::HexEncoder he(new CryptoPP::StringSink(public_key));
		m_public_key->Save(he);
		return public_key;
#endif

#if 0
		String public_key;
		CryptoPP::StringSink ss(public_key);
		m_public_key->Save(ss);
		return public_key;
#endif
	}
#endif
