#pragma once
#ifndef ENIGMA_LOGGER_H
#define ENIGMA_LOGGER_H

#include <Core/Core.hpp>

//Logging Library
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/basic_file_sink.h>

NS_ENIGMA_BEGIN
class ENIGMA_API Logger
{
public:
	/*
	*	Initialize Enigma Logger
	*/
	static void Initialize();

public: /* Accessors */
	static const SharedPtr<spdlog::logger>& GetCoreLogger() noexcept { return m_core_logger; }
	static const SharedPtr<spdlog::logger>& GetClientLogger() noexcept { return m_client_logger; }

private:
    static SharedPtr<spdlog::logger> m_core_logger;
	static SharedPtr<spdlog::logger> m_client_logger;
};


/// Engine Core Log Macros
#define ENIGMA_CORE_TRACE(...)			::Enigma::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define ENIGMA_CORE_INFO(...)			::Enigma::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define ENIGMA_CORE_WARN(...)			::Enigma::Logger::GetCoreLogger()->warn("{0} | {1}:{2} | {3}", fmt::format(__VA_ARGS__), __FILE__, __LINE__, ENIGMA_CURRENT_FUNCTION)
#define ENIGMA_CORE_ERROR(...)			::Enigma::Logger::GetCoreLogger()->error("{0} | {1}:{2} | {3}", fmt::format(__VA_ARGS__), __FILE__, __LINE__, ENIGMA_CURRENT_FUNCTION)
#define ENIGMA_CORE_CRITICAL(...)		::Enigma::Logger::GetCoreLogger()->critical("{0} | {1}:{2} | {3}", fmt::format(__VA_ARGS__), __FILE__, __LINE__, ENIGMA_CURRENT_FUNCTION)

/// Client Log Macros
#define ENIGMA_TRACE(...)				::Enigma::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define ENIGMA_INFO(...)				::Enigma::Logger::GetClientLogger()->info(__VA_ARGS__)
#define ENIGMA_WARN(...)				::Enigma::Logger::GetClientLogger()->warn("{0} | {1}:{2} | {3}", fmt::format(__VA_ARGS__), __FILE__, __LINE__, ENIGMA_CURRENT_FUNCTION)
#define ENIGMA_ERROR(...)				::Enigma::Logger::GetClientLogger()->error("{0} | {1}:{2} | {3}", fmt::format(__VA_ARGS__), __FILE__, __LINE__, ENIGMA_CURRENT_FUNCTION)
#define ENIGMA_CRITICAL(...)			::Enigma::Logger::GetClientLogger()->critical("{0} | {1}:{2} | {3}", fmt::format(__VA_ARGS__), __FILE__, __LINE__, ENIGMA_CURRENT_FUNCTION)


/// Lowercase logging Style
#define log_core_trace(...)			ENIGMA_CORE_TRACE(__VA_ARGS__)
#define log_core_info(...)			ENIGMA_CORE_INFO(__VA_ARGS__)
#define log_core_warn(...)			ENIGMA_CORE_WARN(__VA_ARGS__)
#define log_core_error(...)			ENIGMA_CORE_ERROR(__VA_ARGS__)
#define log_core_critical(...)		ENIGMA_CORE_CRITICAL(__VA_ARGS__)

#define log_trace(...)				ENIGMA_TRACE(__VA_ARGS__)
#define log_info(...)				ENIGMA_INFO(__VA_ARGS__)
#define log_warn(...)				ENIGMA_WARN(__VA_ARGS__)
#define log_error(...)				ENIGMA_ERROR(__VA_ARGS__)
#define log_critical(...)			ENIGMA_CRITICAL(__VA_ARGS__)


/// Common logging style
#define LOG_CORE_TRACE(...)			ENIGMA_CORE_TRACE(__VA_ARGS__)
#define LOG_CORE(...)				ENIGMA_CORE_TRACE(__VA_ARGS__)
#define LOG_CORE_INFO(...)			ENIGMA_CORE_INFO(__VA_ARGS__)
#define LOG_CORE_WARN(...)			ENIGMA_CORE_WARN(__VA_ARGS__)
#define LOG_CORE_ERROR(...)			ENIGMA_CORE_ERROR(__VA_ARGS__)
#define LOG_CORE_CRITICAL(...)		ENIGMA_CORE_CRITICAL(__VA_ARGS__)

#define LOG(...)					ENIGMA_TRACE(__VA_ARGS__)
#define LOG_TRACE(...)				ENIGMA_TRACE(__VA_ARGS__)
#define LOG_INFO(...)				ENIGMA_INFO(__VA_ARGS__)
#define LOG_WARN(...)				ENIGMA_WARN(__VA_ARGS__)
#define LOG_ERROR(...)				ENIGMA_ERROR(__VA_ARGS__)
#define LOG_CRITICAL(...)			ENIGMA_CRITICAL(__VA_ARGS__)

NS_ENIGMA_END

#endif // !ENIGMA_LOGGER_H
