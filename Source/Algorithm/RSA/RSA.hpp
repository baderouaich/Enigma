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
	inline static constexpr std::array<ui32, 6 + 3> RSA_KEY_SIZES =
	{
		512,   // Generates in about 0.00s (Intel i7-4770K 4 Physical CPUs 8 Logical CPUs) 3.40GHz
		1024,  // Generates in about 0.01s  
		2048,  // Generates in about 0.09s | Lifetime of data : Up to 2030
		3072,  // Generates in about 0.25s | Lifetime of data : Up to 2031 onwards
		4096,  // Generates in about 0.40s
		8192,  // Generates in about 10.0s 
		16384, // Generates in about 16.6s
		32768, // Generates in about 2422.92s (40 minutes)
		65536, // Generates in about 9005.5s (2.50 hours)
	};

	// Heading & trailing to wrap up generated base64 private & public keys
	inline static const String RSA_PRIVATE_KEY_HEADING  = "-----BEGIN RSA PRIVATE KEY-----\n";
	inline static const String RSA_PRIVATE_KEY_TRAILING = "-----END RSA PRIVATE KEY-----";
	inline static const String RSA_PUBLIC_KEY_HEADING   = "-----BEGIN RSA PUBLIC KEY-----\n";
	inline static const String RSA_PUBLIC_KEY_TRAILING  = "-----END RSA PUBLIC KEY-----";

public:
	/** Returns length of maximum buffer RSA can encrypt with the selected key size */
	size_t GetMaximumBufferSize() const noexcept;

public:
	/**
	*	@param intent: Operation, Encrypt or Decrypt
	*/
	explicit RSA(const Algorithm::Intent intent) noexcept;
	virtual ~RSA() noexcept = default;

public:
	/** Generates private and public key and encrypts buffer
	*	@returns cipher
	*	@throws CryptoPP::Exception, std::exception on failure
	*/
	String Encrypt([[maybe_unused]] const String&, const String& buffer) override;

	/** Decrypts cipher
	*	@returns recovered buffer
	*	@throws CryptoPP::Exception, std::exception on failure
	*	@note RSA::SetPrivateKey must be called before decrypting a cipher
	*/
	String Decrypt([[maybe_unused]] const String&, const String& cipher) override;

public:
	void SetKeySize(const ui32 key_size) noexcept { m_key_size = key_size; }
	size_t GetKeySize() const noexcept { return m_key_size; }

public:
	/** Sets private key
	*	@throws CryptoPP::Exception on failure
	*/
	void SetPrivateKey(const String& private_key_base64);

	/** Returns RSA Private Key in Base64 wrapped between -----BEGIN RSA PRIVATE KEY----- and -----END RSA PRIVATE KEY-----
	*	@throws CryptoPP::Exception on failure
	*/
	String GetPrivateKey();

	/** Sets public key  
	*	@throws CryptoPP::Exception on failure
	*/
	void SetPublicKey(const String& public_key_base64);

	/** Returns RSA Public Key in Base64 wrapped between -----BEGIN RSA PUBLIC KEY----- and -----END RSA PUBLIC KEY----- 
	*	@throws CryptoPP::Exception on failure
	*/
	String GetPublicKey();

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
