#include <pch.hpp>
#include "Logger.hpp"

NS_ENIGMA_BEGIN

// static members
SharedPtr<spdlog::logger> Logger::m_logger;

void Logger::Initialize()
{
	std::array<SharedPtr<spdlog::sinks::sink>, 2> log_sinks; 

	// console logger
	log_sinks[0] = Enigma::MakeShared<spdlog::sinks::stdout_color_sink_mt>();
	// file logger
	log_sinks[1] = Enigma::MakeShared<spdlog::sinks::basic_file_sink_mt>("Enigma.log", true);

	// set pattern of the console logger
	log_sinks[0]->set_pattern("%^[%T] %n: %v%$");
	// set pattern of the file logger
	log_sinks[1]->set_pattern("[%T] [%l] %n: %v");

	//Engine Logger
	m_logger = Enigma::MakeShared<spdlog::logger>("Enigma", log_sinks.begin() , log_sinks.end());
	spdlog::register_logger(m_logger);
	m_logger->set_level(spdlog::level::trace);
	m_logger->flush_on(spdlog::level::trace);
}

NS_ENIGMA_END

