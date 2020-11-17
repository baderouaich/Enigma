#pragma once
#ifndef ENIGMA_AES_DECRYPTION_H
#define ENIGMA_AES_DECRYPTION_H
#include <Enigma/Core/Core.hpp>
#include <Enigma/Debug/Logger/Logger.hpp>

// Crypto++
#include <aes.h> // AES
#include <cryptlib.h> // HexEncoder, HexDecoder
#include <filters.h> // StringSink, StringSource, StreamTransformationFilter
#include <ccm.h> // CBC_Mode
#include <md5.h> // MD5
#include <osrng.h> // AutoSeededRandomPool


NS_ENIGMA_BEGIN

class ENIGMA_API AESDecryption
{
public:
	AESDecryption();
	~AESDecryption();

	/*
	*	Decrypts base64 Cipher buffer
	*/
	String Decrypt(const String& base64_cipher);


public: /* Accessors */
	//Get auto extracted IV
	byte* const& GetIV() const noexcept { return m_iv; }
	//Get auto extracted Key
	byte* const& GetKey() const noexcept { return m_key; }

private:
	byte* m_key; //auto extracted Encryption password from base64 cipher
	byte* m_iv; //auto extracted 16bytes iv from base64 cipher

private:
	UniquePtr<CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption> m_aes_decryption;
};
NS_ENIGMA_END

#endif // !ENIGMA_AES_DECRYPTION_H
