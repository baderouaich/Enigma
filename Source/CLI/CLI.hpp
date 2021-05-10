#pragma once
#ifndef ENIGMA_CLI_H
#define ENIGMA_CLI_H

#include <Core/Core.hpp>
#include <Logger/Logger.hpp>

// Algorithms
#include <Algorithm/AES/AES.hpp>
#include <Algorithm/ChaCha20/ChaCha20.hpp>
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
class ENIGMA_API CLI
{
public:
	CLI(const i32& argc, const char* const* argv);
	~CLI() noexcept;

public:
	i32 Run();

private: /* Scenarios (divide and conquer) */
	void OnEncryptText(const std::unique_ptr<Algorithm>& algorithm, const String& password, const String& text, const bool save_to_database);
	void OnDecryptText(const std::unique_ptr<Algorithm>& algorithm, const String& password, const String& cipher_base64);
	void OnEncryptFile(const std::unique_ptr<Algorithm>& algorithm, const String& password, const String& in_filename, const String& out_filename_encypted, const bool save_to_database);
	void OnDecryptFile(const std::unique_ptr<Algorithm>& algorithm, const String& password, const String& in_filename_encrypted, const String& out_filename_decrypted);

	void OnListEncryptionRecords();

	void OnHelp();
	void OnVersion();
	void OnCheckForUpdates();

private:
	std::unique_ptr<cxxopts::Options> m_options;
	std::unique_ptr<cxxopts::ParseResult> m_parse_result;
};
NS_ENIGMA_END

#endif // !ENIGMA_CLI_H
