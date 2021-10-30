#pragma once
#ifndef ENIGMA_TWOFISH_H
#define ENIGMA_TWOFISH_H

#include <Algorithm/Algorithm.hpp>

NS_ENIGMA_BEGIN
/**
*	Twofish-GCM Algorithm Encryptor/Decryptor
*/
class Twofish : public Algorithm
{
public:
	/**
	*	@param intent: Operation, Encrypt or Decrypt
	*/
	explicit Twofish(const Algorithm::Intent intent) noexcept;
	virtual ~Twofish() noexcept;

public:
	String EncryptText(const String& password, const String& buffer) override;
	String DecryptText(const String& password, const String& algotype_iv_cipher) override;
	void EncryptFile(const String& password, const fs::path& filename) override;
	void DecryptFile(const String& password, const fs::path& filename) override;

private:
	std::unique_ptr<CryptoPP::GCM<CryptoPP::Twofish>::Encryption> m_twofish_encryptor; /**< Twofish-GCM encryptor */
	std::unique_ptr<CryptoPP::GCM<CryptoPP::Twofish>::Decryption> m_twofish_decryptor; /**< Twofish-GCM decryptor */
};

NS_ENIGMA_END
#endif // !ENIGMA_TWOFISH_H

