#pragma once
#ifndef ENIGMA_TRIPLE_DES_H
#define ENIGMA_TRIPLE_DES_H

#include <Algorithm/Algorithm.hpp>

NS_ENIGMA_BEGIN
/*
*	TripleDES Algorithm Encryptor/Decryptor
*/
class TripleDES : public Algorithm
{
public:
	/*
	*	@param intent: Operation, Encrypt or Decrypt
	*/
	explicit TripleDES(Algorithm::Intent intent) noexcept;
	virtual ~TripleDES() = default;

public:
	String Encrypt(const String& password, const String& buffer) override;
	String Decrypt(const String& password, const String& buffer) override;

private: 
	std::unique_ptr<CryptoPP::CBC_Mode<CryptoPP::DES_EDE3>::Encryption> m_tripledes_encryption; // 3 Key TripleDES-CBC encryptor
	std::unique_ptr<CryptoPP::CBC_Mode<CryptoPP::DES_EDE3>::Decryption> m_tripledes_decryption; // 3 Key TripleDES-CBC decryptor
};

NS_ENIGMA_END
#endif // !ENIGMA_TRIPLE_DES_H

