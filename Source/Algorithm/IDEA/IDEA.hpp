#pragma once
#ifndef ENIGMA_IDEA_H
#define ENIGMA_IDEA_H

#include <Algorithm/Algorithm.hpp>

NS_ENIGMA_BEGIN
/*
*	IDEA-CBC Algorithm Encryptor/Decryptor
*/
class IDEA : public Algorithm
{
public:
	/*
	*	@param intent: Operation, Encrypt or Decrypt
	*/
	explicit IDEA(Algorithm::Intent intent) noexcept;
	virtual ~IDEA() noexcept;

public:
	String Encrypt(const String& password, const String& buffer) override;
	String Decrypt(const String& password, const String& iv_cipher) override;

private:
	std::unique_ptr<CryptoPP::CBC_Mode<CryptoPP::IDEA>::Encryption> m_idea_encryptor; // IDEA-CBC encryptor
	std::unique_ptr<CryptoPP::CBC_Mode<CryptoPP::IDEA>::Decryption> m_idea_decryptor; // IDEA-CBC decryptor
};

NS_ENIGMA_END
#endif // !ENIGMA_IDEA_H


