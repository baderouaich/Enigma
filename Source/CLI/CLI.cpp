#include <pch.hpp>
#include "CLI.hpp"

NS_ENIGMA_BEGIN

CLI::CLI(i32 argc, char** argv)
{
	try
	{
		m_options = std::make_unique<cxxopts::Options>("Enigma", Constants::CLI::CLI_HELP_MESSAGE);
		m_options
			->positional_help("positional_help")
			.show_positional_help();

		m_options
			->allow_unrecognised_options()
			.add_options()
			//option, description, value, arg_help message
			//("e,encrypt", "Encrypt operation", cxxopts::value<std::string>())
			//("d,decrypt", "Decrypt operation", cxxopts::value<bool>()->default_value("false"))
			//("f,foo", "Param foo", cxxopts::value<int>()->default_value("10"))
			//("h,help", "Print usage");
			("b,bob", "Bob")
			("char", "A character", cxxopts::value<char>())
			("t,true", "True", cxxopts::value<bool>()->default_value("true"))
			("f, file", "File", cxxopts::value<std::vector<std::string>>(), "FILE")
			("i,input", "Input", cxxopts::value<std::string>())
			("o,output", "Output file", cxxopts::value<std::string>()
				->default_value("a.out")->implicit_value("b.def"), "BIN")
			("x", "A short-only option", cxxopts::value<std::string>())
			("positional",
				"Positional arguments: these are the arguments that are entered "
				"without an option", cxxopts::value<std::vector<std::string>>())
			("long-description",
				"thisisareallylongwordthattakesupthewholelineandcannotbebrokenataspace")
			("help", "Print help")
			("int", "An integer", cxxopts::value<int>(), "N")
			("float", "A floating point number", cxxopts::value<float>())
			("vector", "A list of doubles", cxxopts::value<std::vector<double>>())
			("option_that_is_too_long_for_the_help", "A very long option");

		m_parse_result = std::make_unique<cxxopts::ParseResult>(std::move(m_options->parse(argc, argv)));
	}
	catch (const std::exception& e)
	{
		ENIGMA_ERROR("Failed to parse arguments: {0}", e.what());
		exit(EXIT_FAILURE);
	}
	catch (...)
	{
		ENIGMA_ERROR("Failed to parse arguments: UNKNOWN ERROR");
		exit(EXIT_FAILURE);
	}
}


i32 CLI::Run()
{
	//LOG(ENIGMA_CURRENT_FUNCTION);

	if (m_parse_result->count("help"))
	{
		std::cout << m_options->help() << std::endl;
		std::exit(EXIT_SUCCESS);
	}
	bool debug = (*m_parse_result)["debug"].as<bool>();
	std::string bar;
	if (m_parse_result->count("bar"))
		bar =(*m_parse_result)["bar"].as<std::string>();
	int foo =(*m_parse_result)["foo"].as<int>();



	return EXIT_SUCCESS;
}

void CLI::OnEncryptText(Algorithm::Type algorithm, const String& text)
{
}

void CLI::OnDecryptText(Algorithm::Type algorithm, const String& text_encrypted)
{
}

void CLI::OnEncryptFile(Algorithm::Type algorithm, const String& in_file, const String& out_filename_encypted)
{
}

void CLI::OnDecryptFile(Algorithm::Type algorithm, const String& in_file_encrypted, const String& out_filename_decrypted)
{
}


CLI::~CLI() noexcept
{
}

NS_ENIGMA_END
