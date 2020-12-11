#pragma once
#ifndef ENIGMA_CHACHA_H
#define ENIGMA_CHACHA_H

#include <Algorithm/Algorithm.hpp>

NS_ENIGMA_BEGIN
/*
*	ChaCha Algorithm Encryptor/Decryptor
*/
class ChaCha : public Algorithm
{
public:
	/*
	*	@param intent: Operation, Encrypt or Decrypt
	*/
	explicit ChaCha(Algorithm::Intent intent) noexcept;
	virtual ~ChaCha() noexcept;

public:
	String Encrypt(const String& password, const String& buffer) override;
	String Decrypt(const String& password, const String& buffer) override;

private:
	std::unique_ptr<CryptoPP::ChaCha::Encryption> m_chacha_encryption; // ChaCha encryptor
	std::unique_ptr<CryptoPP::ChaCha::Decryption> m_chacha_decryption; // ChaCha decryptor
};

NS_ENIGMA_END
#endif // !ENIGMA_CHACHA_H

