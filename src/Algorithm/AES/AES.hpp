#pragma once
#ifndef ENIGMA_AES_H
#define ENIGMA_AES_H

#include <Algorithm/Algorithm.hpp>

NS_ENIGMA_BEGIN
/**
*	AES-GCM Algorithm Encryptor/Decryptor
* 
*	AES symmetric cipher is recommended to be used with GCM mode
*/
class AES : public Algorithm
{
public:
	/**
	*	@param intent: Operation, Encrypt or Decrypt
	*/
	explicit AES(const Algorithm::Intent intent) noexcept;
	virtual ~AES() noexcept;

public:
	String Encrypt(const String& password, const String& buffer) override;
	String Decrypt(const String& password, const String& algotype_iv_cipher) override;

private:
	std::unique_ptr<CryptoPP::GCM<CryptoPP::AES>::Encryption> m_aes_encryptor; /**< AES-GCM encryptor */
	std::unique_ptr<CryptoPP::GCM<CryptoPP::AES>::Decryption> m_aes_decryptor; /**< AES-GCM decryptor */
};

NS_ENIGMA_END
#endif // !ENIGMA_AES_H

