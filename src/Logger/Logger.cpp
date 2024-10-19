#include <pch.hpp>
#include "Logger.hpp"

#include <Utility/DateTimeUtils.hpp>
#include <Utility/FileUtils.hpp>

NS_ENIGMA_BEGIN

void Logger::initialize() {
  // Make 2 log sinks, Console and File.
  std::array<std::shared_ptr<spdlog::sinks::sink>, 2> log_sinks{};

  // Console Logger
  log_sinks[0] = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

  // File Logger
  //log_sinks[1] = std::make_shared<spdlog::sinks::basic_file_sink_mt>(Constants::Logger::LOG_FILE_PATH, true);
  const fs::path logsDir = ResourceManager::getLogsDir();
  const fs::path logFilepath = logsDir / DateTimeUtils::now("Enigma-%Y%m%d%H%M%S.log");
  log_sinks[1] = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilepath.string(), true);

  // Set pattern of the console logger => [time] [trace/info/debug...]: msg]
  log_sinks[0]->set_pattern("%^[%T] [%l]: %v%$"); // regex like pattern output format https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
  //log_sinks[0]->set_pattern("%^[%T] %n: %v%$"); // regex like pattern output format https://github.com/gabime/spdlog/wiki/3.-Custom-formatting

  // Set pattern of the file logger (more detailed for debugging) => [date time am/pm] [thread id] [trace/info/debug...]: msg]
  log_sinks[1]->set_pattern("%^[%Y-%m-%d %T %p] [%t] [%l]: %v%$"); // [2021-10-31 23:46:59.678] [trace]: Some message
  //log_sinks[1]->set_pattern("[%Y-%m-%d %T] [%l] %n: %v"); // [2021-10-31 23:46:59.678] [trace] Enigma: Some message
  //log_sinks[1]->set_pattern("[%T] [%l] %n: %v");
  //log_sinks[1]->set_pattern("%+"); // spdlog's default format	"[2021-10-31 23:46:59.678] Enigma [info] Some message"

  // Create & Register Logger
  m_logger = std::make_shared<spdlog::logger>("Enigma", log_sinks.begin(), log_sinks.end());
  spdlog::register_logger(m_logger);

  // Set Log level
  m_logger->set_level(spdlog::level::trace);

  // Flush logs on trace
  m_logger->flush_on(spdlog::level::trace);

  ENIGMA_TRACE_CURRENT_FUNCTION();
#if defined(ENIGMA_DEBUG)
  ENIGMA_INFO("Log File Path: {0}", logFilepath.string());
#endif
}

void Logger::shutdown() {
  ENIGMA_TRACE_CURRENT_FUNCTION();

  spdlog::shutdown();
}

NS_ENIGMA_END
