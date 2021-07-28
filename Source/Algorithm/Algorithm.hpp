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
//#include <modes.h> // Paddings CBC, EBC...
#include <aes.h> // AES
#include <gcm.h> // GCM Authentication Mode
#include <eax.h> // EAX Authentication Mode
//#include <chacha.h> // ChaCha/Salsa20
#include <chachapoly.h> // ChaCha/Salsa20 Authenticated
#include <des.h> // TripleDES
#include <twofish.h> // Twofish
#include <idea.h> // IDEA
#include <blowfish.h> // Blowfish
#pragma warning(pop)
static_assert(sizeof(Enigma::byte) == sizeof(CryptoPP::byte), "Enigma byte size must be the same size with Crypto++'s byte");


NS_ENIGMA_BEGIN

class AES;
class ChaCha20;
class TripleDES;
class Twofish;
class IDEA;
class Blowfish;

/*
*	Algorithm abstract class
*/
class ENIGMA_API Algorithm
{
public:
	/*
	*	Intention of creating an instance of Algorithm.
	*	to not initialize resources not needed for the operation. 
	*/
	enum class Intent : byte 
	{
		Encrypt = 0x1,
		Decrypt
	};

	/*
	*	Algorithm type, AES, ChaCha...
	*/
	enum class Type : byte
	{
		AES		= 0x01,			// AES-GCM
		Twofish,				// Twofish-GCM
		TripleDES,				// TripleDES-EAX
		Blowfish,				// Blowfish-EAX
		IDEA,					// IDEA-EAX
		ChaCha20Poly1305,		// ChaCha20Poly1305

		BEGIN	= AES,
		END		= ChaCha20Poly1305
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
	* @param cipher: can contain more than cipher part, like IV, algorithm used for encryption, MAC and so on, depending on the algorithm implementation.
	* @return Recovered Buffer
	* @exception throws CryptoPP::Exception, std::exception on failure
	*/
	virtual String Decrypt(const String& password, const String& cipher) = 0;

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
	*	Returns a string of supported algorithms represented as "[Algo1, Algo2, Algo3...]" orderd as enum Type
	*/
	static String GetSupportedAlgorithmsStr() noexcept;

	/*
	*	Returns a vector of supported algorithms represented as pair of algo name string, algo type. orderd as enum Type
	*/
	static std::vector<std::pair<String, Algorithm::Type>> GetSupportedAlgorithms() noexcept;

protected:
	Type m_type; // Algorithm type: AES, ChaCha, TripleDES...
	Intent m_intent; // Operation, Encrypt or Decrypt
	std::unique_ptr<CryptoPP::AutoSeededRandomPool> m_auto_seeded_random_pool; // To generate random IV

};


NS_ENIGMA_END



#endif // !ENIGMA_ALGORITHM_H
