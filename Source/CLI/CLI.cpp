#include <pch.hpp>
#include "CLI.hpp"
#include <Database/Models/Encryption.hpp>
#include <Database/Database.hpp>
#include <Utility/DialogUtils.hpp>
#include <Utility/Base64.hpp>
#include <Utility/GZip.hpp>
#include <Utility/SizeUtils.hpp>
#include <Utility/StringUtils.hpp>
#include <Utility/FileUtils.hpp>
#include <Networking/CheckForUpdates.hpp>

NS_ENIGMA_BEGIN


CLI::CLI(const i32 argc, const char* const* argv)
{
	ENIGMA_TRACE_CURRENT_FUNCTION();
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
			("a,algorithm", "Encryption/Decryption algorithm", cxxopts::value<std::string>()->default_value("AES"), Algorithm::GetSupportedAlgorithmsStr()) // -a aes | --algorithm=aes
			("p,password", "Encryption password", cxxopts::value<std::string>()) // -p "mypass" | --password="mypass"
			("t,text", "Text to Encrypt or Decrypt (base64)", cxxopts::value<std::string>()) // -t "lorem" | --text="lorem"
			("i,infile", "Input File to Encrypt/Decrypt", cxxopts::value<std::string>()) // -i "C:/file" | --infile="C:/file"
			("o,outfile", "Output File to Encrypt/Decrypt", cxxopts::value<std::string>()) // -o "C:/file" | --outfile="C:/file"			
			("s,save", "Save encryption record to database")  // save to database
			("l,list", "List saved encryption records from database")  // show saved encryption records
			("h,help", "Displays help message")  // HELP
			("v,version", "Displays Enigma's version")  // VERSION
			("check-for-updates", "Check for new version releases")  // Check for enigma updates from github api
			;

		m_parse_result = std::make_unique<cxxopts::ParseResult>(std::move(m_options->parse(argc, argv)));

		// Display the unmatched arguments if available
		const auto& unmatched_args = m_parse_result->unmatched();
		if (!unmatched_args.empty())
		{
			for (const auto& uarg : unmatched_args)
				ENIGMA_WARN("Unknown argument {}", uarg);
			throw std::runtime_error("Received unknown arguments");
		}

	}
	catch (const std::exception& e)
	{
		ENIGMA_ERROR("Failed to parse arguments: {} | {}", e.what(), Constants::CLI::CLI_HELP_MESSAGE);
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

	// Handle -l & --list options (Show saved encryption records)
	if (r.count("l") || r.count("list"))
	{
		this->OnListEncryptionRecords();
		return EXIT_SUCCESS;
	}
	// Handle --help & -h options
	if (r.count("h") || r.count("help")) // u can also check only r.count("h"), cxxopts will check the second pair item "help" if the first one isnt there
	{
		this->OnHelp();
		return EXIT_SUCCESS;
	}
	// Handle --version & -v options
	if (r.count("v") || r.count("version"))
	{
		this->OnVersion();
		return EXIT_SUCCESS;
	}
	// Handle --check-for-updates
	if (r.count("check-for-updates"))
	{
		this->OnCheckForUpdates();
		return EXIT_SUCCESS;
	}

	ENIGMA_LOG("Processing arguments...");

	std::unique_ptr<Algorithm> algorithm{}; // Polymorphic algorithm to be created by mode name with Algorithm::CreateFromName
	Algorithm::Intent intent{}; // Encrypt or Decrypt? to save memory resources not needed in a specific operation
	String algo{}; // "aes", "tripledes"..
	String password{}; // Encryption password 
	String text{}; // Text to encrypt if mode is --encrypt, otherwise cipher base64 to decrypt
	String infilename{}; // In file to encrypt if mode is --encrypt, otherwise file to be decrypted
	String outfilename{}; // Out file to ouput encrypted cipher to if mode is --encrypt, otherwise decrypted file
	const bool save_to_database = !!r.count("save"); // Save encryption record to database
	try
	{
		// Encrypting or Decrypting ?
		if (r.count("e") || r.count("encrypt")) // -e or --encrypt
		{
			//LOG("Encrypting ");
			intent |= Algorithm::Intent::Encrypt;
		}
		else if (r.count("d") || r.count("decrypt")) // -d or --decrypt
		{
			//LOG("Decrypting ");
			intent |= Algorithm::Intent::Decrypt;
		}
		else
			throw std::runtime_error("You should specify whether you want to encrypt -e or decrypt -d");

		// White algorithm are we using ?
		if (r.count("a") || r.count("algorithm")) // --a=aes or --algorithm=aes or chacha..
		{
			algo = r["a"].as<String>();
			//LOG("Algorithm: {0}", m);
		}
		else if(intent & Algorithm::Intent::Encrypt) // If intent is encrypting, algorithm is required, otherwise we can detect which mode used for encryption.
			throw std::runtime_error("You should specify an encryption algorithm example -a aes, unless you are decrypting, then we can auto-detect the algorithm used for encryption.");
		// What is the encryption/decryption password?
		if (r.count("p") || r.count("password")) // -p "mypass" | --password="mypass"
		{
			password = r["p"].as<String>();

			//LOG("Password: {0}", password);
		}
		else
			throw std::runtime_error("You should specify an encryption/decryption password example -p mypass");


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



		///============ Call Scenarios ============///

		// Create polymorphic Algorithm type
		// if algorithm is not set, probably user forgot which algorithm used in encryption? auto-detect it then...since first character of cipher is Algorithm::Type enum id
		if (algo.empty())
		{
			Algorithm::Type type{};
			if (!text.empty()) // Cipher Base64 Text
			{
				type = Algorithm::DetectFromCipherBase64(text);
			}
			else // File
			{
				type = Algorithm::DetectFromFile(infilename);
			}
			algorithm = Algorithm::CreateFromType(type, intent);

			// little happy msg for user
			ENIGMA_INFO("Successfully auto-detected algorithm used for encryption which is {}",  Algorithm::AlgoTypeEnumToStr(type));
		}
		else
		{
			algorithm = Algorithm::CreateFromName(algo, intent);
		}


		// Check wether its a text or file encryption/decryption
		if (!text.empty())
		{
			// Check intention
			if(intent & Algorithm::Intent::Encrypt)
				this->OnEncryptText(algorithm, password, text, save_to_database);
			else 
				this->OnDecryptText(algorithm, password, text);


			/*
			that was before Intent enum was bitshifts based
			// Check intention
			switch (intent)
			{
			case Algorithm::Intent::Encrypt:
				this->OnEncryptText(algorithm, password, text, save_to_database);
				break;
			case Algorithm::Intent::Decrypt:
				this->OnDecryptText(algorithm, password, text);
				break;
			}*/
		}
		else if (!infilename.empty() && !outfilename.empty())
		{

			// Check intention
			if(intent & Algorithm::Intent::Encrypt)
				this->OnEncryptFile(algorithm, password, infilename, outfilename, save_to_database);
			else 
				this->OnDecryptFile(algorithm, password, infilename, outfilename);

			/*
			that was before Intent enum was bitshifts based
			switch (intent)
			{
			case Algorithm::Intent::Encrypt:
				this->OnEncryptFile(algorithm, password, infilename, outfilename, save_to_database);
				break;
			case Algorithm::Intent::Decrypt:
				this->OnDecryptFile(algorithm, password, infilename, outfilename);
				break;
			}*/
		}
		else
			throw std::runtime_error("Please specify what to encrypt, either text using --text or a file using --infile & --outfile");

	}
	catch (const CryptoPP::Exception& e)
	{
		ENIGMA_ERROR("Failed to parse arguments: {}", CryptoPPUtils::GetFullErrorMessage(e));
		return EXIT_FAILURE;
	}
	catch (const std::exception& e)
	{
		ENIGMA_ERROR("Failed to parse arguments: {}", e.what());
		return EXIT_FAILURE;
	}
	catch (...)
	{
		ENIGMA_ERROR("Failed to parse arguments: UNKNOWN ERROR");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

void CLI::OnEncryptText(const std::unique_ptr<Algorithm>& algorithm, const String& password, const String& text, const bool save_to_database)
{
	// assert the pw size is 6 or more
	ENIGMA_ASSERT_OR_THROW(password.size() >= Constants::Algorithm::MINIMUM_PASSWORD_LENGTH,
		fmt::format("Password is too weak! consider using {} characters or more including special characters like {}", Constants::Algorithm::MINIMUM_PASSWORD_LENGTH, Constants::Algorithm::SPECIAL_CHARACTERS));

	String cipher{}, cipher_base64{};
	f64 elapsed_seconds{ 0.0 };

	// Compression
	const String compressed_text = GZip::Compress(text);
	ENIGMA_ASSERT_OR_THROW(!compressed_text.empty(), ("Failed to compress text"));

	ENIGMA_BEGIN_TIMER(t1);
	{
		ENIGMA_TRACE("Encrypting Text with {} Algorithm...", algorithm->GetTypeString());
		cipher = algorithm->Encrypt(password, compressed_text);
		ENIGMA_ASSERT_OR_THROW(!cipher.empty(), ("Failed to encrypt text"));

		ENIGMA_TRACE("Encoding Cipher to Base64...");
		cipher_base64 = Base64::Encode(cipher);
		ENIGMA_ASSERT_OR_THROW(!cipher_base64.empty(), "Failed to encode cipher to base64");

		elapsed_seconds = ENIGMA_END_TIMER(t1, f64, std::milli) / 1000.0;
	}

	ENIGMA_INFO(cipher_base64);
	ENIGMA_LOG(fmt::format("Encrypted {0} in {1:0.3f} seconds. (Please save cipher base64 text above in a safe place)",
		 SizeUtils::FriendlySize(text.size()), elapsed_seconds));

	// Save encryption record to database on option -s | --save 
	if(save_to_database)
	{
		String title{};
		ENIGMA_LOG("Save encryption to database, please enter encryption title (example My Github Password). it helps with searching through encryptions from the database in the future: ");
		std::cout << "> ";
		std::getline(std::cin, title);

		ENIGMA_ASSERT_OR_THROW(ENIGMA_IS_BETWEEN(title.size(), 3, 255), "Encryption title is too long or short, must be between 3 and 255 characters");
		auto e = std::make_unique<Encryption>();
		e->title = title;
		e->is_file = false;
		e->cipher.data = cipher; // already compressed above
		e->size = static_cast<decltype(Encryption::size)>(e->cipher.data.size());
		ENIGMA_ASSERT_OR_THROW(Database::AddEncryption(e), "Failed to save encryption record to database");

		ENIGMA_INFO("Encryption saved successfully.");
	}
	
	cipher.clear();
	cipher_base64.clear();
}

void CLI::OnDecryptText(const std::unique_ptr<Algorithm>& algorithm, const String& password, const String& cipher_base64)
{
	// assert the pw size is 6 or more
	ENIGMA_ASSERT_OR_THROW(password.size() >= Constants::Algorithm::MINIMUM_PASSWORD_LENGTH, 
		fmt::format("Password is too weak! encryption passwords are mimimum {} characters", Constants::Algorithm::MINIMUM_PASSWORD_LENGTH));

	String cipher{}, decrypted_text{};
	f64 elapsed_seconds{ 0.0 };

	ENIGMA_BEGIN_TIMER(t1);
	{
		ENIGMA_TRACE("Decoding Base64 Text to Cipher...");
		cipher = Base64::Decode(cipher_base64);
		ENIGMA_ASSERT_OR_THROW(!cipher.empty(), "Failed to decode encrypted text from base64 to cipher");

		ENIGMA_TRACE("Decrypting Cipher...");
		decrypted_text = algorithm->Decrypt(password, cipher);
		ENIGMA_ASSERT_OR_THROW(!decrypted_text.empty(), "Failed to decrypt cipher");

		elapsed_seconds = ENIGMA_END_TIMER(t1, f64, std::milli) / 1000.0;
	}
	// Decompress 
	const String recovered_text = GZip::Decompress(decrypted_text);
	ENIGMA_ASSERT_OR_THROW(!recovered_text.empty(), ("Failed to decompress recovered text"));

	ENIGMA_INFO(recovered_text);
	ENIGMA_LOG("Decrypted {0} in {1:0.3f} seconds.", SizeUtils::FriendlySize(decrypted_text.size()), elapsed_seconds);

	cipher.clear();
	decrypted_text.clear();
}

void CLI::OnEncryptFile(const std::unique_ptr<Algorithm>& algorithm, const String& password, const String& in_filename, const String& out_filename_encypted, const bool save_to_database)
{
	// assert the pw size is 9 or more
	ENIGMA_ASSERT_OR_THROW(password.size() >= Constants::Algorithm::MINIMUM_PASSWORD_LENGTH, 
		("Password is too weak! consider using {} characters or more including special characters like {}", Constants::Algorithm::MINIMUM_PASSWORD_LENGTH, Constants::Algorithm::SPECIAL_CHARACTERS));
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

	// Compression
	ENIGMA_TRACE("Compressing file buffer {0} ...", in_filename);
	auto old_buffer_size = buffer.size();
	buffer = GZip::Compress(buffer); ENIGMA_ASSERT_OR_THROW(!buffer.empty(), "Failed to compress file content");
	auto new_buffer_size = buffer.size();
	auto decreased_bytes = new_buffer_size < old_buffer_size ? (old_buffer_size - new_buffer_size) : 0;
	ENIGMA_LOG("File size decreased by {0}", SizeUtils::FriendlySize(decreased_bytes));
	

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

	ENIGMA_LOG("Encrypted {0} in {1:0.3f} seconds.", SizeUtils::FriendlySize(buffer.size()), elapsed_seconds);

	// Save encryption record to database on option -s | --save (Note: file buffer forced to be compressed above if saving to database)
	if (save_to_database)
	{
		String title{};
		ENIGMA_LOG("Save encryption to database, please enter encryption title (e.g: My Image): ");
		std::cout << "> ";
		std::getline(std::cin, title);
		
		ENIGMA_ASSERT_OR_THROW(ENIGMA_IS_BETWEEN(title.size(), 3, 255), "Encryption title is too long or short, must be between 3 and 255 characters");
		auto e = std::make_unique<Encryption>();
		e->title = title;
		e->is_file = true;
		e->cipher.data = cipher; // already compressed above
		e->size = static_cast<decltype(Encryption::size)>(e->cipher.data.size());
		ENIGMA_ASSERT_OR_THROW(Database::AddEncryption(e), "Failed to save encryption record to database");

		ENIGMA_INFO("Encryption saved successfully.");
	}


	buffer.clear();
	cipher.clear();
}

void CLI::OnDecryptFile(const std::unique_ptr<Algorithm>& algorithm, const String& password, const String& in_filename_encrypted, const String& out_filename_decrypted)
{
	// assert the pw size is 9 or more
	ENIGMA_ASSERT_OR_THROW(password.size() >= Constants::Algorithm::MINIMUM_PASSWORD_LENGTH, 
		("Password is too weak! consider using {} characters or more including special characters like {}", Constants::Algorithm::MINIMUM_PASSWORD_LENGTH, Constants::Algorithm::SPECIAL_CHARACTERS));
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

	// Decompression 	
	ENIGMA_TRACE("Decompressing file buffer {0} ...", in_filename_encrypted);
	auto old_buffer_size = buffer.size();
	buffer = GZip::Decompress(buffer); ENIGMA_ASSERT_OR_THROW(!buffer.empty(), "Failed to decompress recovered file content");
	auto new_buffer_size = buffer.size();
	auto increased_bytes = old_buffer_size < new_buffer_size ? (new_buffer_size - old_buffer_size) : 0;
	ENIGMA_LOG("File size increased by {0}", SizeUtils::FriendlySize(increased_bytes));


	// Write recovered buffer to file
	ENIGMA_TRACE("Writing Decrypted Cipher to " + out_filename_decrypted + "...");
	const bool successfully_written_file = FileUtils::Write(out_filename_decrypted, buffer);
	ENIGMA_ASSERT_OR_THROW(successfully_written_file, "Failed to save decrypted cipher to file " + out_filename_decrypted);

	ENIGMA_LOG("Decrypted in {1:0.3f} seconds.", SizeUtils::FriendlySize(buffer.size()), elapsed_seconds);

	cipher.clear();
	buffer.clear();
}

void CLI::OnListEncryptionRecords()
{
	const std::vector<std::unique_ptr<Encryption>> encryptions =
		Database::GetAllEncryptions<true, false, true, true, true>(
			Database::OrderBy::DateTime,
			Database::Order::Ascending);

	if (encryptions.empty())
	{
		ENIGMA_TRACE("No Encryption records saved yet.");
		return;
	}

	using namespace tabulate;

	Table table{};
	table.add_row({ "ID", "Title", "Date Time", "Size", "Format" });

	// Make table header
	for (size_t i = 0; i < table.row(0).size(); i++)
		table
		.column(i)
		.format()
		.background_color(Color::cyan)
		.font_color(Color::white)
		.font_align(FontAlign::center)
		.font_style({ FontStyle::bold, FontStyle::italic });

	// Make table body
	std::for_each(encryptions.begin(), encryptions.end(), [&table](const std::unique_ptr<Encryption>& e)
	{
		const auto& [ide, title, cipher, date_time, size, is_file] = *e;

		//substr title to 50 chars only
		String sub_title = title;
		if (sub_title.size() > 50)
			sub_title = sub_title.substr(0, 50) + "...";

		table.add_row({ std::to_string(ide), sub_title, date_time, SizeUtils::FriendlySize(size), (is_file ? "File" : "Text") })
			.format()
			.font_align(FontAlign::left);
	});	 

	// Display table
	if (std::cout)
		std::cout << table << std::endl;
	else
		std::printf("%s\n", table.str().c_str());
}


void CLI::OnHelp()
{
	ENIGMA_INFO(m_options->help());
}

void CLI::OnVersion()
{
	ENIGMA_INFO("Enigma version {0}", ENIGMA_VERSION);
}

void CLI::OnCheckForUpdates()
{
	// Check for enigma updates from github api --check-for-updates
	ENIGMA_TRACE("Retrieving Enigma's latest release info from {}...", Enigma::Constants::Links::ENIGMA_GITHUB_API_LATEST_RELEASE);
	const auto info = CheckForUpdates::GetLatestReleaseInfo();
	if (!info) return;

	const CheckForUpdates::Version current_version(ENIGMA_VERSION);
	const CheckForUpdates::Version& latest_version = info->version;

	std::ostringstream oss{};
	if (current_version == latest_version)
	{ 
		oss << "You are using the latest Enigma version " << info->tag_name;
	}
	else if (latest_version > current_version)
	{
		oss << "New version is available!\n"
			<< "# Name: " << info->name << '\n'
			<< "# Version: " << info->tag_name << '\n'
			<< "# Created At: " << info->created_at << '\n'
			<< "# Published At: " << info->published_at << '\n'
			<< "# What's new ?: " << info->body << '\n'
			<< "# .tar release download url: " << info->tarball_url << '\n'
			<< "# .zip release download url: " << info->zipball_url << '\n';
	}
	else if (latest_version < current_version) // [[unlikely]] // please don't happen!
	{
		oss << "This version of Enigma is newer than the latest version available! there must have been some bug, or you have compiled Enigma with a higher version than it currently is, please report this issue to " << Constants::Links::ENIGMA_GITHUB_REPOSITORY_ISSUES;
	}
	ENIGMA_LOG(oss.str());

#if 0
	// Check for enigma updates from github api --updates | -n 
	ENIGMA_TRACE("Retrieving Enigma's latest release info from {0}...", Enigma::Constants::Links::ENIGMA_GITHUB_API_LATEST_RELEASE);
	const auto info = CheckForUpdates::GetLatestReleaseInfo();
	if (!info)
		return;

	const auto current_version = "v" + String(ENIGMA_VERSION);
	std::ostringstream oss{};
	if (info->tag_name == current_version)
	{
		oss << "You are using the latest Enigma version " << current_version;
	}
	else
	{
		oss << "New version is available!\n"
			<< "# Name: " << info->name << '\n'
			<< "# Version: " << info->tag_name << '\n'
			<< "# Created At: " << info->created_at << '\n'
			<< "# Published At: " << info->published_at << '\n'
			<< "# What's new ?: " << info->body << '\n'
			<< "# .tar release download url: " << info->tarball_url << '\n'
			<< "# .zip release download url: " << info->zipball_url << '\n';
	}
	ENIGMA_LOG(oss.str());
#endif
}

NS_ENIGMA_END
