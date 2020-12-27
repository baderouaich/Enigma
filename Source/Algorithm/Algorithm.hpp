#pragma once
#ifndef ENIGMA_ALGORITHM_H
#define ENIGMA_ALGORITHM_H
#include <Core/Core.hpp>
#include <Logger/Logger.hpp>
#include <Utility/CryptoPPUtils.hpp>

// Crypto++
// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <cryptlib.h> // HexEncoder, HexDecoder
#include <filters.h> // StringSink, StringSource, StreamTransformationFilter
#include <ccm.h> // CBC_Mode
#include <osrng.h> // AutoSeededRandomPool
#include <sha.h> // SHA256
#include <hkdf.h> // KeyDerivationFunction
#include <modes.h> // Classes for block cipher modes of operation

#include <aes.h> // AES
#include <gcm.h> // GCM Mode
#include <chacha.h> // ChaCha/Salsa20
#include <rsa.h> // RSA
#include <des.h> // TripleDES
#pragma warning(pop)

NS_ENIGMA_BEGIN

/*
*	Algorithm abstract class
*/
class ENIGMA_API Algorithm
{
public:
	enum class Intent : ui8 { Encrypt, Decrypt };
	enum class Type : ui8
	{
		AES = 0,
		ChaCha20,
		TripleDES,

		First = AES,
		Last = TripleDES
	};
public:
	explicit Algorithm(Type type, Intent intent) noexcept 
		:
		m_type(type),
		m_intent(intent),
		// we only generate random iv when encrypting
		m_auto_seeded_random_pool(intent == Intent::Encrypt ? std::make_unique<CryptoPP::AutoSeededRandomPool>() : nullptr) 
	{}
	virtual ~Algorithm() noexcept {}

public:
	virtual String Encrypt(const String& password, const String& buffer) = 0;
	virtual String Decrypt(const String& password, const String& buffer) = 0;

public:
	const Type& GetType() const noexcept { return m_type; }
	String GetTypeString() const noexcept { return AlgoTypeEnumToStr(m_type); }

protected:
	/*
	*	Generates random 16 bytes IV String
	*/
	String GenerateRandomIV(const size_t& iv_size) 
	{
		String iv(iv_size, '\000');
		m_auto_seeded_random_pool->GenerateBlock(reinterpret_cast<byte*>(iv.data()), iv.size());
		return iv;
	}

public:
	static String AlgoTypeEnumToStr(const Algorithm::Type& e) noexcept
	{
#define CASE_ENUM(e) case Algorithm::Type::e: return #e
		switch (e)
		{
			CASE_ENUM(AES);
			CASE_ENUM(ChaCha20);
			CASE_ENUM(TripleDES);
			//CASE_ENUM(Twofish);
			//CASE_ENUM(IDEA);
			default: return "Unknown";
		}
#undef CASE_ENUM
	}
	static String GetSupportedAlgorithms() noexcept
	{
		String out = "[";
		for (ui8 i = static_cast<ui8>(Algorithm::Type::First); i <= static_cast<ui8>(Algorithm::Type::Last); i++)
		{
			out += AlgoTypeEnumToStr(static_cast<Algorithm::Type>(i)) + (i != ui8(Algorithm::Type::Last) ? ", " : "");
		}
		out += ']';
		return out;
	}

protected:
	Type m_type; // Algorithm type: AES, ChaCha, TripleDES...
	Intent m_intent; // Operation, Encrypt or Decrypt
	std::unique_ptr<CryptoPP::AutoSeededRandomPool> m_auto_seeded_random_pool; // To generate random IV

};

NS_ENIGMA_END



#endif // !ENIGMA_ALGORITHM_H
