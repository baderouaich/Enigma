/**
*	Enigma Entry Point
*/
#include <pch.hpp>

#if ! ENIGMA_TESTING
	#include <Enigma.hpp>
#else	
	#include <Tests/TestsAll.hpp>
#endif
#include <Algorithm/RSA/RSA.hpp>

int main(int argc, char* argv[])
{

	//try{
		using namespace Enigma;
		ENIGMA_BEGIN_TIMER(t1);
		// Make IDEA algorithm with intention to Encrypt and Decrypt
		std::unique_ptr<RSA> rsa(new RSA(RSA::Intent::Encrypt | RSA::Intent::Decrypt));
		rsa->SetKeySize(512);
		// Buffer to encrypt
		String buffer = Random::Str(rsa->GetMaximumBufferSize()); 

		std::cout << "Encrypting... " << std::endl << std::endl;
		String cipher = rsa->Encrypt("", buffer);
		auto s = ENIGMA_END_TIMER(t1, f64, std::milli)/1000.0;

		String private_key = rsa->GetPrivateKey();
		String public_key = rsa->GetPublicKey();
		std::cout << "took: " << s << "s to generate " << rsa->GetKeySize() << std::endl << std::endl;
		std::cout << "buffer: " << buffer << std::endl << std::endl;
		std::cout << "cipher: " << cipher << std::endl << std::endl;
		std::cout << "private_key: \n" << private_key << std::endl << std::endl;
		std::cout << "public_key: \n" << public_key << std::endl << std::endl;


		std::cout << "\nDecrypting... " << std::endl << std::endl;
		rsa->SetPrivateKey(private_key);
		rsa->SetPublicKey(public_key);
		String recovered = rsa->Decrypt("", cipher);
		std::cout << "recovered: " << recovered << std::endl << std::endl;
		ENIGMA_ASSERT(buffer == recovered);

		return 0;
//}
//catch (const CryptoPP::Exception& e)
//{
//	std::cerr << Enigma::CryptoPPUtils::GetFullErrorMessage(e) << '\n';
//	return 1;
//}
	// Initialize Logger
	Enigma::Logger::Initialize();
	// Initialize Languages Translation
	Enigma::Translation::Initialize();
	// Initialize SQLite3 Database
	Enigma::Database::Initialize();

	Enigma::i32 exit_code = -1;
	try
	{
#if ! ENIGMA_TESTING
		//========= CLI Entry =========//
		if (argc > 1)
		{
			const auto _Cli = std::make_unique<Enigma::CLI>(argc, argv);
			exit_code = _Cli->Run();
		}
		//========= UI Entry =========//
		else
		{
			// Load Window Configuration (title, width, height...)
			const Enigma::Config window_config((Enigma::FileUtils::GetEnigmaExecutableDir() / fs::path(Enigma::Constants::Config::WINDOW_CONFIG_FILE_PATH)).string());
			// Construct WindowSettings from loaded Config
			const Enigma::WindowSettings window_settings = Enigma::WindowSettings::FromConfig(window_config);
			// Create Enigma UI Application
			const auto _App = std::make_unique<Enigma::Application>(window_settings);
			// Run Application
			_App->Run();
			// Exit Gracefully
			exit_code = EXIT_SUCCESS;
		}
#else
		//========= Tests Entry =========//
		const auto _Tests = std::make_unique<Catch::Session>();
		exit_code = _Tests->run(argc, argv);
#endif
	}
	catch (const std::exception& e)
	{
		// Exit Abnormally
		ENIGMA_CRITICAL(e.what());
		exit_code = EXIT_FAILURE;
	}


	// Shutdown SQLite3 Database
	Enigma::Database::Shutdown();
	// Shutdown Languages Translation
	Enigma::Translation::Shutdown();
	// Shutdown Logger
	Enigma::Logger::Shutdown();

	return exit_code;
}


/**
*	Windows Entry Point (for Release & Distribution)
*	Note: even when using WinMain, release & dist are built as ConsoleApp so CLI will work properly.
*/
#if defined(ENIGMA_PLATFORM_WINDOWS) && (defined(ENIGMA_RELEASE) || defined(ENIGMA_DIST))

int WINAPI WinMain(
	[[maybe_unused]] _In_ HINSTANCE instance,
	[[maybe_unused]] _In_opt_ HINSTANCE prev_instance,
	[[maybe_unused]] _In_ LPSTR cmd_line,
	[[maybe_unused]] _In_ int cmd_show)
{
	return ::main(__argc, __argv);
}

#endif
