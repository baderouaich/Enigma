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
	explicit AES(Algorithm::Intent intent) noexcept;
	virtual ~AES() = default;

public:
	String Encrypt(const String& password, const String& buffer) override;
	String Decrypt(const String& password, const String& iv_cipher) override;

private:
	// AES symmetric cipher is recommended to be used with GCM mode
	std::unique_ptr<CryptoPP::GCM<CryptoPP::AES>::Encryption> m_aes_encryption; // AES-GCM encryptor
	std::unique_ptr<CryptoPP::GCM<CryptoPP::AES>::Decryption> m_aes_decryption; // AES-GCM decryptor
};

NS_ENIGMA_END
#endif // !ENIGMA_AES_H

