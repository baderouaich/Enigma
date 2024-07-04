#pragma once
#ifndef ENIGMA_SIZE_UTILS_H
#define ENIGMA_SIZE_UTILS_H

#include <Core/Core.hpp>
#include <cmath>   // std::log, std::floor
#include <iomanip> // std::setprecision
#include <sstream> // std::ostringstream

NS_ENIGMA_BEGIN

class SizeUtils final {
    ENIGMA_STATIC_CLASS(SizeUtils);

  public:
    /*
	*	Converts bytes to human friendly readable format, e.g (100 KB, 30 Bytes, 5.23 MB...)
	*/
    static std::string FriendlySize(const std::size_t bytes) noexcept {
      constexpr const std::size_t KB = 1024;

      if (bytes < KB)
        return std::to_string(bytes) + " Bytes";

      constexpr const char *sizes[]{"Bytes", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};
      const std::size_t i = static_cast<std::size_t>(std::floor(static_cast<double>(std::log(bytes) / std::log(KB))));

      std::ostringstream oss{};
      oss << std::fixed
          << std::setprecision(2)
          << static_cast<float>(bytes / std::pow(KB, i))
          << ' '
          << sizes[i];

      return oss.str();
    }
};

/// Convert bytes to kb, mb, gb, tb
#define ENIGMA_BYTES_TO_KB(bytes) (static_cast<float>(bytes) / 1024.0f)
#define ENIGMA_BYTES_TO_MB(bytes) (static_cast<float>(bytes) / 1024.0f / 1024.0f)
#define ENIGMA_BYTES_TO_GB(bytes) (static_cast<float>(bytes) / 1024.0f / 1024.0f / 1024.0f)
#define ENIGMA_BYTES_TO_TB(bytes) (static_cast<float>(bytes) / 1024.0f / 1024.0f / 1024.0f / 1024.0f)

/// Convert mb to bytes, kb, gb, tb
#define ENIGMA_MB_TO_BYTES(mb) (static_cast<std::size_t>(mb) * 1024 * 1024)
#define ENIGMA_MB_TO_KB(mb) (static_cast<std::size_t>(mb) * 1024)
#define ENIGMA_MB_TO_GB(mb) (static_cast<float>(mb) / 1024.0f)
#define ENIGMA_MB_TO_TB(mb) (static_cast<float>(mb) / 1024.0f / 1024.0f)

#define ENIGMA_KB_TO_BYTES(kb) (static_cast<std::size_t>(kb) * 1024)

NS_ENIGMA_END

#endif // !ENIGMA_SIZE_UTILS_H
