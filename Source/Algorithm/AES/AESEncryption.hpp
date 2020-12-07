#pragma once
#ifndef ENIGMA_AES_ENCRYPTION_H
#define ENIGMA_AES_ENCRYPTION_H

#include <Core/Core.hpp>
#include <Logger/Logger.hpp>
#include <Memory/CreatePtr.hpp>

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
class ENIGMA_API AESEncryption : public CreatePtr<AESEncryption>
{
public:
	AESEncryption();
	~AESEncryption() noexcept;

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
	std::unique_ptr<CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption> m_aes_encryption; // Current Mode and intent
	std::unique_ptr<CryptoPP::AutoSeededRandomPool> m_auto_seeded_random_pool; // To generate random IV

private:
};
NS_ENIGMA_END

#endif // !ENIGMA_AES_ENCRYPTION_H
