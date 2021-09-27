#pragma once
#ifndef ENIGMA_LOGGER_H
#define ENIGMA_LOGGER_H

#include <Core/Core.hpp>

// Logging Library
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

NS_ENIGMA_BEGIN

class ENIGMA_API Logger final
{
	ENIGMA_STATIC_CLASS(Logger);
public:
	/**
	*	Initialize Enigma Logger
	*/
	static void Initialize();

	/**
	*	Shutdown Enigma Logger (cleans up spdlog stuff, remains optional to call at the exit of the app)
	*/
	static void Shutdown();

public: /* Accessors */
	static const std::shared_ptr<spdlog::logger>& GetLogger() noexcept { return m_logger; }

private:
	inline static std::shared_ptr<spdlog::logger> m_logger{ nullptr };
};


/// Enigma Log 
#define ENIGMA_TRACE(...)				::Enigma::Logger::GetLogger()->trace(__VA_ARGS__)
#define ENIGMA_LOG(...)					::Enigma::Logger::GetLogger()->debug(__VA_ARGS__)
#define ENIGMA_INFO(...)				::Enigma::Logger::GetLogger()->info(__VA_ARGS__)
#define ENIGMA_WARN(...)				::Enigma::Logger::GetLogger()->warn("{0}\n{1}:{2}\n{3}", fmt::format(__VA_ARGS__), __FILE__, __LINE__, ENIGMA_CURRENT_FUNCTION)
#define ENIGMA_ERROR(...)				::Enigma::Logger::GetLogger()->error("{0}\n{1}:{2}\n{3}", fmt::format(__VA_ARGS__), __FILE__, __LINE__, ENIGMA_CURRENT_FUNCTION)
#define ENIGMA_CRITICAL(...)			::Enigma::Logger::GetLogger()->critical("{0}\n{1}:{2}\n{3}", fmt::format(__VA_ARGS__), __FILE__, __LINE__, ENIGMA_CURRENT_FUNCTION)

#define ENIGMA_TRACE_IF(condition, ...)    if((condition))  ENIGMA_TRACE(__VA_ARGS__);
#define ENIGMA_LOG_IF(condition, ...)      if((condition))  ENIGMA_LOG(__VA_ARGS__);
#define ENIGMA_INFO_IF(condition, ...)     if((condition))  ENIGMA_INFO(__VA_ARGS__);
#define ENIGMA_WARN_IF(condition, ...)     if((condition))  ENIGMA_WARN(__VA_ARGS__);
#define ENIGMA_ERROR_IF(condition, ...)    if((condition))  ENIGMA_ERROR(__VA_ARGS__);
#define ENIGMA_CRITICAL_IF(condition, ...) if((condition))  ENIGMA_CRITICAL(__VA_ARGS__);

#define log_trace(...)				ENIGMA_TRACE(__VA_ARGS__)
#define log_debug(...)				ENIGMA_LOG(__VA_ARGS__)
#define log_info(...)				ENIGMA_INFO(__VA_ARGS__)
#define log_warn(...)				ENIGMA_WARN(__VA_ARGS__)
#define log_error(...)				ENIGMA_ERROR(__VA_ARGS__)
#define log_critical(...)			ENIGMA_CRITICAL(__VA_ARGS__)

#define LOG_TRACE(...)				ENIGMA_TRACE(__VA_ARGS__)
#define LOG_DEBUG(...)				ENIGMA_LOG(__VA_ARGS__)
#define LOG_INFO(...)				ENIGMA_INFO(__VA_ARGS__)
#define LOG_WARN(...)				ENIGMA_WARN(__VA_ARGS__)
#define LOG_ERROR(...)				ENIGMA_ERROR(__VA_ARGS__)
#define LOG_CRITICAL(...)			ENIGMA_CRITICAL(__VA_ARGS__)

NS_ENIGMA_END

#endif // !ENIGMA_LOGGER_H
