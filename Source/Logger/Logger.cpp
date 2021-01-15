#include <pch.hpp>
#include "Logger.hpp"

NS_ENIGMA_BEGIN

// static members
std::shared_ptr<spdlog::logger> Logger::m_logger;

void Logger::Initialize()
{
	std::array<std::shared_ptr<spdlog::sinks::sink>, 2> log_sinks;

	// console logger
	log_sinks[0] = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	// file logger
	log_sinks[1] = std::make_shared<spdlog::sinks::basic_file_sink_mt>("Enigma.log", true);

	// set pattern of the console logger
	log_sinks[0]->set_pattern("%^[%T] %n: %v%$"); // regex like pattern output format https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
	// set pattern of the file logger
	log_sinks[1]->set_pattern("[%Y-%m-%d %T] [%l] %n: %v"); // [2021-10-31 23:46:59.678] [trace] Enigma: Some message
	//log_sinks[1]->set_pattern("[%T] [%l] %n: %v");
	//log_sinks[1]->set_pattern("%+"); // spdlog's default format	"[2014-10-31 23:46:59.678] [mylogger] [info] Some message"

	//Engine Logger
	m_logger = std::make_shared<spdlog::logger>("Enigma", log_sinks.begin() , log_sinks.end());
	spdlog::register_logger(m_logger);
	m_logger->set_level(spdlog::level::trace);
	m_logger->flush_on(spdlog::level::trace);
}

NS_ENIGMA_END

