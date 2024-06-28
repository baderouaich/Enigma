#pragma once
#ifndef ENIGMA_CLI_H
#define ENIGMA_CLI_H

#include <Core/Core.hpp>
#include <Logger/Logger.hpp>

// Algorithms
#include <Algorithm/AES/AES.hpp>
#include <Algorithm/ChaCha20Poly1305/ChaCha20Poly1305.hpp>
#include <Algorithm/TripleDES/TripleDES.hpp>
#include <Algorithm/Twofish/Twofish.hpp>
#include <Algorithm/IDEA/IDEA.hpp>
//#include <Algorithm/RSA/RSA.hpp>

/// cxxopts: CMD parser library
#pragma warning(push, 0)
#include <cxxopts.hpp>
#pragma warning(pop)
///


/*
Example encypting a text with AES-GCM:
$ ./Enigma -e --algorithm=aes --password="My@pa$$W0rd" --text="my important data securly encrypted!"
output: AaxsjxUSbOmTIjmT90WrYlqg7I7dFY7e+OqPaX20DusVnBLpkg4LW0f/fWpKES8gzFb1SrglpVt0goGfijzVHx+ULjuT

Example decrypting a text with AES-GCM:
$ ./Enigma -d --password="My@pa$$W0rd" --text="AaxsjxUSbOmTIjmT90WrYlqg7I7dFY7e+OqPaX20DusVnBLpkg4LW0f/fWpKES8gzFb1SrglpVt0goGfijzVHx+ULjuT"
output: my important data securly encrypted!

Example encypting a file with AES-GCM:
$ ./Enigma -e --algorithm=aes --password="My@pa$$W0rd" --infile="/home/user/Desktop/Data.txt" --outfile="/home/user/Desktop/Data_encrypted.txt.enigma"

Example decypting a file with AES-GCM:
$ ./Enigma -d --password="My@pa$$W0rd" --infile="/home/user/Desktop/Data_encrypted.txt.enigma" --outfile="/home/user/Desktop/Data_decrypted.txt"
*/

NS_ENIGMA_BEGIN
/** Command Line Interface */
class ENIGMA_API CLI
{
public:
	/** CLI Constructor
	*	Parses arguments in constructor's body
	*	@param argc: number of arguments from main entry point
	*	@param argv: arguments values array from main entry point
	*/
	CLI(const std::int32_t argc, const char* const* argv);
	~CLI() noexcept = default;

public:
	/** Processes arguments and invokes scenarios bellow
	*	@returns exit code
	*/
	std::int32_t Run();

private: /* Scenarios (divide and conquer) */
	/** Scenario when encrypting a text */
	void OnEncryptText(const std::unique_ptr<Algorithm>& algorithm, const std::string& password, const std::string& text, const bool save_to_database);
	/** Scenario when decrypting a text */
	void OnDecryptText(const std::unique_ptr<Algorithm>& algorithm, const std::string& password, const std::string& cipher_base64);
	/** Scenario when encrypting a file */
	void OnEncryptFile(const std::unique_ptr<Algorithm>& algorithm, const std::string& password, const std::string& in_filename, const std::string& out_filename_encypted, const bool save_to_database);
	/** Scenario when decrypting a file */
	void OnDecryptFile(const std::unique_ptr<Algorithm>& algorithm, const std::string& password, const std::string& in_filename_encrypted, const std::string& out_filename_decrypted);

	/** Scenario when --list arg processed to list saved encryptions from database */
	void OnListEncryptionRecords();

	/** Scenario when --help arg processed to display help message */
	void OnHelp();
	/** Scenario when --version arg processed to display Enigma's current version */
	void OnVersion();
	/** Scenario when --check-for-updates arg processed to check for new releases of Enigma */
	void OnCheckForUpdates();

private:
	std::unique_ptr<cxxopts::Options> m_options; /**< cxxopts command line arguments processor */
	std::unique_ptr<cxxopts::ParseResult> m_parse_result; /**< cxxopts command line arguments parser */
};
NS_ENIGMA_END

#endif // !ENIGMA_CLI_H


#pragma region todo 
#if 0
	/** Scenario when encrypting a text with RSA (the only algorithm that should be handled differently)*/
	void OnRSAEncryptText(const std::unique_ptr<Algorithm>& algorithm, const std::size_t key_size, const std::string& text)
	{
		 RSA* rsa = static_cast<RSA*>(algorithm.get());

		 // Check if key size is valid
		 ENIGMA_ASSERT_OR_THROW(
			 std::any_of(RSA::RSA_KEY_SIZES.begin(), RSA::RSA_KEY_SIZES.end(), [&key_size](const std::size_t& key) { return key_size == key; }),
			 fmt::format("Key size {} is not supported", key_size));

		 // Compression
		 std::string compressed_text = GZip::Compress(text);
		 ENIGMA_ASSERT_OR_THROW(!compressed_text.empty(), "Failed to compress text");
		
		 // Check if buffer size is valid
		 const std::size_t max_buffer_size = rsa->GetMaximumBufferSize();
		 ENIGMA_ASSERT_OR_THROW(compressed_text.size() <= max_buffer_size, fmt::format("RSA with key length {} requires maximum {} buffer length to encrypt", key_size, max_buffer_size));

		 std::string cipher, cipher_base64;
		 double elapsed_seconds{ 0.0 };

		 ENIGMA_BEGIN_TIMER(t1);
		 {
			 ENIGMA_TRACE("Encrypting Text with " + algorithm->GetTypeString() + " Algorithm Key size: " + std::to_string(key_size) +  " ...");
			 cipher = algorithm->Encrypt("", compressed_text);
			 ENIGMA_ASSERT_OR_THROW(!cipher.empty(), "Failed to encrypt text");

			 ENIGMA_TRACE("Encoding Cipher to Base64...");
			 cipher_base64 = Base64::Encode(cipher);
			 ENIGMA_ASSERT_OR_THROW(!cipher_base64.empty(), "Failed to encode cipher to base64");

			 elapsed_seconds = ENIGMA_END_TIMER(t1, double, std::milli) / 1000.0;
		 }

		 ENIGMA_LOG("Encrypted {0} in {1:0.3f} seconds. (Please save cipher base64, private and public keys bellow in a safe place)",
			 SizeUtils::FriendlySize(text.size()), elapsed_seconds);


		 std::string private_key = rsa->GetPrivateKey();
		 std::string public_key = rsa->GetPrivateKey();
		 ENIGMA_INFO(cipher_base64);
		 ENIGMA_INFO(private_key);
		 ENIGMA_INFO(public_key);


		 // Done
		 private_key.clear();
		 public_key.clear();
		 compressed_text.clear();
		 cipher.clear();
		 cipher_base64.clear();
	}
	
	/** Scenario when decrypting a text with RSA (the only algorithm that should be handled differently)*/
	void OnRSADecryptText(const std::unique_ptr<Algorithm>& algorithm, const std::string& private_key, const std::string& cipher)
	{

	}
#endif
#pragma endregion
