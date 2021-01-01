#pragma once
#ifndef ENIGMA_CHACHA_H
#define ENIGMA_CHACHA_H

#include <Algorithm/Algorithm.hpp>

NS_ENIGMA_BEGIN
/*
*	ChaCha20 Algorithm Encryptor/Decryptor
*/
class ChaCha20 : public Algorithm
{
public:
	/*
	*	@param intent: Operation, Encrypt or Decrypt
	*/
	explicit ChaCha20(Algorithm::Intent intent) noexcept;
	virtual ~ChaCha20() = default;

public:
	String Encrypt(const String& password, const String& buffer) override;
	String Decrypt(const String& password, const String& iv_cipher) override;

private:
	// The ChaCha20 algorithm is a stream cipher using a key of 256 bit size.
	// the ChaCha20 stream cipher does not have a block operation mode therefore it does not use padding.
	std::unique_ptr<CryptoPP::ChaCha::Encryption> m_chacha_encryption; // ChaCha20 encryptor
	std::unique_ptr<CryptoPP::ChaCha::Decryption> m_chacha_decryption; // ChaCha20 decryptor
};

NS_ENIGMA_END
#endif // !ENIGMA_CHACHA_H

