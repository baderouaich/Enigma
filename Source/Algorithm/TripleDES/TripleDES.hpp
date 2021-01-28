#pragma once
#ifndef ENIGMA_TRIPLE_DES_H
#define ENIGMA_TRIPLE_DES_H

#include <Algorithm/Algorithm.hpp>

NS_ENIGMA_BEGIN
/*
*	TripleDES-CBC Algorithm Encryptor/Decryptor
*/
class TripleDES : public Algorithm
{
public:
	/*
	*	@param intent: Operation, Encrypt or Decrypt
	*/
	explicit TripleDES(Algorithm::Intent intent) noexcept;
	virtual ~TripleDES() noexcept;

public:
	String Encrypt(const String& password, const String& buffer) noexcept(false) override;
	String Decrypt(const String& password, const String& iv_cipher) noexcept(false) override;

private: 
	// Note: using CBC since GCM is defined for block ciphers with a block size of 128 bits https://en.m.wikipedia.org/wiki/Galois/Counter_Mode
	std::unique_ptr<CryptoPP::CBC_Mode<CryptoPP::DES_EDE3>::Encryption> m_tripledes_encryption; // 3 Key TripleDES-CBC encryptor
	std::unique_ptr<CryptoPP::CBC_Mode<CryptoPP::DES_EDE3>::Decryption> m_tripledes_decryption; // 3 Key TripleDES-CBC decryptor
};

NS_ENIGMA_END
#endif // !ENIGMA_TRIPLE_DES_H

