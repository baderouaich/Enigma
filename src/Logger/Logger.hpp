#pragma once
#ifndef ENIGMA_LOGGER_H
#define ENIGMA_LOGGER_H

#include <Core/Core.hpp>

// Logging Library
#pragma warning(push, 0)
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>
#pragma warning(pop)


NS_ENIGMA_BEGIN

class Logger final {
    ENIGMA_STATIC_CLASS(Logger);

  public:
    /**
    *	Initialize Enigma Logger
    */
    static void initialize();

    /**
    *	Shutdown Enigma Logger (cleans up spdlog stuff, remains optional to call at the exit of the app)
    */
    static void shutdown();

  public: /* Accessors */
    static const std::shared_ptr<spdlog::logger>& getLogger() noexcept { return m_logger; }

  private:
    inline static std::shared_ptr<spdlog::logger> m_logger{nullptr};
};


/// Enigma Log Macros
#define ENIGMA_TRACE(...) ::Enigma::Logger::getLogger()->trace(__VA_ARGS__)
#define ENIGMA_LOG(...) ::Enigma::Logger::getLogger()->debug(__VA_ARGS__)
#define ENIGMA_INFO(...) ::Enigma::Logger::getLogger()->info(__VA_ARGS__)
#define ENIGMA_WARN(...) ::Enigma::Logger::getLogger()->warn("{}\n{}:{}\n{}", fmt::format(__VA_ARGS__), __FILE__, __LINE__, ENIGMA_CURRENT_FUNCTION)
#define ENIGMA_ERROR(...) ::Enigma::Logger::getLogger()->error("{}\n{}:{}\n{}", fmt::format(__VA_ARGS__), __FILE__, __LINE__, ENIGMA_CURRENT_FUNCTION)
#define ENIGMA_CRITICAL(...) ::Enigma::Logger::getLogger()->critical("{}\n{}:{}\n{}", fmt::format(__VA_ARGS__), __FILE__, __LINE__, ENIGMA_CURRENT_FUNCTION)

#define ENIGMA_TRACE_IF(condition, ...) \
  if ((condition)) ENIGMA_TRACE(__VA_ARGS__);
#define ENIGMA_LOG_IF(condition, ...) \
  if ((condition)) ENIGMA_LOG(__VA_ARGS__);
#define ENIGMA_INFO_IF(condition, ...) \
  if ((condition)) ENIGMA_INFO(__VA_ARGS__);
#define ENIGMA_WARN_IF(condition, ...) \
  if ((condition)) ENIGMA_WARN(__VA_ARGS__);
#define ENIGMA_ERROR_IF(condition, ...) \
  if ((condition)) ENIGMA_ERROR(__VA_ARGS__);
#define ENIGMA_CRITICAL_IF(condition, ...) \
  if ((condition)) ENIGMA_CRITICAL(__VA_ARGS__);

#define log_trace(...) ENIGMA_TRACE(__VA_ARGS__)
#define log_debug(...) ENIGMA_LOG(__VA_ARGS__)
#define log_info(...) ENIGMA_INFO(__VA_ARGS__)
#define log_warn(...) ENIGMA_WARN(__VA_ARGS__)
#define log_error(...) ENIGMA_ERROR(__VA_ARGS__)
#define log_critical(...) ENIGMA_CRITICAL(__VA_ARGS__)

#define log_trace_if(...) \
  if ((condition)) ENIGMA_TRACE(__VA_ARGS__);
#define log_debug_if(...) \
  if ((condition)) ENIGMA_LOG(__VA_ARGS__);
#define log_info_if(...) \
  if ((condition)) ENIGMA_INFO(__VA_ARGS__);
#define log_warn_if(...) \
  if ((condition)) ENIGMA_WARN(__VA_ARGS__);
#define log_error_if(...) \
  if ((condition)) ENIGMA_ERROR(__VA_ARGS__);
#define log_critical_if(...) \
  if ((condition)) ENIGMA_CRITICAL(__VA_ARGS__);

#define LOG_TRACE(...) ENIGMA_TRACE(__VA_ARGS__)
#define LOG_DEBUG(...) ENIGMA_LOG(__VA_ARGS__)
#define LOG_INFO(...) ENIGMA_INFO(__VA_ARGS__)
#define LOG_WARN(...) ENIGMA_WARN(__VA_ARGS__)
#define LOG_ERROR(...) ENIGMA_ERROR(__VA_ARGS__)
#define LOG_CRITICAL(...) ENIGMA_CRITICAL(__VA_ARGS__)

#define LOG_TRACE_IF(condition, ...) \
  if ((condition)) ENIGMA_TRACE(__VA_ARGS__);
#define LOG_LOG_IF(condition, ...) \
  if ((condition)) ENIGMA_LOG(__VA_ARGS__);
#define LOG_INFO_IF(condition, ...) \
  if ((condition)) ENIGMA_INFO(__VA_ARGS__);
#define LOG_WARN_IF(condition, ...) \
  if ((condition)) ENIGMA_WARN(__VA_ARGS__);
#define LOG_ERROR_IF(condition, ...) \
  if ((condition)) ENIGMA_ERROR(__VA_ARGS__);
#define LOG_CRITICAL_IF(condition, ...) \
  if ((condition)) ENIGMA_CRITICAL(__VA_ARGS__);


NS_ENIGMA_END

#endif // !ENIGMA_LOGGER_H
