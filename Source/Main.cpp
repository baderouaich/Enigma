/*
*	Enigma Entry Point
*/
#include <pch.hpp>

#if ENIGMA_TEST
	//#include <Tests/Algorithm/AESTests.hpp>
	//#include <Tests/Algorithm/ChaChaTests.hpp>
	//#include <Tests/Algorithm/TripleDESTests.hpp>
	//#include <Tests/Algorithm/TwofishTests.hpp>
	//#include <Tests/Algorithm/IDEATests.hpp>
	//#include <Tests/Algorithm/BlowfishTests.hpp>
	//#include <Tests/Utils/Base64Tests.hpp>
	//#include <Tests/Utils/GZipTests.hpp>
	//#include <Tests/Utils/FileUtilsTests.hpp>
#else
	#include <Enigma.hpp>
#endif


int main(int argc, char* argv[])
{
	// Initialize Enigma Logger
	Enigma::Logger::Initialize();
	// Initialize SQLite3 Database
	Enigma::Database::Initialize();


	Enigma::i32 exit_status = -1;
	try
	{
#if ! ENIGMA_TEST
		//========= CLI Entry =========//
		if (argc > 1)
		{
			const auto _Cli = std::make_unique<Enigma::CLI>(argc, argv);
			exit_status = _Cli->Run();
		}
		//========= UI Entry =========//
		else
		{
			// Load Window Configuration (title, width, height...)
			Enigma::Config window_config(Enigma::Constants::Config::WINDOW_CONFIG_FILE_PATH);
			// Construct WindowSettings from loaded Config
			Enigma::WindowSettings window_settings = Enigma::WindowSettings::FromConfig(window_config);
			// Create Enigma UI Application
			const auto _App = std::make_unique<Enigma::Application>(window_settings);
			// Run Application
			_App->Run();
			// Exit Successfully
			exit_status = EXIT_SUCCESS;
		}
#else
		//========= Tests Entry =========//
		exit_status = Catch::Session().run(argc, argv);
#endif
	}
	catch (const std::exception& e)
	{
		ENIGMA_CRITICAL(e.what());
		// Exit Abnormally
		exit_status = EXIT_FAILURE;
	}
	

	// Shutdown SQLite3 Database
	Enigma::Database::Shutdown();
	// Shutdown Enigma Logger
	Enigma::Logger::Shutdown();

	return exit_status;
}



/*
*	Windows Entry Point (for Release & Distribution)
*	Note: even when using WinMain, release & dist are built as ConsoleApp so CLI will work properly.
*/
#if defined(ENIGMA_PLATFORM_WINDOWS) && (defined(ENIGMA_RELEASE) || defined(ENIGMA_DIST))

int WINAPI WinMain(
	_In_ HINSTANCE instance,
	_In_opt_ HINSTANCE prev_instance,
	_In_ LPSTR cmd_line,
	_In_ int cmd_show)
{
	return ::main(__argc, __argv);
}

#endif














































#if 0 // OLD way

/*
*	Enigma Entry Point
*/
#include <pch.hpp>

#if ENIGMA_TEST
	//#include <Tests/Algorithm/AESTests.hpp>
	//#include <Tests/Algorithm/ChaChaTests.hpp>
	//#include <Tests/Algorithm/TripleDESTests.hpp>
	//#include <Tests/Algorithm/TwofishTests.hpp>
	//#include <Tests/Algorithm/IDEATests.hpp>
	//#include <Tests/Algorithm/BlowfishTests.hpp>
	//#include <Tests/Utils/Base64Tests.hpp>
	//#include <Tests/Utils/GZipTests.hpp>
#else
	#include <Enigma.hpp>
#endif

int main(int argc, char* argv[])
{
	// Initialize Enigma Logger
	Enigma::Logger::Initialize();
	// Initialize SQLite3 Database
	Enigma::Database::Initialize();
#if !ENIGMA_TEST
	// Command Line Interface Entry
	if (argc > 1)
	{
		// ======================== CLI ======================== //
		std::unique_ptr<Enigma::CLI> _Cli = std::make_unique<Enigma::CLI>(argc, argv);
		const auto exit_status =  _Cli->Run();
		// Shutdown SQLite3 Database
		Enigma::Database::Shutdown();
		// Shutdown Enigma Logger
		Enigma::Logger::Shutdown();
		// Exit
		return exit_status;
	}
	// Application Entry
	else
	{
		// ======================== UI ======================== //
		try
		{		
			// Load Window Configuration (title, width, height...)
			Enigma::Config window_config(Enigma::Constants::Config::WINDOW_CONFIG_FILE_PATH);
			// Construct WindowSettings from loaded Config
			Enigma::WindowSettings window_settings = Enigma::WindowSettings::FromConfig(window_config);
			// Create Enigma UI Application
			std::unique_ptr<Enigma::Application> _App = std::make_unique<Enigma::Application>(window_settings);
			// Run Application
			_App->Run();
			// Shutdown SQLite3 Database
			Enigma::Database::Shutdown();
			// Shutdown Enigma Logger
			Enigma::Logger::Shutdown();
			// Exit
			return EXIT_SUCCESS;
		}
		catch (const std::exception& e)
		{
			// Exit abnormally 
			ENIGMA_CRITICAL(e.what());
			// Shutdown SQLite3 Database
			Enigma::Database::Shutdown();
			// Shutdown Enigma Logger
			Enigma::Logger::Shutdown();
			// Exit
			return EXIT_FAILURE;
		}

	}
#else
	// ======================== Tests ======================== //
	// Run Tests
	const auto exit_status = Catch::Session().run(argc, argv);
	// Shutdown SQLite3 Database
	Enigma::Database::Shutdown();
	// Shutdown Enigma Logger
	Enigma::Logger::Shutdown();
	// Exit
	return exit_status;
#endif
}



/*
*	Windows Entry Point (for Release & Distribution)
*	Note: even when using WinMain as an entry point, release & distribution are built as ConsoleApp for CLI to work properly.
*/
#if defined(ENIGMA_PLATFORM_WINDOWS) && (defined(ENIGMA_RELEASE) || defined(ENIGMA_DIST))

int WINAPI WinMain(
	_In_ HINSTANCE instance, 
	_In_opt_ HINSTANCE prev_instance, 
	_In_ LPSTR cmd_line, 
	_In_ int cmd_show) 
{
	return ::main(__argc, __argv);
}

#endif

#endif // !OLD way