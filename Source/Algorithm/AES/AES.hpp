#pragma once
#ifndef ENIGMA_AES_H
#define ENIGMA_AES_H

#include <Algorithm/Algorithm.hpp>

NS_ENIGMA_BEGIN
/*
*	AES Algorithm Encryptor/Decryptor
*/
class AES : public Algorithm
{
public:
	/*
	*	@param intent: Operation, Encrypt or Decrypt
	*/
	AES(Algorithm::Intent intent) noexcept;
	virtual ~AES() noexcept;

public:
	String Encrypt(const String& password, const String& buffer) override;
	String Decrypt(const String& password, const String& buffer) override;

private:
	std::unique_ptr<CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption> m_aes_encryption; // AES-CBC encryptor
	std::unique_ptr<CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption> m_aes_decryption; // AES-CBC decryptor

};

NS_ENIGMA_END
#endif // !ENIGMA_AES_H

