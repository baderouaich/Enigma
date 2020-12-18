#pragma once
#ifndef ENIGMA_CLI_H
#define ENIGMA_CLI_H

#include <Core/Core.hpp>
#include <Logger/Logger.hpp>

#include <Algorithm/AES/AES.hpp>

#include <cxxopts.hpp> // cmd parser LIB


NS_ENIGMA_BEGIN

class ENIGMA_API CLI
{
public:
	CLI(i32 argc, char** argv);
	~CLI() noexcept;

public:
	i32 Run();

public: /* Scenarios (divide and conquer) */
	void OnEncryptText(Algorithm::Type algorithm, const String& text);
	void OnDecryptText(Algorithm::Type algorithm, const String& text_encrypted);
	void OnEncryptFile(Algorithm::Type algorithm, const String& in_file, const String& out_filename_encypted);
	void OnDecryptFile(Algorithm::Type algorithm, const String& in_file_encrypted, const String& out_filename_decrypted);

private:
	std::unique_ptr<cxxopts::Options> m_options;
	std::unique_ptr<cxxopts::ParseResult> m_parse_result;
};

NS_ENIGMA_END

#endif // !ENIGMA_CLI_H
