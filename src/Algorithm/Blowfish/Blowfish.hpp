#pragma once
#ifndef ENIGMA_BLOWFISH_H
#define ENIGMA_BLOWFISH_H

#include <Algorithm/Algorithm.hpp>

NS_ENIGMA_BEGIN
/**
*	Blowfish Algorithm Encryptor/Decryptor
*/
class Blowfish : public Algorithm
{
public:
	/**
	*	@param intent: Operation, Encrypt or Decrypt
	*/
	explicit Blowfish(const Algorithm::Intent intent) noexcept;
	virtual ~Blowfish() noexcept;

public:
	std::string Encrypt(const std::string& password, const std::string& buffer) override;
	std::string Decrypt(const std::string& password, const std::string& algotype_iv_cipher) override;

private:
	std::unique_ptr<CryptoPP::EAX<CryptoPP::Blowfish>::Encryption> m_blowfish_encryptor; /**< Blowfish-EAX encryptor */
	std::unique_ptr<CryptoPP::EAX<CryptoPP::Blowfish>::Decryption> m_blowfish_decryptor; /**< Blowfish-EAX decryptor */
};

NS_ENIGMA_END
#endif // !ENIGMA_BLOWFISH_H


