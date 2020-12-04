#pragma once
#ifndef ENIGMA_AES_DECRYPTION_H
#define ENIGMA_AES_DECRYPTION_H
#include <Core/Core.hpp>
#include <Logger/Logger.hpp>

// Crypto++
#include <cryptlib.h> // HexEncoder, HexDecoder
#include <aes.h> // AES
#include <filters.h> // StringSink, StringSource, StreamTransformationFilter
#include <ccm.h> // CBC_Mode
#include <osrng.h> // AutoSeededRandomPool
#include <sha.h> // SHA256
#include <hkdf.h> // KeyDerivationFunction
#include <modes.h> // Classes for block cipher modes of operation


NS_ENIGMA_BEGIN

class ENIGMA_API AESDecryption
{
public:
	AESDecryption();
	~AESDecryption() noexcept;

	/*
	*	Decrypt encrypted (IV, CIPHER)
	*/
	String Decrypt(const String& password, const String& buffer);

private:
	UniquePtr<CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption> m_aes_decryption;
};
NS_ENIGMA_END

#endif // !ENIGMA_AES_DECRYPTION_H
