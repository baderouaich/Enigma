#pragma once
#ifndef ENIGMA_ALGORITHM_H
#define ENIGMA_ALGORITHM_H

#include <Core/Core.hpp>
#include <Logger/Logger.hpp>

// Crypto++
#pragma warning(push, 0) // This ignores all warnings raised inside External headers
#include <Utility/CryptoPPUtils.hpp>
#include <cryptlib.h> // HexEncoder, HexDecoder
#include <filters.h> // StringSink, StringSource, StreamTransformationFilter
#include <osrng.h> // AutoSeededRandomPool
#include <hkdf.h> // HKDF
#include <sha.h> // SHA256
#include <modes.h> // Paddings CBC, EBC...

#include <aes.h> // AES
#include <gcm.h> // GCM Mode
#include <chacha.h> // ChaCha/Salsa20
#include <des.h> // TripleDES
#include <twofish.h> // Twofish
#include <idea.h> // IDEA
#pragma warning(pop)
static_assert(sizeof(Enigma::byte) == sizeof(CryptoPP::byte), "Enigma byte size must be the same size with Crypto++'s byte");


NS_ENIGMA_BEGIN

class AES;
class ChaCha20;
class TripleDES;
class Twofish;
class IDEA;

/*
*	Algorithm abstract class
*/
class ENIGMA_API Algorithm
{
public:
	enum class Intent : byte 
	{
		Encrypt = 0x0,
		Decrypt = 0x1
	};
	enum class Type : byte
	{
		AES			= 0x01,
		ChaCha20	= 0x02,
		TripleDES	= 0x03,
		Twofish		= 0x04,
		IDEA		= 0x05,

		First = AES,
		Last  = IDEA
	};

public:
	explicit Algorithm(Type type, Intent intent) noexcept;
	virtual ~Algorithm() noexcept;

public:
	/*
	*	Encrypts buffer with password
	* @param password: Encryption password
	* @param buffer: Buffer to encrypt (text, binary...)
	* @return (Algo type enum id + IV + Cipher)
	* @exception throws CryptoPP::Exception, std::exception on failure
	*/
	virtual String Encrypt(const String& password, const String& buffer) = 0;
	/*
	*	Decrypts cipher with password
	* @param password: Password used to Encyrpt buffer
	* @param iv_cipher: (Algo type enum id + IV + Cipher)
	* @return Recovered Buffer
	* @exception throws CryptoPP::Exception, std::exception on failure
	*/
	virtual String Decrypt(const String& password, const String& iv_cipher) = 0;


public: /* Create polymorphic algorithm by either mode name or type*/
	static std::unique_ptr<Algorithm> CreateFromName(const String& name, const Intent intent);
	static std::unique_ptr<Algorithm> CreateFromType(const Type type, const Intent intent);

public:
	Type GetType() const noexcept { return m_type; }
	void SetType(const Type type) noexcept { this->m_type = type; }
	String GetTypeString() const noexcept { return AlgoTypeEnumToStr(m_type); }

protected:
	/*
	*	Generates random IV with specified length
	*/
	String GenerateRandomIV(const size_t size);

public:
	/*
	*	Auto detect algorithm used for encryption from cipher text
	*/
	static Type DetectFromCipher(const String& cipher);

	/*
	*	Auto detect algorithm used for encryption from cipher base64 text
	*/
	static Type DetectFromCipherBase64(const String& cipher_base64);

	/*
	*	Auto detect algorithm used for encryption from encrypted file
	*/
	static Type DetectFromFile(const String& filename);

	/*
	*	Converts Algorithm::Type to String
	*/
	static String AlgoTypeEnumToStr(const Algorithm::Type e) noexcept;

	/*
	*	Returns a string of supported algorithms represented as "[Algo1, Algo2, Algo3...]"
	*/
	static String GetSupportedAlgorithmsStr() noexcept;

	/*
	*	Returns a vector of supported algorithms represented as pair of algo name string, algo type
	*/
	static std::vector<std::pair<String, Algorithm::Type>> GetSupportedAlgorithms() noexcept;

protected:
	Type m_type; // Algorithm type: AES, ChaCha, TripleDES...
	Intent m_intent; // Operation, Encrypt or Decrypt
	std::unique_ptr<CryptoPP::AutoSeededRandomPool> m_auto_seeded_random_pool; // To generate random IV

};


NS_ENIGMA_END



#endif // !ENIGMA_ALGORITHM_H



















#if 0
#pragma once
#ifndef ENIGMA_ALGORITHM_H
#define ENIGMA_ALGORITHM_H
#include <Core/Core.hpp>
#include <Logger/Logger.hpp>
#include <Utility/CryptoPPUtils.hpp>

// Crypto++
#pragma warning(push, 0) // This ignores all warnings raised inside External headers
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
#include <des.h> // TripleDES
#include <twofish.h> // Twofish
#include <idea.h> // IDEA
#include <rsa.h> // RSA
#include <pssr.h> // RSA PSSR
#pragma warning(pop)
static_assert(sizeof(Enigma::byte) == sizeof(CryptoPP::byte), "Enigma byte size must be the same size with Crypto++'s byte");


NS_ENIGMA_BEGIN

class AES;
class ChaCha20;
class TripleDES;
class Twofish;
class IDEA;
/*
*	Algorithm abstract class
*/
class ENIGMA_API Algorithm
{
public:
	enum class Intent : byte { Encrypt, Decrypt };
	enum class Type : byte
	{
		AES			= 0x01,
		ChaCha20	= 0x02,
		TripleDES	= 0x03,
		Twofish		= 0x04,
		IDEA		= 0x05,

		First = AES,
		Last = IDEA
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
	/*
	*	Encrypts buffer with password
	* @param password: Encryption password
	* @param buffer: Buffer to encrypt (text, binary...)
	* @return (Algo type enum id + IV + Cipher)
	* @exception throws CryptoPP::Exception, std::exception on failure
	*/
	virtual String Encrypt(const String& password, const String& buffer) noexcept(false) = 0;
	/*
	*	Decrypts cipher with password
	* @param password: Password used to Encyrpt buffer
	* @param iv_cipher: (Algo type enum id + IV + Cipher)
	* @return Recovered Buffer
	* @exception throws CryptoPP::Exception, std::exception on failure
	*/
	virtual String Decrypt(const String& password, const String& iv_cipher) noexcept(false) = 0;


public: /* Create polymorphic algorithm by either mode name or type*/
	template<class A = Algorithm>
	static std::unique_ptr<A> CreateFromName(const String& mode, const Intent& intent)
	{
		const auto ModeIn = [&mode](const std::vector<std::string_view>& v) -> const bool
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
		else if (ModeIn({ "idea", "idea-cbc" }))
			return std::make_unique<IDEA>(intent);
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
	const Type& GetType() const noexcept { return m_type; }
	void SetType(const Type& type) noexcept { this->m_type = type; }
	String GetTypeString() const noexcept { return AlgoTypeEnumToStr(m_type); }

protected:
	String GenerateRandomIV(const size_t& size) 
	{
		String iv(size, '\000');
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
			CASE_ENUM(IDEA);
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
		std::vector<std::pair<String, Algorithm::Type>> out(static_cast<size_t>(Algorithm::Type::Last));
		for (ui8 i = static_cast<ui8>(Algorithm::Type::First); i <= static_cast<ui8>(Algorithm::Type::Last); i++)
		{
			out[static_cast<size_t>(i) - 1] = std::make_pair(AlgoTypeEnumToStr(static_cast<Algorithm::Type>(i)) , static_cast<Algorithm::Type>(i));
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
#endif
