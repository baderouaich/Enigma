#pragma once
#ifndef ENIGMA_IDEA_H
#define ENIGMA_IDEA_H

#include <Algorithm/Algorithm.hpp>

NS_ENIGMA_BEGIN
/**
*	IDEA-EAX Algorithm Encryptor/Decryptor
* 
*	IDEA is the International Data Encryption Standard by Massey and Lai. The cipher is a 64-bit block cipher which uses 126-bit
*	keys and a 64-bit initialization vector (IV). IDEA is a popular choice in open source projects such as OpenPGP.
*/
class IDEA : public Algorithm
{
public:
	/**
	*	@param intent: Operation, Encrypt or Decrypt
	*/
	explicit IDEA(const Algorithm::Intent intent) noexcept;
	virtual ~IDEA() noexcept;

public:
	String EncryptText(const String& password, const String& buffer) override;
	String DecryptText(const String& password, const String& algotype_iv_cipher) override;
	void EncryptFile(const String& password, const fs::path& in_filename, const fs::path& out_filename) override;
	void DecryptFile(const String& password, const fs::path& in_filename, const fs::path& out_filename) override;

private:
	std::unique_ptr<CryptoPP::EAX<CryptoPP::IDEA>::Encryption> m_idea_encryptor; /**< IDEA-EAX encryptor */
	std::unique_ptr<CryptoPP::EAX<CryptoPP::IDEA>::Decryption> m_idea_decryptor; /**< IDEA-EAX decryptor */
};

NS_ENIGMA_END
#endif // !ENIGMA_IDEA_H


