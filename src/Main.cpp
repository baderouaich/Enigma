#include <pch.hpp>
#include <Enigma.hpp>

static void SignalHandler(const int sig);

int main(int argc, char* argv[]) 
{
  using namespace Enigma;

	// Handle abnormal exists to normally end program and release resources gracefully
	std::signal(SIGABRT, SignalHandler); // Abnormal termination triggered by abort call
	std::signal(SIGFPE,  SignalHandler); // Floating point exception
	std::signal(SIGILL,  SignalHandler); // Illegal instruction - invalid function image
	std::signal(SIGINT,  SignalHandler); // Interrupt e.g console CTRL+C
	std::signal(SIGSEGV, SignalHandler); // Segment violation (nullptr read access violation exception...)
	std::signal(SIGTERM, SignalHandler); // Software termination signal from kill
#if defined(ENIGMA_PLATFORM_WINDOWS)
	std::signal(SIGBREAK, SignalHandler); // On Windows, a click on console window close button will raise SIGBREAK
#endif

	Logger::Initialize();
	Database::Initialize();

	std::int32_t exit_code = -1;
	try
	{
		//========= CLI Entry =========//
		if (argc > 1)
		{
			const std::unique_ptr<CLI> _Cli(new CLI(argc, argv));
			exit_code = _Cli->Run();
		}
		//========= UI Entry =========//
		else
		{
			const auto windowConfig = std::make_unique<Config>(Enigma::Constants::Config::WINDOW_CONFIG_FILE_PATH);
			const auto windowSettings = std::make_unique<WindowSettings>(*windowConfig);
      const auto app = std::make_unique<Application>(*windowSettings);
			app->Run();
			exit_code = EXIT_SUCCESS;
		}
	}
	catch (const std::exception& e)
	{
		ENIGMA_CRITICAL(e.what());
		exit_code = EXIT_FAILURE;
	}

	Database::Shutdown();
	Logger::Shutdown();

	return exit_code;
}

/**
*	Handles exit signals to release program resources gracefully
*/
static void SignalHandler(const int sig)
{
	const auto stringify_signal = [sig]() noexcept -> const char* 
	{
#define RET_STR(s, desc) case s: return #s ": " desc
		switch (sig) 
		{
			default: return "<unknown signal>";
			RET_STR(SIGABRT, "Abnormal termination triggered by abort call");
			RET_STR(SIGFPE, "Floating point exception");
			RET_STR(SIGILL, "Illegal instruction");
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

	if (Enigma::Application::GetInstance()) Enigma::Application::GetInstance()->~Application();
	if (Enigma::Database::GetInstance()) Enigma::Database::Shutdown();
	if (Enigma::Logger::GetLogger()) Enigma::Logger::Shutdown();

	std::exit(EXIT_SUCCESS);
}

/**
*	Windows Entry Point (for Release mode)
*	Note: even when using WinMain, release & dist are built as ConsoleApp so CLI will work properly.
*/
#if defined(ENIGMA_PLATFORM_WINDOWS) && defined(ENIGMA_RELEASE)

int WINAPI WinMain(
	[[maybe_unused]] _In_ HINSTANCE instance,
	[[maybe_unused]] _In_opt_ HINSTANCE prev_instance,
	[[maybe_unused]] _In_ LPSTR cmd_line,
	[[maybe_unused]] _In_ int cmd_show)
{
	return ::main(__argc, __argv);
}

#endif
