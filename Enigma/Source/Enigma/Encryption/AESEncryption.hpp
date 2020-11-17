#pragma once
#ifndef ENIGMA_AES_ENCRYPTION_H
#define ENIGMA_AES_ENCRYPTION_H
#include <Enigma/Core/Core.hpp>
#include <Enigma/Debug/Logger/Logger.hpp>

// Crypto++
#include <aes.h> // AES
#include <filters.h> // StringSink, StringSource, StreamTransformationFilter
#include <ccm.h> // CBC_Mode
#include <osrng.h> // AutoSeededRandomPool


NS_ENIGMA_BEGIN

class ENIGMA_API AESEncryption
{
public:
	AESEncryption();
	~AESEncryption();

	/*
	*	Encrypts Buffer and returns Cipher as Base64 in format (IV + KEY + CIPHER), iv and key length are in namespace Constants::Encryption::AES
	*/
	String Encrypt(const String& buffer);


public: /* Accessors */
	//Get auto generated IV
	byte* const& GetIV() const noexcept { return m_iv; }
	//Get auto generated Key
	byte* const& GetKey() const noexcept { return m_key; }

private:
	byte* m_key; //auto generated Encryption password length from Security level 64 - bit 128 - bit 256 - bit 512 - bit 1024 - bit 2048 - bit 4096 - bit
	byte* m_iv; //auto generated 16bytes iv
	

private:
	UniquePtr<CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption> m_aes_encryption;
	UniquePtr<CryptoPP::AutoSeededRandomPool> m_auto_seeded_random_pool;

};
NS_ENIGMA_END

#endif // !ENIGMA_AES_ENCRYPTION_H
