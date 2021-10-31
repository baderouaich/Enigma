#pragma once
#ifndef ENIGMA_TRIPLE_DES_H
#define ENIGMA_TRIPLE_DES_H

#include <Algorithm/Algorithm.hpp>

NS_ENIGMA_BEGIN
/**
*	TripleDES Algorithm Encryptor/Decryptor
*	
*	TripleDES, TDEA or 3DES, is multiple applications of the DES algorithm to plain text data to increase the security
*	of the original DES algorithm.As the name implies, the DES algorithm is applied 3 times.There are two variants
*	of TripleDES : the first is two key;and the second is three key. 2 - key TDEA provides approximately 80 bits of security,
*	while 3 - key TDEA provides approximately 112 bits of security.In contrast, AES provides a minimum security level of 128.
*/
class TripleDES : public Algorithm
{
public:
	/**
	*	@param intent: Operation, Encrypt or Decrypt
	*/
	explicit TripleDES(const Algorithm::Intent intent) noexcept;
	virtual ~TripleDES() noexcept;

public:
	String EncryptText(const String& password, const String& buffer) override;
	String DecryptText(const String& password, const String& algotype_iv_cipher) override;
	void EncryptFile(const String& password, const fs::path& in_filename, const fs::path& out_filename) override;
	void DecryptFile(const String& password, const fs::path& in_filename, const fs::path& out_filename) override;

private: 
	std::unique_ptr<CryptoPP::EAX<CryptoPP::DES_EDE3>::Encryption> m_tripledes_encryptor; /**< 3 Key TripleDES-EAX encryptor */
	std::unique_ptr<CryptoPP::EAX<CryptoPP::DES_EDE3>::Decryption> m_tripledes_decryptor; /**< 3 Key TripleDES-EAX decryptor */
};

NS_ENIGMA_END
#endif // !ENIGMA_TRIPLE_DES_H

