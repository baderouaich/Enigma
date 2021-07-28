#pragma once
#ifndef ENIGMA_CHACHA_H
#define ENIGMA_CHACHA_H

#include <Algorithm/Algorithm.hpp>

NS_ENIGMA_BEGIN
/*
*	ChaCha20Poly1305 Algorithm Encryptor/Decryptor
*/
class ChaCha20Poly1305 : public Algorithm
{
public:
	/*
	*	@param intent: Operation, Encrypt or Decrypt
	*/
	explicit ChaCha20Poly1305(Algorithm::Intent intent) noexcept;
	virtual ~ChaCha20Poly1305() noexcept;

public:
	String Encrypt(const String& password, const String& buffer) override;
	String Decrypt(const String& password, const String& algotype_iv_mac_cipher) override;
private:
	// ChaCha20Poly1305 is an authenticated encryption scheme that combines ChaChaTLS and Poly1305.
	// The scheme uses the IETF versions of the ciphers because it is defined in RFC 8439, ChaCha20and Poly1305 for IETF Protocols.
	// The scheme is somewhat unique because it pairs a stream cipher with an authenticator(most authenticated encryption modes pair a block cipher).
	std::unique_ptr<CryptoPP::ChaCha20Poly1305::Encryption> m_chacha_encryptor; // ChaCha20Poly1305 encryptor
	std::unique_ptr<CryptoPP::ChaCha20Poly1305::Decryption> m_chacha_decryptor; // ChaCha20Poly1305 decryptor

};

NS_ENIGMA_END
#endif // !ENIGMA_CHACHA_H

