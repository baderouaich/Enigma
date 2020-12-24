#pragma once
#ifndef ENIGMA_CLI_H
#define ENIGMA_CLI_H

#include <Core/Core.hpp>
#include <Logger/Logger.hpp>

#include <Algorithm/AES/AES.hpp>
#include <Algorithm/ChaCha/ChaCha.hpp>
#include <Algorithm/TripleDES/TripleDES.hpp>

#include <cxxopts.hpp> // cmd parser library

#pragma warning(push, 0) // disable warnings
	#include <cxxopts.hpp> // cmd parser library
#pragma warning(pop)


NS_ENIGMA_BEGIN
class ENIGMA_API CLI
{
	using Intent = Algorithm::Intent;
	using Type = Algorithm::Type;

public:
	CLI(const i32& argc, char** argv);
	~CLI() noexcept;

public:
	i32 Run();

private: /* Scenarios (divide and conquer) */
	void OnEncryptText(const std::unique_ptr<Algorithm>& algorithm, const String& password, const String& text);
	void OnDecryptText(const std::unique_ptr<Algorithm>& algorithm, const String& password, const String& encrypted_text_base64);
	void OnEncryptFile(const std::unique_ptr<Algorithm>& algorithm, const String& password, const String& in_file, const String& out_filename_encypted);
	void OnDecryptFile(const std::unique_ptr<Algorithm>& algorithm, const String& password, const String& in_file_encrypted, const String& out_filename_decrypted);

private:
	std::unique_ptr<cxxopts::Options> m_options;
	std::unique_ptr<cxxopts::ParseResult> m_parse_result;
};

NS_ENIGMA_END

#endif // !ENIGMA_CLI_H
