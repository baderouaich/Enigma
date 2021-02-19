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
	//#include <Tests/Utils/Base64Tests.hpp>
	//#include <Tests/Utils/GZipTests.hpp>
#else
	#include <Enigma.hpp>
#endif

//#include <Networking/CheckForUpdates.hpp>
#include <cpr/cpr.h>
//

int main(int argc, char* argv[])
{	
	//Test 
	const auto& current_version = Enigma::ENIGMA_VERSION;
	cpr::Response r = cpr::Get(cpr::Url{ Enigma::Constants::Links::ENIGMA_GITHUB_API_LATEST_RELEASE });
	if (r.status_code == cpr::status::HTTP_OK)
	{
		std::cout << "r.status_code " << r.status_code << std::endl;                  // 200
		std::cout << "r.header " << r.header["content-type"] << std::endl;       // application/json; charset=utf-8
		std::cout << "r.text " << r.text << std::endl;
	}
	return 0;

	
	// Initialize Enigma Logger
	Enigma::Logger::Initialize();
#if !ENIGMA_TEST
	// Command Line Interface Entry
	if (argc > 1)
	{
		std::unique_ptr<Enigma::CLI> _Cli = std::make_unique<Enigma::CLI>(argc, argv);
		auto ret = _Cli->Run();
		Enigma::Logger::Shutdown();
		return ret;
	}
	// Application Entry
	else
	{
		try
		{
			// Create Enigma UI Application
			Enigma::Config window_config(Enigma::Constants::Config::ENIGMA_WINDOW_SETTINGS_CONFIG_PATH);
			Enigma::WindowSettings window_settings = Enigma::WindowSettings::FromConfig(window_config);
			std::unique_ptr<Enigma::Application> _App = std::make_unique<Enigma::Application>(window_settings);
			// Run Application
			_App->Run();
			// Shutdown Logger
			Enigma::Logger::Shutdown();
			// Exit
			return EXIT_SUCCESS;
		}
		catch (const std::exception& e)
		{
			// Exit abnormally 
			ENIGMA_CRITICAL(e.what());
			// Shutdown Logger
			Enigma::Logger::Shutdown();
			// Exit
			return EXIT_FAILURE;
		}

	}
#else
	// Run Tests
	return Catch::Session().run(argc, argv);
#endif
}



/*
*	Windows Entry Point (for Release & Distribution)
*	Note: even when using WinMain, release & dist are exported as ConsoleApp for CLI to work.
*/
#if defined(ENIGMA_PLATFORM_WINDOWS) && (defined(ENIGMA_RELEASE) || defined(ENIGMA_DIST))

int WINAPI WinMain(
	_In_ HINSTANCE instance, 
	_In_opt_ HINSTANCE prev_instance, 
	_In_ LPSTR cmd_line, 
	_In_ int cmd_show) 
{
	return main(__argc, __argv);
}

#endif

