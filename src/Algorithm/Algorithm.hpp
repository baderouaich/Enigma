#pragma once
#ifndef ENIGMA_ALGORITHM_H
#define ENIGMA_ALGORITHM_H

#include <Core/Core.hpp>
#include <Logger/Logger.hpp>

// Crypto++
#pragma warning(push, 0) // This ignores all warnings raised inside External headers
#include <Utility/CryptoPPUtils.hpp>
#include <cryptlib.h> // CryptoPP
#include <filters.h>  // StringSink, StringSource, StreamTransformationFilter
#include <hex.h>      // HexEncoder, HexDecoder
#include <hkdf.h>     // HKDF
#include <osrng.h>    // AutoSeededRandomPool
#include <sha.h>      // SHA256
//#include <modes.h> // Paddings CBC, EBC...
#include <aes.h> // AES
#include <eax.h> // EAX Authentication Mode
#include <gcm.h> // GCM Authentication Mode
//#include <chacha.h> // ChaCha/Salsa20
#include <blowfish.h>   // Blowfish
#include <chachapoly.h> // ChaCha20Poly1305 Authenticated
#include <des.h>        // TripleDES
#include <idea.h>       // IDEA
#include <pssr.h>       // RSA Signer & Verifier
#include <rsa.h>        // RSA
#include <twofish.h>    // Twofish
#pragma warning(pop)
static_assert(sizeof(Enigma::byte) == sizeof(CryptoPP::byte), "Enigma byte size must be the same size with Crypto++'s byte");


NS_ENIGMA_BEGIN

/**
*	Forward Declarations
*/
class AES;
class Twofish;
class TripleDES;
class Blowfish;
class IDEA;
class ChaCha20Poly1305;
//class RSA;

/**
*	@brief Algorithm abstract class
*	@details All encryption algorithms like AES, Twofish.. inherit from this class
*	@todo add more encryption algorithms
*/
class Algorithm {
  public:
    /**
	*	Intention of creating an instance of an Algorithm.
	* 
	*	helps to avoid initializing resources not needed for an specific operation.
	*	(for example when encrypting, we initialize a random seeder which is not needed
	*   when decrypting)
	*/
    enum class Intent : Enigma::byte {
      None = 0 << 0,
      Encrypt = 1 << 1,
      Decrypt = 1 << 2,
      All = Encrypt | Decrypt
    };
    ENIGMA_ENUM_CLASS_BITWISE_OPERATORS(Intent, Enigma::byte);


    /**
    *	Algorithm type: AES, ChaCha...
    */
    enum class Type : byte {
      AES = 0x01,       // AES-GCM
      Twofish,          // Twofish-GCM
      TripleDES,        // TripleDES-EAX
      Blowfish,         // Blowfish-EAX
      IDEA,             // IDEA-EAX
      ChaCha20Poly1305, // ChaCha20Poly1305
      //RSA,							// RSA N/A yet

      ENIGMA_ENUM_DECLARE_BEGIN_END(AES)
    };


  public:
    explicit Algorithm(const Type type, const Intent intent = Intent::Encrypt | Intent::Decrypt) noexcept;
    virtual ~Algorithm() noexcept;

  public:
    /**
	*	Encrypts buffer with password
	*	@param password: Encryption password
	*	@param buffer: Buffer to encrypt (text, binary...)
	*	@return (Algo type enum id + IV + Cipher)
	*	@exception throws CryptoPP::Exception, std::exception on failure
	*/
    virtual std::string Encrypt(const std::string& password, const std::string& buffer) = 0;

    /**
	*	Decrypts cipher with password
	*	@param password: Password used to Encyrpt buffer
	*	@param cipher: can contain more than cipher part, like IV, algorithm used for encryption, MAC and so on, depending on the algorithm implementation.
	*	@return Recovered Buffer
	*	@exception throws CryptoPP::Exception, std::exception on failure
	*/
    virtual std::string Decrypt(const std::string& password, const std::string& cipher) = 0;

  public: /* Create polymorphic algorithm by either mode name or type*/
    /**
	*	Creates polymorphic algorithm from algorithm type name
	*	@see CreateFromType()
	*/
    static std::unique_ptr<Algorithm> CreateFromName(const std::string& name, const Intent intent);
    /**
	*	Creates polymorphic algorithm from algorithm type
	*/
    static std::unique_ptr<Algorithm> CreateFromType(const Type type, const Intent intent);

  public:
    Type GetType() const noexcept { return m_type; }
    void SetType(const Type type) noexcept { this->m_type = type; }
    std::string GetTypeString() const noexcept { return AlgoTypeEnumToStr(m_type); }

  protected:
    /**
	*	Generates random IV (aka Salt) with a desired length
	*/
    static std::string GenerateRandomIV(const std::size_t size);

  public:
    /**
	*	Auto detect algorithm used for encryption from cipher text
	*/
    static Type DetectFromCipher(const std::string& cipher);

    /**
	*	Auto detect algorithm used for encryption from cipher base64 text
	*/
    static Type DetectFromCipherBase64(const std::string& cipher_base64);

    /**
	*	Auto detect algorithm used for encryption from encrypted file
	*/
    static Type DetectFromFile(const std::string& filename);

    /**
	*	Converts Algorithm::Type to std::string
	*/
    static std::string AlgoTypeEnumToStr(const Algorithm::Type e) noexcept;

    /**
	*	Returns a string of supported algorithms represented as "[Algo1, Algo2, Algo3...]" orderd as enum Type
	*/
    static std::string GetSupportedAlgorithmsStr() noexcept;

    /**
	*	Returns a vector of supported algorithms represented as pair of algo name string, algo type. orderd as enum Type
	*/
    static std::vector<std::pair<std::string, Algorithm::Type>> GetSupportedAlgorithms() noexcept;

  protected:
    Type m_type{};                                                                                    /**< Algorithm type: AES, ChaCha, TripleDES... */
    Intent m_intent{};                                                                                /**< Operation, Encrypt or Decrypt */
    inline static std::unique_ptr<CryptoPP::AutoSeededRandomPool> m_auto_seeded_random_pool{nullptr}; /**< To generate random IV on encryption */
};

NS_ENIGMA_END

#endif // !ENIGMA_ALGORITHM_H
