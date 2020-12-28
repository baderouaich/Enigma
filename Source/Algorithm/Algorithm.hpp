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
#include <twofish.h> // Twofish
#pragma warning(pop)

NS_ENIGMA_BEGIN

class AES;
class ChaCha20;
class TripleDES;
class Twofish;
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
		Twofish,

		First = AES,
		Last = Twofish
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

public: /* Create polymorphic algorithm by either mode name or type*/
	template<class A = Algorithm>
	static std::unique_ptr<A> CreateFromName(const String& mode, const Intent& intent)
	{
		const auto ModeIn = [&mode](const std::vector<std::string_view>& v) -> bool
		{
			return std::find(v.begin(), v.end(), mode) != v.end();
		};

		if (ModeIn({ "aes", "aes-gcm" }))
			return std::make_unique<AES>(intent);
		else if (ModeIn({ "chacha", "chacha20", "salsa", "salsa20" }))
			return std::make_unique<ChaCha20>(intent);
		else if (ModeIn({ "tripledes", "triple-des", "tripledes-cbc" }))
			return std::make_unique<TripleDES>(intent);
		else if (ModeIn({ "twofish", "twofish-gcm" }))
			return std::make_unique<Twofish>(intent);
		//else if (mode == "idea")
		//	return std::make_unique<Enigma::IDEA>(intent);
		else
			throw std::runtime_error("Unsupported algorithm mode: " + mode);
	}
	static std::unique_ptr<Algorithm> CreateFromType(const Type& type, const Intent& intent)
	{
		String mode = AlgoTypeEnumToStr(type);
		StringUtils::Lower(mode);
		return CreateFromName(mode, intent);
	}

public:
	virtual String Encrypt(const String& password, const String& buffer) = 0;
	virtual String Decrypt(const String& password, const String& buffer) = 0;

public:
	const Type& GetType() const noexcept { return m_type; }
	void SetType(const Type& type) noexcept { this->m_type = type; }
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
			CASE_ENUM(Twofish);
			//CASE_ENUM(IDEA);
			default: return "Unknown";
		}
#undef CASE_ENUM
	}
	static String GetSupportedAlgorithmsStr() noexcept
	{
		String out = "[";
		for (ui8 i = static_cast<ui8>(Algorithm::Type::First); i <= static_cast<ui8>(Algorithm::Type::Last); i++)
		{
			out += AlgoTypeEnumToStr(static_cast<Algorithm::Type>(i)) + (i != ui8(Algorithm::Type::Last) ? ", " : "");
		}
		out += ']';
		return out;
	}
	static std::vector<std::pair<String, Algorithm::Type>> GetSupportedAlgorithms() noexcept
	{
		std::vector<std::pair<String, Algorithm::Type>> out(static_cast<size_t>(Algorithm::Type::Last) + 1);
		for (ui8 i = static_cast<ui8>(Algorithm::Type::First); i <= static_cast<ui8>(Algorithm::Type::Last); i++)
		{
			out[static_cast<size_t>(i)] = std::make_pair(AlgoTypeEnumToStr(static_cast<Algorithm::Type>(i)) , static_cast<Algorithm::Type>(i));
		}
		return out;
	}
protected:
	Type m_type; // Algorithm type: AES, ChaCha, TripleDES...
	Intent m_intent; // Operation, Encrypt or Decrypt
	std::unique_ptr<CryptoPP::AutoSeededRandomPool> m_auto_seeded_random_pool; // To generate random IV

};

NS_ENIGMA_END



#endif // !ENIGMA_ALGORITHM_H
