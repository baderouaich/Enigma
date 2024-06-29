#pragma once
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

namespace Enigma {
  /// @brief Namespace contains useful Date Time related utility functions.
  namespace DateTimeUtils {
    /// @brief Converts an std::time_t to a string date time with a specific format
    /// @param time std::time_t value to convert
    /// @param format Format of the date to return. Default %Y-%m-%d %H:%M:%S (e.g 2023-10-25 12:55:05) https://www.gnu.org/software/libc/manual/html_node/Formatting-Calendar-Time.html
    /// @returns time as string date time with the specific format
    [[nodiscard]] static std::string toString(const std::time_t& time, const std::string_view& format = "%Y-%m-%d %H:%M:%S") {
      char buffer[128]{};
      tm tm_{};
#ifdef _WIN32
      localtime_s(&tm_, &time);
#else
      tm_ = *std::localtime(&time);
#endif
      std::strftime(buffer, sizeof(buffer), format.data(), &tm_);
      return std::string{buffer};
    }

    /// @brief Converts a string date time with a specific format to an std::time_t
    /// @param dateTimeStr String date time to convert
    /// @param format Format of the string date to return. Default %Y-%m-%d %H:%M:%S (e.g 2023-10-25 12:55:05) https://www.gnu.org/software/libc/manual/html_node/Formatting-Calendar-Time.html
    /// @returns std::time_t
    [[nodiscard]] static std::time_t fromString(const std::string& dateTimeStr, const std::string_view& format = "%Y-%m-%d %H:%M:%S") {
      std::tm tm{};
      std::istringstream iss{dateTimeStr};
      iss >> std::get_time(&tm, format.data());
      return std::mktime(&tm);
    }

    /// @brief returns current date and time as a string with a specific format
    /// @param format Format of the date to return. Default %Y-%m-%d %H:%M:%S (e.g 2023-10-25 12:55:05) https://www.gnu.org/software/libc/manual/html_node/Formatting-Calendar-Time.html
    /// @returns current date time string
    [[nodiscard]] static std::string now(const std::string_view& format = "%Y-%m-%d %H:%M:%S") {
      const std::time_t tm = std::time(nullptr);
      return toString(tm, format);
    }
  }
}