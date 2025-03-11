#pragma once

#if defined(_MSC_VER)
#include <stdlib.h>
#endif

#include <cstdint>
#include <bit>

#ifdef __has_builtin
#define ENIGMA_HAVE_BUILTIN(x) __has_builtin(x)
#else
#define ENIGMA_HAVE_BUILTIN(x) false
#endif

namespace Enigma {

  constexpr std::uint64_t BSwap64(std::uint64_t host_int) {
#if defined(__GNUC__) || ENIGMA_HAVE_BUILTIN(__builtin_bswap64)
    return __builtin_bswap64(host_int);
#elif defined(_MSC_VER)
    return _byteswap_uint64(host_int);
#else
    return (((host_int & std::uint64_t{0xFF}) << 56) |
            ((host_int & std::uint64_t{0xFF00}) << 40) |
            ((host_int & std::uint64_t{0xFF0000}) << 24) |
            ((host_int & std::uint64_t{0xFF000000}) << 8) |
            ((host_int & std::uint64_t{0xFF00000000}) >> 8) |
            ((host_int & std::uint64_t{0xFF0000000000}) >> 24) |
            ((host_int & std::uint64_t{0xFF000000000000}) >> 40) |
            ((host_int & std::uint64_t{0xFF00000000000000}) >> 56));
#endif
  }

  constexpr std::uint32_t BSwap32(std::uint32_t host_int) {
#if defined(__GNUC__) || ENIGMA_HAVE_BUILTIN(__builtin_bswap32)
    return __builtin_bswap32(host_int);
#elif defined(_MSC_VER)
    return _byteswap_ulong(host_int);
#else
    return (((host_int & std::uint32_t{0xFF}) << 24) |
            ((host_int & std::uint32_t{0xFF00}) << 8) |
            ((host_int & std::uint32_t{0xFF0000}) >> 8) |
            ((host_int & std::uint32_t{0xFF000000}) >> 24));
#endif
  }

  constexpr std::uint16_t BSwap16(std::uint16_t host_int) {
#if defined(__GNUC__) || ENIGMA_HAVE_BUILTIN(__builtin_bswap16)
    return __builtin_bswap16(host_int);
#elif defined(_MSC_VER)
    return _byteswap_ushort(host_int);
#else
    return (((host_int & std::uint16_t{0xFF}) << 8) |
            ((host_int & std::uint16_t{0xFF00}) >> 8));
#endif
  }

  namespace LittleEndian {

    constexpr std::uint16_t fromHost(std::uint16_t value) {
      if constexpr (std::endian::native == std::endian::big) {
        return BSwap16(value);
      } else {
        return value;
      }
    }

    constexpr std::uint32_t fromHost(std::uint32_t value) {
      if constexpr (std::endian::native == std::endian::big) {
        return BSwap32(value);
      } else {
        return value;
      }
    }

    constexpr std::uint64_t fromHost(std::uint64_t value) {
      if constexpr (std::endian::native == std::endian::big) {
        return BSwap64(value);
      } else {
        return value;
      }
    }

    constexpr std::uint16_t toHost(std::uint16_t value) {
      if constexpr (std::endian::native == std::endian::big) {
        return BSwap16(value);
      } else {
        return value;
      }
    }

    constexpr std::uint32_t toHost(std::uint32_t value) {
      if constexpr (std::endian::native == std::endian::big) {
        return BSwap32(value);
      } else {
        return value;
      }
    }

    constexpr std::uint64_t toHost(std::uint64_t value) {
      if constexpr (std::endian::native == std::endian::big) {
        return BSwap64(value);
      } else {
        return value;
      }
    }

  } // namespace LittleEndian

  //[[maybe_unused]]
  namespace BigEndian {
    constexpr std::uint16_t fromHost(std::uint16_t value) {
      if constexpr (std::endian::native == std::endian::big) {
        return value;
      } else {
        return BSwap16(value);
      }
    }

    constexpr std::uint32_t fromHost(std::uint32_t value) {
      if constexpr (std::endian::native == std::endian::big) {
        return value;
      } else {
        return BSwap32(value);
      }
    }

    constexpr std::uint64_t fromHost(std::uint64_t value) {
      if constexpr (std::endian::native == std::endian::big) {
        return value;
      } else {
        return BSwap64(value);
      }
    }

    constexpr std::uint16_t toHost(std::uint16_t value) {
      if constexpr (std::endian::native == std::endian::big) {
        return value;
      } else {
        return BSwap16(value);
      }
    }

    constexpr std::uint32_t toHost(std::uint32_t value) {
      if constexpr (std::endian::native == std::endian::big) {
        return value;
      } else {
        return BSwap32(value);
      }
    }

    constexpr std::uint64_t toHost(std::uint64_t value) {
      if constexpr (std::endian::native == std::endian::big) {
        return value;
      } else {
        return BSwap64(value);
      }
    }

  } // namespace BigEndian

} // namespace Enigma
