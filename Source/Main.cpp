/**
*	Enigma Entry Point
*/
#include <pch.hpp>

#if ! ENIGMA_TESTING
	#include <Enigma.hpp>
#else	
	#include <Tests/TestsAll.hpp>
#endif

static void SignalHandler(int sig);

int main(int argc, char* argv[])
{	
	// Handle abnormal exists to normally end program and release resources gracefully
	std::signal(SIGABRT, SignalHandler); // Abnormal termination triggered by abort call
	std::signal(SIGFPE,  SignalHandler); // Floating point exception
	std::signal(SIGILL,  SignalHandler); // Illegal instruction - invalid function image
	std::signal(SIGINT,  SignalHandler); // Interrupt e.g console CTRL+C
	std::signal(SIGSEGV, SignalHandler); // Segment violation
	std::signal(SIGTERM, SignalHandler); // Software termination signal from kill
#if defined(ENIGMA_PLATFORM_WINDOWS)
	std::signal(SIGBREAK, SignalHandler); //  On Windows, a click on console window close button will raise SIGBREAK
#endif

	// Initialize Logger
	Enigma::Logger::Initialize();
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
	// Shutdown Logger
	Enigma::Logger::Shutdown();

	return exit_code;
}

/**
*	Handles exit signals to release program resources gracefully
*/
static void SignalHandler(int sig)
{
	const auto stringify_signal = [sig]() noexcept -> const char*
	{
#define RET_STR(s, desc) case s: return #s ## ": " ## desc
		switch (sig) {
		default: return "<unknown signal>";
			RET_STR(SIGABRT, "Abnormal termination triggered by abort call");
			RET_STR(SIGFPE, "Floating point exception");
			RET_STR(SIGILL, "Illegal instruction - invalid function image");
			RET_STR(SIGINT, "Interrupt");
			RET_STR(SIGSEGV, "Segment violation");
			RET_STR(SIGTERM, "Software termination signal from kill");
#if defined(ENIGMA_PLATFORM_WINDOWS)
			RET_STR(SIGBREAK, "Ctrl-Break sequence");
#endif  
#undef RET_STR
		}
	};

	if (Enigma::Logger::GetLogger()) {
	   ENIGMA_INFO("Signal handler invoked due abnormal exit with signal: {} ({})", sig, stringify_signal());
	   ENIGMA_INFO("Exiting gracefully...");
	}

	// Shutdown Application
	if (Enigma::Application::GetInstance()) Enigma::Application::GetInstance()->~Application();
	// Shutdown SQLite3 Database
	if (Enigma::Database::GetInstance()) Enigma::Database::Shutdown();
	// Shutdown Logger
	if (Enigma::Logger::GetLogger()) Enigma::Logger::Shutdown();

	std::exit(EXIT_SUCCESS);
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
