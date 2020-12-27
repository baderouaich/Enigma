#include <pch.hpp>
#include "CLI.hpp"

NS_ENIGMA_BEGIN


CLI::CLI(const i32& argc, char** argv)
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
			("m,mode", "Encryption/Decryption mode", cxxopts::value<std::string>()->default_value("AES"), Algorithm::GetSupportedAlgorithms()) // -m aes | --mode=aes
			("p,password", "Encryption/Decryption password", cxxopts::value<std::string>()) // -p "mypass" | --password="mypass"
			("t,text", "Text to encrypt", cxxopts::value<std::string>()) // -t "lorem" | --text="lorem"
			("i,infile", "File to Encrypt/Decrypt", cxxopts::value<std::string>()) // -i "C:/file" | --infile="C:/file"
			("o,outfile", "File to Output Cipher/Decrypted", cxxopts::value<std::string>()->default_value(fs::temp_directory_path().string() + "Enigma")) // -o "C:/file" | --outfile="C:/file"
			("h,help", "Displays help message")  // HELP
			("v,version", "Displays Enigma current version")  // VERSION
			;
			
		m_parse_result = std::make_unique<cxxopts::ParseResult>(std::move(m_options->parse(argc, argv)));

		// get the unmatched arguments
		const auto& unmatched_args = m_parse_result->unmatched();
		for (const auto& arg : unmatched_args)
		{
			ENIGMA_TRACE("Unknown argument {0}", arg.c_str());
		}
		//if (!unmatched_args.empty())
		//	ENIGMA_INFO(m_options->help());


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
	// Handle --version & -v options
	if (r.count("v") || r.count("version"))
	{
		ENIGMA_INFO(ENIGMA_VERSION);
		return EXIT_SUCCESS;
	}

	std::unique_ptr<Algorithm> algorithm{}; // polymorphic algorithm
	Intent intent{}; // Encrypt or Decrypt?
	String mode{}; // "aes", "tripledes"..
	String password{}, text{}, infilename{}, outfilename{};
	ENIGMA_TRACE("Processing arguments...");
	try
	{
		// Encrypting or Decrypting ?
		if (r.count("e") || r.count("encrypt")) // -e or --encrypt
		{
			//LOG("Encrypting ");
			intent = Algorithm::Intent::Encrypt;
		}
		else if (r.count("d") || r.count("decrypt")) // -d or --decrypt
		{
			//LOG("Decrypting ");
			intent = Algorithm::Intent::Decrypt;
		}
		else
			throw std::runtime_error("You should specify whether you want to encrypt -e or decrypt -d, or say -help");
		
// White algorithm mode are we using ?
if (r.count("m") || r.count("mode")) // --m=aes or --mode=aes or chacha..
{
	mode = r["m"].as<String>();
	StringUtils::Lower(mode);

	//Hardcoded argh!!
	//if (m == "aes") mode = Algorithm::Type::AES;
	//else if (m == "chacha") mode = Algorithm::Type::ChaCha20;
	//else if (m == "tripledes") mode = Algorithm::Type::TripleDES;
	//else throw std::runtime_error("Unsupported algorithm mode: " + m);

	//LOG("Mode: {0}", m);
}
else
throw std::runtime_error("You should specify an encryption/decryption mode like: --mode=aes or -m aes");

// What is the encryption/decryption password?
if (r.count("p") || r.count("password")) // -p "mypass" | --password="mypass"
{
	password = r["p"].as<String>();

	//LOG("Password: {0}", password);
}
else
throw std::runtime_error("You should specify an encryption/decryption password like: --password=mypass or -p mypass");


// Is there a text to encrypt/decrypt?
if (r.count("t") || r.count("text")) // -t "lorem" | --text="lorem"
{
	text = r["t"].as<String>();

	//LOG("Text: {0}", text);
}

// Or Is there a file to encrypt/decrypt?
if (r.count("i") || r.count("infile")) // -i "C:/file" | --infile="C:/file_to_encrypt.txt"
{
	infilename = r["i"].as<String>();

	//LOG("In File name: {0}", infilename);
}
if (r.count("o") || r.count("outfile")) // -o "C:/file" | --outfile="C:/file"
{
	outfilename = r["o"].as<String>();

	//LOG("Out File name: {0}", outfilename);
}


// Call Scenarios //
// Create polymorphic Algorithm type
algorithm = this->CreateAlgorithm(mode, intent);

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

std::unique_ptr<Algorithm> CLI::CreateAlgorithm(const String& mode, const Intent& intent)
{
	//auto ModeIn = [&mode](vector<> v)
	if (mode == "aes")
		return std::make_unique<Enigma::AES>(intent);
	else if (mode == "chacha")
		return std::make_unique<Enigma::ChaCha20>(intent);
	else if (mode == "tripledes")
		return std::make_unique<Enigma::TripleDES>(intent);
	//else if (mode == "twofish")
	//	return std::make_unique<Enigma::TwoFish>(intent);
	//else if (mode == "idea")
	//	return std::make_unique<Enigma::IDEA>(intent);
	else
		throw std::runtime_error("Unsupported algorithm mode: " + mode);
}

void CLI::OnEncryptText(const std::unique_ptr<Algorithm>& algorithm, const String& password, const String& text)
{
	// assert the pw size is 9 or more
	ENIGMA_ASSERT_OR_THROW(password.size() >= Constants::Algorithm::MINIMUM_PASSWORD_LENGTH, Constants::ErrorMessages::WEAK_PASSWORD_ERROR_MESSAGE);

	String encrypted_text{}, encrypted_text_base64{};
	f64 elapsed_seconds{0.0};

	ENIGMA_BEGIN_TIMER(t1);
	{
		ENIGMA_TRACE("Encrypting Text With " + algorithm->GetTypeString() + " Algorithm ...");
		encrypted_text = algorithm->Encrypt(password, text);
		ENIGMA_ASSERT_OR_THROW(!encrypted_text.empty(), "Failed to encrypt text");

		ENIGMA_TRACE("Encoding Cipher to Base64...");
		encrypted_text_base64 = Base64::Encode(encrypted_text);
		ENIGMA_ASSERT_OR_THROW(!encrypted_text_base64.empty(), "Failed to encode encrypted text to base64");

		elapsed_seconds = ENIGMA_END_TIMER(t1, f64, std::milli) / 1000.0;
	}
	
	ENIGMA_INFO(encrypted_text_base64);
	ENIGMA_INFO("Encrypted {0} bytes in {1:0.3f} seconds.", text.size(), elapsed_seconds);
	
	encrypted_text.clear();
	encrypted_text_base64.clear();
}

void CLI::OnDecryptText(const std::unique_ptr<Algorithm>& algorithm, const String& password, const String& encrypted_text_base64)
{
	// assert the pw size is 9 or more
	ENIGMA_ASSERT_OR_THROW(password.size() >= Constants::Algorithm::MINIMUM_PASSWORD_LENGTH, Constants::ErrorMessages::WEAK_PASSWORD_ERROR_MESSAGE);

	String encrypted_text{}, decrypted_text{};
	f64 elapsed_seconds{0.0};

	ENIGMA_BEGIN_TIMER(t1);
	{
		ENIGMA_TRACE("Decoding Base64 Text to Cipher...");
		encrypted_text = Base64::Decode(encrypted_text_base64);
		ENIGMA_ASSERT_OR_THROW(!encrypted_text.empty(), "Failed to decode encrypted text from base64 to cipher");

		ENIGMA_TRACE("Decrypting Cipher...");
		decrypted_text = algorithm->Decrypt(password, encrypted_text);
		ENIGMA_ASSERT_OR_THROW(!decrypted_text.empty(), "Failed to decrypt text");

		elapsed_seconds = ENIGMA_END_TIMER(t1, f64, std::milli) / 1000.0;
	}
	ENIGMA_INFO(decrypted_text);
	ENIGMA_INFO("Decrypted {0} bytes in {1:0.3f} seconds.", decrypted_text.size(), elapsed_seconds);

	encrypted_text.clear();
	decrypted_text.clear();
}

void CLI::OnEncryptFile(const std::unique_ptr<Algorithm>& algorithm, const String& password, const String& in_filename, const String& out_filename_encypted)
{
	// assert the pw size is 9 or more
	ENIGMA_ASSERT_OR_THROW(password.size() >= Constants::Algorithm::MINIMUM_PASSWORD_LENGTH, Constants::ErrorMessages::WEAK_PASSWORD_ERROR_MESSAGE);
	// assert input file is valid
	ENIGMA_ASSERT_OR_THROW(fs::exists(in_filename), "Input file " + in_filename + " does not exist");
	

	String buffer{}, encrypted_buffer{};
	f64 elapsed_seconds{0.0};

	
	ENIGMA_TRACE("Reading buffer from " + in_filename + "...");
	const bool successfully_read_file = FileUtils::Read(in_filename, buffer);
	ENIGMA_ASSERT_OR_THROW(successfully_read_file, "Failed to read file content " + in_filename);
	
	ENIGMA_BEGIN_TIMER(t1);
	{
		ENIGMA_TRACE("Encrypting buffer with " + algorithm->GetTypeString() + " Algorithm ...");
		encrypted_buffer = algorithm->Encrypt(password, buffer);
		ENIGMA_ASSERT_OR_THROW(!encrypted_buffer.empty(), "Failed to encrypt file content");
		elapsed_seconds = ENIGMA_END_TIMER(t1, f64, std::milli) / 1000.0;
	}

	ENIGMA_TRACE("Writing Cipher to " + out_filename_encypted + "...");
	const bool successfully_written_file = FileUtils::Write(out_filename_encypted, encrypted_buffer);
	ENIGMA_ASSERT_OR_THROW(successfully_written_file, "Failed to save cipher to file " + out_filename_encypted);
	
	ENIGMA_INFO("Encrypted {0} bytes in {1:0.3f} seconds.", buffer.size(), elapsed_seconds);

	buffer.clear();
	encrypted_buffer.clear();
}

void CLI::OnDecryptFile(const std::unique_ptr<Algorithm>& algorithm, const String& password, const String& in_filename_encrypted, const String& out_filename_decrypted)
{
	// assert the pw size is 9 or more
	ENIGMA_ASSERT_OR_THROW(password.size() >= Constants::Algorithm::MINIMUM_PASSWORD_LENGTH, Constants::ErrorMessages::WEAK_PASSWORD_ERROR_MESSAGE);
	// assert input file is valid
	ENIGMA_ASSERT_OR_THROW(fs::exists(in_filename_encrypted), "Input file " + in_filename_encrypted + " does not exist");


	String cipher{}, decrypted_cipher{};
	f64 elapsed_seconds{ 0.0 };


	ENIGMA_TRACE("Reading Cipher from " + in_filename_encrypted + "...");
	const bool successfully_read_file = FileUtils::Read(in_filename_encrypted, cipher);
	ENIGMA_ASSERT_OR_THROW(successfully_read_file, "Failed to read file content " + in_filename_encrypted);

	ENIGMA_BEGIN_TIMER(t1);
	{
		ENIGMA_TRACE("Decrypting Cipher with " + algorithm->GetTypeString() + " Algorithm ...");
		decrypted_cipher = algorithm->Decrypt(password, cipher);
		ENIGMA_ASSERT_OR_THROW(!decrypted_cipher.empty(), "Failed to decrypt file content");
		elapsed_seconds = ENIGMA_END_TIMER(t1, f64, std::milli) / 1000.0;
	}

	ENIGMA_TRACE("Writing Decrypted Cipher to " + out_filename_decrypted + "...");
	const bool successfully_written_file = FileUtils::Write(out_filename_decrypted, decrypted_cipher);
	ENIGMA_ASSERT_OR_THROW(successfully_written_file, "Failed to save decrypted cipher to file " + out_filename_decrypted);

	ENIGMA_INFO("Decrypted {0} bytes in {1:0.3f} seconds.", decrypted_cipher.size(), elapsed_seconds);

	cipher.clear();
	decrypted_cipher.clear();
}


CLI::~CLI() noexcept
{

}

NS_ENIGMA_END
