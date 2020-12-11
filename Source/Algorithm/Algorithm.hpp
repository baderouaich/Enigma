#pragma once
#ifndef ENIGMA_ALGORITHM_H
#define ENIGMA_ALGORITHM_H
#include <Core/Core.hpp>
#include <Logger/Logger.hpp>
#include <Utility/CryptoPPUtils.hpp>

// Crypto++
#include <cryptlib.h> // HexEncoder, HexDecoder
#include <filters.h> // StringSink, StringSource, StreamTransformationFilter
#include <ccm.h> // CBC_Mode
#include <osrng.h> // AutoSeededRandomPool
#include <sha.h> // SHA256
#include <hkdf.h> // KeyDerivationFunction
#include <modes.h> // Classes for block cipher modes of operation

#include <aes.h> // AES
#include <chacha.h> // ChaCha/Salsa20
#include <rsa.h> // RSA

NS_ENIGMA_BEGIN

/*
*	Algorithm abstract class
*/
class ENIGMA_API Algorithm
{
public:
	enum class Intent : ui8 { Encrypt, Decrypt };

public:
	Algorithm(Intent intent) noexcept 
		:
		m_intent(intent),
		m_auto_seeded_random_pool(intent == Intent::Encrypt ? std::make_unique<CryptoPP::AutoSeededRandomPool>() : nullptr)
	{}

	virtual String Encrypt(const String& password, const String& buffer) = 0;
	virtual String Decrypt(const String& password, const String& buffer) = 0;

protected:
	/*
	*	Generates random 16 bytes IV String
	*/
	String GenerateRandomIV() 
	{
		//if (!m_auto_seeded_random_pool)
		//	m_auto_seeded_random_pool = std::make_unique<CryptoPP::AutoSeededRandomPool>();
		String iv(CryptoPP::AES::BLOCKSIZE, '\000');
		m_auto_seeded_random_pool->GenerateBlock(reinterpret_cast<byte*>(iv.data()), iv.size());
		return iv;
	}

protected:
	Intent m_intent; // Operation, Encrypt or Decrypt
	std::unique_ptr<CryptoPP::AutoSeededRandomPool> m_auto_seeded_random_pool; // To generate random IV

};

NS_ENIGMA_END



#endif // !ENIGMA_ALGORITHM_H
