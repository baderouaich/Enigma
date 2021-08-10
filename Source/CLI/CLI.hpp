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

#include <cxxopts.hpp> // CMD parser library

/*
Example encypting a text with AES-GCM:
$ ./Enigma -e --mode=aes --password="My@pa$$W0rd" --text="my important data securly encrypted!"
output: AaxsjxUSbOmTIjmT90WrYlqg7I7dFY7e+OqPaX20DusVnBLpkg4LW0f/fWpKES8gzFb1SrglpVt0goGfijzVHx+ULjuT

Example decrypting a text with AES-GCM:
$ ./Enigma -d --password="My@pa$$W0rd" --text="AaxsjxUSbOmTIjmT90WrYlqg7I7dFY7e+OqPaX20DusVnBLpkg4LW0f/fWpKES8gzFb1SrglpVt0goGfijzVHx+ULjuT"
output: my important data securly encrypted!

Example encypting a file with AES-GCM:
$ ./Enigma -e --mode=aes --password="My@pa$$W0rd" --infile="/home/bader/Desktop/Data" --outfile="/home/bader/Desktop/Data-encrypted.enigma" --compress

Example decypting a file with AES-GCM:
$ ./Enigma -d --password="My@pa$$W0rd" --infile="/home/bader/Desktop/Data-encrypted.enigma" --outfile="/home/bader/Desktop/Data-decrypted" --decompress
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
	CLI(const i32 argc, const char* const* argv);
	~CLI() noexcept = default;

public:
	/** Processes arguments and invokes scenarios bellow
	*	@returns exit code
	*/
	i32 Run();

private: /* Scenarios (divide and conquer) */
	/** Scenario when encrypting a text */
	void OnEncryptText(const std::unique_ptr<Algorithm>& algorithm, const String& password, const String& text, const bool save_to_database);
	/** Scenario when decrypting a text */
	void OnDecryptText(const std::unique_ptr<Algorithm>& algorithm, const String& password, const String& cipher_base64);
	/** Scenario when encrypting a file */
	void OnEncryptFile(const std::unique_ptr<Algorithm>& algorithm, const String& password, const String& in_filename, const String& out_filename_encypted, const bool save_to_database);
	/** Scenario when decrypting a file */
	void OnDecryptFile(const std::unique_ptr<Algorithm>& algorithm, const String& password, const String& in_filename_encrypted, const String& out_filename_decrypted);

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
