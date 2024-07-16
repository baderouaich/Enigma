#include <pch.hpp>
#include <Enigma.hpp>

static void SignalHandler(int sig);

int main(int argc, char *argv[]) {
  if (argc > 1 && (!std::strcmp(argv[1], "--version") || !std::strcmp(argv[1], "-v"))) {
    std::cout << ENIGMA_VERSION << std::endl;
    return EXIT_SUCCESS;
  }

  // Handle abnormal exists to normally end program and release resources gracefully
  std::signal(SIGABRT, SignalHandler); // Abnormal termination triggered by abort call
  std::signal(SIGFPE, SignalHandler);  // Floating point exception
  std::signal(SIGILL, SignalHandler);  // Illegal instruction - invalid function image
  std::signal(SIGINT, SignalHandler);  // Interrupt e.g console CTRL+C
  std::signal(SIGSEGV, SignalHandler); // Segment violation (nullptr read access violation exception...)
  std::signal(SIGTERM, SignalHandler); // Software termination signal from kill
#if defined(ENIGMA_PLATFORM_WINDOWS)
  std::signal(SIGBREAK, SignalHandler); // On Windows, a click on console window close button will raise SIGBREAK
#endif

  Enigma::Logger::initialize();
  Enigma::Database::initialize();

  std::int32_t exit_code = -1;
  try {
    const Enigma::Config windowConfig(Enigma::Constants::Config::WINDOW_CONFIG_FILE_PATH);
    const Enigma::WindowSettings windowSettings(windowConfig);
    Enigma::Application app(windowSettings);
    app.Run();
    exit_code = EXIT_SUCCESS;
  } catch (const std::exception& e) {
    ENIGMA_CRITICAL(e.what());
    exit_code = EXIT_FAILURE;
  }

  Enigma::Database::shutdown();
  Enigma::Logger::shutdown();

  return exit_code;
}

/**
*	Handles exit signals to release program resources gracefully
*/
static void SignalHandler(int sig) {
  const auto stringify_signal = [sig]() noexcept -> const char * {

#define RET_STR(s, desc) \
  case s:                \
    return #s ": " desc
    switch (sig) {
      default:
        return "<unknown signal>";
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

  if (Enigma::Logger::getLogger()) {
    ENIGMA_INFO("Signal handler invoked due abnormal exit with signal: {} ({})", sig, stringify_signal());
    ENIGMA_INFO("Exiting gracefully...");
  }

  /** Note:
   * When a signal is raised, destructors will not be called..
   * sadly we must cleanup manually in this case
   */
  if (Enigma::Application::getInstance()) Enigma::Application::getInstance()->~Application();
  if (Enigma::Database::getStorage()) Enigma::Database::shutdown();
  if (Enigma::Logger::getLogger()) Enigma::Logger::shutdown();

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
  [[maybe_unused]] _In_ int cmd_show) {
  return ::main(__argc, __argv);
}

#endif
