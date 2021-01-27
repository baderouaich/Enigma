#include <pch.hpp>
#include "CLI.hpp"
#include <Utility/DialogUtils.hpp>

NS_ENIGMA_BEGIN


CLI::CLI(const i32& argc, const char* const* argv)
{

	try
	{
		m_options = std::make_unique<cxxopts::Options>("Enigma", Constants::CLI::CLI_HELP_MESSAGE);
		//m_options
		//	->positional_help("positional_help")
		//	.show_positional_help();


		m_options
			->allow_unrecognised_options() // allow unknown options, will be ignored and can be retrieved with unmatched()
			.add_options()
			//option, description, value, arg_help message
			("e,encrypt", "Encrypt operation") // -e | --encrypt
			("d,decrypt", "Decrypt operation") // -d | --decrypt
			("m,mode", "Encryption/Decryption mode", cxxopts::value<std::string>()->default_value("AES"), Algorithm::GetSupportedAlgorithmsStr()) // -m aes | --mode=aes
			("p,password", "Encryption password", cxxopts::value<std::string>()) // -p "mypass" | --password="mypass"
			("t,text", "Text to Encrypt/Decrypt", cxxopts::value<std::string>()) // -t "lorem" | --text="lorem"
			("i,infile", "Input File to Encrypt/Decrypt", cxxopts::value<std::string>()) // -i "C:/file" | --infile="C:/file"
			("o,outfile", "Output File to Encrypt/Decrypt", cxxopts::value<std::string>()) // -o "C:/file" | --outfile="C:/file"
			("c,compress", "Compress File Before Encrypting") // -c | --compress 
			("u,decompress", "Decompress File After Decrypting") // -u | --decompress 
			("h,help", "Displays help message")  // HELP
			("v,version", "Displays Enigma's version")  // VERSION
			;

		m_parse_result = std::make_unique<cxxopts::ParseResult>(std::move(m_options->parse(argc, argv)));

		// Display the unmatched arguments if available
		const auto& unmatched_args = m_parse_result->unmatched();
		if (!unmatched_args.empty())
		{
			for (const auto& uarg : unmatched_args)
				ENIGMA_WARN("Unknown argument {0}", uarg);
			throw std::runtime_error("Received unknown arguments");
		}

	}
	catch (const std::exception& e)
	{
		ENIGMA_ERROR("Failed to parse arguments: {0} | {1}", e.what(), Constants::CLI::CLI_HELP_MESSAGE);
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
	if (r.count("h") || r.count("help")) // u can also check only r.count("h"), cxxopts will check the second pair item "help" if the first one isnt there
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

	ENIGMA_LOG("Processing arguments...");

	std::unique_ptr<Algorithm> algorithm{}; // Polymorphic algorithm to be created by mode name with Algorithm::CreateFromName
	Intent intent{}; // Encrypt or Decrypt?
	String mode{}; // "aes", "tripledes"..
	String password{}; // Encryption password 
	String text{}; // Text to encrypt if mode is --encrypt, otherwise cipher base64 to decrypt
	String infilename{}; // In file to encrypt if mode is --encrypt, otherwise file to be decrypted
	String outfilename{}; // Out file to ouput encrypted cipher to if mode is --encrypt, otherwise decrypted file
	bool compress{ false }, decompress{ false }; // Compress/Decompress File Before Encrypting/Decrypting
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
			throw std::runtime_error("You should specify whether you want to encrypt -e or decrypt -d");

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
		else if(intent == Algorithm::Intent::Encrypt) // If intent is encrypting, mode is required, otherwise we can detect which mode used for encryption.
			throw std::runtime_error("You should specify an encryption mode like: --mode=aes or -m aes, unless you are decrypting, then we can autodetect the mode used for encryption.");

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

		// Check if file compression/decompression enabled
		compress = r.count("c") || r.count("compress");
		decompress = r.count("u") || r.count("decompress");


		///============ Call Scenarios ============///

		// Create polymorphic Algorithm type
		
		// if mode is not set, probably user forgot which algorithm used in encryption? auto-detect it then...since first character of cipher is Algorithm::Type enum id
		if (mode.empty())
		{
			byte cipher_first_byte{(byte)Algorithm::Type::Last + 1};
			if (!text.empty())
			{
				// extract first byte from cipher which must be the mode type used in encryption
				cipher_first_byte = *std::make_unique<String>(Base64::Decode(text))->begin();
			}
			else
			{
				// check if the infile exists
				ENIGMA_ASSERT_OR_THROW(fs::exists(infilename), "infile does not exist.");
				ENIGMA_ASSERT_OR_THROW(!fs::is_empty(infilename), "infile is empty.");
				ENIGMA_ASSERT_OR_THROW(fs::is_regular_file(infilename), "infile is not a regular file.");

				// extract first byte from infile cipher which must be the mode type used in encryption
				if (std::ifstream ifs{ infilename, std::ios_base::binary | std::ios_base::in })
				{
					ifs >> cipher_first_byte;
					ifs.close();
				}
				
			}
			// Check if detected mode is valid
			ENIGMA_ASSERT_OR_THROW(ENIGMA_IS_BETWEEN(cipher_first_byte, (byte)Algorithm::Type::First, (byte)Algorithm::Type::Last),
				"Could not auto-detect algorithm mode used for encryption, please set it manually with --mode=" +
				Algorithm::GetSupportedAlgorithmsStr());

			algorithm = Algorithm::CreateFromType(static_cast<Algorithm::Type>(cipher_first_byte), intent);
			
			ENIGMA_INFO("Auto-Detected Algorithm used for encryption => {0}", algorithm->GetTypeString());
		}
		else
			algorithm = Algorithm::CreateFromName(mode, intent);

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
				this->OnEncryptFile(algorithm, password, infilename, outfilename, compress);
				break;
			case Intent::Decrypt:
				this->OnDecryptFile(algorithm, password, infilename, outfilename, decompress);
				break;
			}
		}
		else
			throw std::runtime_error("Please specify what to encrypt, either text using --text or a file using --infile & --outfile");

	}
	catch (const CryptoPP::Exception& e)
	{
		ENIGMA_ERROR("Failed to parse arguments: {0}", CryptoPPUtils::GetFullErrorMessage(e));
		return EXIT_FAILURE;
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
	// assert the pw size is 9 or more
	ENIGMA_ASSERT_OR_THROW(password.size() >= Constants::Algorithm::MINIMUM_PASSWORD_LENGTH, Constants::ErrorMessages::WEAK_PASSWORD_ERROR_MESSAGE);

	String cipher{}, cipher_base64{};
	f64 elapsed_seconds{ 0.0 };

	ENIGMA_BEGIN_TIMER(t1);
	{
		ENIGMA_TRACE("Encrypting Text with " + algorithm->GetTypeString() + " Algorithm ...");
		cipher = algorithm->Encrypt(password, text);
		ENIGMA_ASSERT_OR_THROW(!cipher.empty(), "Failed to encrypt text");

		ENIGMA_TRACE("Encoding Cipher to Base64...");
		cipher_base64 = Base64::Encode(cipher);
		ENIGMA_ASSERT_OR_THROW(!cipher_base64.empty(), "Failed to encode cipher to base64");

		elapsed_seconds = ENIGMA_END_TIMER(t1, f64, std::milli) / 1000.0;
	}

	ENIGMA_INFO(cipher_base64);
	ENIGMA_LOG("Encrypted {0} bytes in {1:0.3f} seconds. (Please save cipher base64 text above in a safe place)",
		text.size(), elapsed_seconds);
	
	cipher.clear();
	cipher_base64.clear();
}

void CLI::OnDecryptText(const std::unique_ptr<Algorithm>& algorithm, const String& password, const String& cipher_base64)
{
	// assert the pw size is 9 or more
	ENIGMA_ASSERT_OR_THROW(password.size() >= Constants::Algorithm::MINIMUM_PASSWORD_LENGTH, Constants::ErrorMessages::WEAK_PASSWORD_ERROR_MESSAGE);

	String cipher{}, decrypted_text{};
	f64 elapsed_seconds{ 0.0 };

	ENIGMA_BEGIN_TIMER(t1);
	{
		ENIGMA_TRACE("Decoding Base64 Text to Cipher...");
		cipher = Base64::Decode(cipher_base64);
		ENIGMA_ASSERT_OR_THROW(!cipher.empty(), "Failed to decode encrypted text from base64 to cipher");

		ENIGMA_TRACE("Decrypting Cipher...");
		decrypted_text = algorithm->Decrypt(password, cipher);
		ENIGMA_ASSERT_OR_THROW(!decrypted_text.empty(), "Failed to decrypt text");

		elapsed_seconds = ENIGMA_END_TIMER(t1, f64, std::milli) / 1000.0;
	}
	ENIGMA_INFO(decrypted_text);
	ENIGMA_LOG("Decrypted {0} bytes in {1:0.3f} seconds.", decrypted_text.size(), elapsed_seconds);

	cipher.clear();
	decrypted_text.clear();
}

void CLI::OnEncryptFile(const std::unique_ptr<Algorithm>& algorithm, const String& password, const String& in_filename, const String& out_filename_encypted, const bool& compress)
{
	// assert the pw size is 9 or more
	ENIGMA_ASSERT_OR_THROW(password.size() >= Constants::Algorithm::MINIMUM_PASSWORD_LENGTH, Constants::ErrorMessages::WEAK_PASSWORD_ERROR_MESSAGE);
	// assert input file is valid
	ENIGMA_ASSERT_OR_THROW(fs::exists(in_filename), "Input file " + in_filename + " does not exist");
	ENIGMA_ASSERT_OR_THROW(fs::is_regular_file(in_filename), "Input file " + in_filename + " is not a regular file");
	ENIGMA_ASSERT_OR_THROW(!fs::is_empty(in_filename), "Input file " + in_filename + " is empty");


	String buffer{}, cipher{};
	f64 elapsed_seconds{ 0.0 };

	// Read buffer from file
	ENIGMA_TRACE("Reading buffer from " + in_filename + "...");
	const bool successfully_read_file = FileUtils::Read(in_filename, buffer);
	ENIGMA_ASSERT_OR_THROW(successfully_read_file, "Failed to read file content " + in_filename);

	// Compression (if needed)
	if (compress)
	{
		ENIGMA_TRACE("Compressing file buffer {0} ...", in_filename);
		const size_t old_buffer_size = buffer.size();
		buffer = GZip::Compress(buffer);
		const size_t new_buffer_size = buffer.size();
		const size_t decreased_bytes = new_buffer_size < old_buffer_size ? (old_buffer_size - new_buffer_size) : 0;
		ENIGMA_LOG("File size decreased by {0:0.3f} MB", ENIGMA_BYTES_TO_MB(decreased_bytes));
	}

	// Encrypt
	ENIGMA_BEGIN_TIMER(t1);
	{
		ENIGMA_TRACE("Encrypting buffer with " + algorithm->GetTypeString() + " Algorithm ...");
		cipher = algorithm->Encrypt(password, buffer);
		ENIGMA_ASSERT_OR_THROW(!cipher.empty(), "Failed to encrypt file content");
		
		elapsed_seconds = ENIGMA_END_TIMER(t1, f64, std::milli) / 1000.0;
	}

	// Write cipher to file
	ENIGMA_TRACE("Writing Cipher to " + out_filename_encypted + "...");
	const bool successfully_written_file = FileUtils::Write(out_filename_encypted, cipher);
	ENIGMA_ASSERT_OR_THROW(successfully_written_file, "Failed to save cipher to file " + out_filename_encypted);

	ENIGMA_LOG("Encrypted {0} bytes in {1:0.3f} seconds.", buffer.size(), elapsed_seconds);

	buffer.clear();
	cipher.clear();
}

void CLI::OnDecryptFile(const std::unique_ptr<Algorithm>& algorithm, const String& password, const String& in_filename_encrypted, const String& out_filename_decrypted, const bool& decompress)
{
	// assert the pw size is 9 or more
	ENIGMA_ASSERT_OR_THROW(password.size() >= Constants::Algorithm::MINIMUM_PASSWORD_LENGTH, Constants::ErrorMessages::WEAK_PASSWORD_ERROR_MESSAGE);
	// assert input file is valid
	ENIGMA_ASSERT_OR_THROW(fs::exists(in_filename_encrypted), "Input file " + in_filename_encrypted + " does not exist");
	ENIGMA_ASSERT_OR_THROW(fs::is_regular_file(in_filename_encrypted), "Input file " + in_filename_encrypted + " is not a regular file");
	ENIGMA_ASSERT_OR_THROW(!fs::is_empty(in_filename_encrypted), "Input file " + in_filename_encrypted + " is empty");


	String cipher{}, buffer{}; // buffer: recovered file content
	f64 elapsed_seconds{ 0.0 };

	// Read cipher from file
	ENIGMA_TRACE("Reading Cipher from " + in_filename_encrypted + "...");
	const bool successfully_read_file = FileUtils::Read(in_filename_encrypted, cipher);
	ENIGMA_ASSERT_OR_THROW(successfully_read_file, "Failed to read file content " + in_filename_encrypted);

	// Decrypt
	ENIGMA_BEGIN_TIMER(t1);
	{
		ENIGMA_TRACE("Decrypting Cipher with " + algorithm->GetTypeString() + " Algorithm ...");
		buffer = algorithm->Decrypt(password, cipher);
		ENIGMA_ASSERT_OR_THROW(!buffer.empty(), "Failed to decrypt file content");
		elapsed_seconds = ENIGMA_END_TIMER(t1, f64, std::milli) / 1000.0;
	}

	// Decompression (if needed)
	if (decompress)
	{
		ENIGMA_TRACE("Decompressing file buffer {0} ...", in_filename_encrypted);
		const size_t old_buffer_size = buffer.size();
		buffer = GZip::Decompress(buffer);
		const size_t new_buffer_size = buffer.size();
		const size_t increased_bytes = old_buffer_size < new_buffer_size ? (new_buffer_size - old_buffer_size) : 0;
		ENIGMA_LOG("File size increased by {0:0.3f} MB", (f32(increased_bytes) / 1024.0f / 1024.0f));
	}


	// Write recovered buffer to file
	ENIGMA_TRACE("Writing Decrypted Cipher to " + out_filename_decrypted + "...");
	const bool successfully_written_file = FileUtils::Write(out_filename_decrypted, buffer);
	ENIGMA_ASSERT_OR_THROW(successfully_written_file, "Failed to save decrypted cipher to file " + out_filename_decrypted);

	ENIGMA_LOG("Decrypted {0} bytes in {1:0.3f} seconds.", buffer.size(), elapsed_seconds);

	cipher.clear();
	buffer.clear();
}


CLI::~CLI() noexcept
{
}

NS_ENIGMA_END
