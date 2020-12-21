#include <pch.hpp>
#include "CLI.hpp"

NS_ENIGMA_BEGIN

//Example command to encrypt text with aes
//$ enigma -encrypt --mode=aes --text="hello world!" 
//$ enigma -encrypt --mode=aes --infile="C:/Users/bader/Desktop/file.txt" --outfile="C:/Users/bader/Desktop/encrypted.txt"

CLI::CLI(i32 argc, char** argv)
{
	try
	{
		m_options = std::make_unique<cxxopts::Options>("Enigma", Constants::CLI::CLI_HELP_MESSAGE);
		//m_options
		//	->positional_help("positional_help")
		//	.show_positional_help();
		
		m_options
			->allow_unrecognised_options()
			.add_options()
			//option, description, value, arg_help message
			("e,encrypt", "Encrypt operation") // -e | --encrypt
			("d,decrypt", "Decrypt operation") // -d | --decrypt
			("m,mode", "Encryption/Decryption mode", cxxopts::value<std::string>()->default_value("aes"), "[aes, chacha, tripledes]") // -m aes | --mode=aes
			("p,password", "Encryption/Decryption password", cxxopts::value<std::string>()) // -p "mypass" | --password="mypass"
			("t,text", "Text to encrypt", cxxopts::value<std::string>()) // -t "lorem" | --text="lorem"
			("i,infile", "File name to encrypt", cxxopts::value<std::string>()->default_value(".")) // -i "C:/file" | --infile="C:/file"
			("o,oufile", "Output encrypted file name", cxxopts::value<std::string>()->default_value(".")) // -o "C:/file" | --outfile="C:/file"
			("h,help", "Shows help message"); // HELP
			
		m_parse_result = std::make_unique<cxxopts::ParseResult>(std::move(m_options->parse(argc, argv)));

		// get the unmatched arguments
		const auto& unmatched_args = m_parse_result->unmatched();
		for (const auto& arg : unmatched_args)
		{
			ENIGMA_TRACE("Unknown argument {0}", arg.c_str());
		}
		if (!unmatched_args.empty())
			ENIGMA_INFO(m_options->help());


	}
	catch (const std::exception& e)
	{
		ENIGMA_ERROR("Failed to parse arguments: {0}", e.what());
		std::exit(EXIT_FAILURE);
	}
	catch (...)
	{
		ENIGMA_ERROR("Failed to parse arguments: UNKNOWN ERROR");
		std::exit(EXIT_FAILURE);
	}
}


i32 CLI::Run()
{
	const auto& r = *m_parse_result;

	// Handle --help & -h options
	if (r.count("h") || r.count("help"))
	{
		ENIGMA_INFO(m_options->help());
		return EXIT_SUCCESS;
	}

	Intent intent{};
	Type mode{};
	std::unique_ptr<Algorithm> algorithm{}; // polymorphic algorithm
	String password{}, text{}, infilename{}, outfilename{};
	try
	{
		// Encrypting or Decrypting ?
		if (r.count("e") || r.count("encrypt")) // -e or --encrypt
		{
			LOG("Encrypting ");
			intent = Algorithm::Intent::Encrypt;
		}
		else if (r.count("d") || r.count("decrypt")) // -d or --decrypt
		{
			LOG("Decrypting ");
			intent = Algorithm::Intent::Decrypt;
		}
		else
			throw std::runtime_error("You should specify whether you want to encrypt -e or decrypt -d, or say -help");
		
		// White algorithm mode are we using ?
		if (r.count("m") || r.count("mode")) // --m=aes or --mode=aes or chacha..
		{
			String m = r["m"].as<String>();
			StringUtils::Lower(m);
			if (m == "aes") mode = Algorithm::Type::AES;
			else if (m == "chacha") mode = Algorithm::Type::ChaCha;
			else if (m == "tripledes") mode = Algorithm::Type::TripleDES;
			else throw std::runtime_error("Unsupported algorithm mode: " + m);

			LOG("Mode: {0}", m);
		}
		else
			throw std::runtime_error("You should specify an encryption/decryption mode like: --mode=aes or -m aes");

		// What is the encryption/decryption password?
		if (r.count("p") || r.count("password")) // -p "mypass" | --password="mypass"
		{
			password = r["p"].as<String>();

			LOG("Password: {0}", password);
		}
		else
			throw std::runtime_error("You should specify an encryption/decryption password like: --password=mypass or -p mypass");


		// Is there a text to encrypt/decrypt?
		if (r.count("t") || r.count("text")) // -t "lorem" | --text="lorem"
		{
			text = r["t"].as<String>();

			LOG("Text: {0}", text);
		}
		
		// Or Is there a file to encrypt/decrypt?
		if (r.count("i") || r.count("infile")) // -i "C:/file" | --infile="C:/file_to_encrypt.txt"
		{
			infilename = r["i"].as<String>();

			LOG("In File name: {0}", infilename);
		}
		if (r.count("o") || r.count("outfile")) // -o "C:/file" | --outfile="C:/file"
		{
			outfilename = r["o"].as<String>();

			LOG("Out File name: {0}", outfilename);
		}


		// Call Scenarios //
		// Create polymorphic Algorithm type
		switch (mode)
		{
		case Type::ChaCha:
			algorithm.reset(new Enigma::ChaCha(intent));
			break;
		case Type::TripleDES:
			algorithm.reset(new Enigma::TripleDES(intent));
			break;
		default:
		case Type::AES:
			algorithm.reset(new Enigma::AES(intent));
			break;
		}
		// Check wether its a text or file encryption/decryption
		if (!text.empty())
		{
			// Check intention
			switch (intent)
			{
			case Intent::Encrypt:
				this->OnEncryptText(algorithm, password, text);
				break;
			case Intent::Decrypt:
				this->OnDecryptText(algorithm, password, text);
				break;
			}
		}
		else if (!infilename.empty() && !outfilename.empty())
		{
			// Check intention
			switch (intent)
			{
			case Intent::Encrypt:
				this->OnEncryptFile(algorithm, password, infilename, outfilename);
				break;
			case Intent::Decrypt:
				this->OnDecryptFile(algorithm, password, infilename, outfilename);
				break;
			}
		}
		else
			throw std::runtime_error("in file name and out file name should not be empty");

	}
	catch (const std::exception& e) 
	{
		ENIGMA_ERROR("Failed to parse arguments: {0}", e.what());
		return EXIT_FAILURE;
	}
	catch (...)
	{
		ENIGMA_ERROR("Failed to parse arguments: UNKNOWN ERROR");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}


void CLI::OnEncryptText(const std::unique_ptr<Algorithm>& algorithm, const String& password, const String& text)
{
	LOG(ENIGMA_CURRENT_FUNCTION);
}

void CLI::OnDecryptText(const std::unique_ptr<Algorithm>& algorithm, const String& password, const String& text_encrypted)
{
	LOG(ENIGMA_CURRENT_FUNCTION);
}

void CLI::OnEncryptFile(const std::unique_ptr<Algorithm>& algorithm, const String& password, const String& in_file, const String& out_filename_encypted)
{
	LOG(ENIGMA_CURRENT_FUNCTION);
}

void CLI::OnDecryptFile(const std::unique_ptr<Algorithm>& algorithm, const String& password, const String& in_file_encrypted, const String& out_filename_decrypted)
{
	LOG(ENIGMA_CURRENT_FUNCTION);
}


CLI::~CLI() noexcept
{
}

NS_ENIGMA_END