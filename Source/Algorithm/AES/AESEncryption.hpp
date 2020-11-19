#pragma once
#ifndef ENIGMA_AES_ENCRYPTION_H
#define ENIGMA_AES_ENCRYPTION_H
#include <Core/Core.hpp>
#include <Logger/Logger.hpp>

// Crypto++
#include <aes.h> // AES
#include <filters.h> // StringSink, StringSource, StreamTransformationFilter
#include <ccm.h> // CBC_Mode
#include <osrng.h> // AutoSeededRandomPool
#include <cryptlib.h> // HexEncoder, HexDecoder
#include <md5.h> // MD5
#include <osrng.h> // AutoSeededRandomPool
#include <sha.h> // SHA256
#include <hkdf.h> // KeyDerivationFunction
#include <modes.h> // 

NS_ENIGMA_BEGIN
class ENIGMA_API AESEncryption
{
public:
	AESEncryption();
	~AESEncryption();

	/*
	*	Encrypts Buffer and returns (IV + Cipher)
	*/
	String Encrypt(const String& password, const String& buffer);


private:
	/*
	*	Generates random 16 bytes IV String
	*/
	String GenerateRandomIV();
	
private:
	UniquePtr<CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption> m_aes_encryption; // Current Mode and intent
	UniquePtr<CryptoPP::AutoSeededRandomPool> m_auto_seeded_random_pool; // To generate random IV

};
NS_ENIGMA_END

#endif // !ENIGMA_AES_ENCRYPTION_H
