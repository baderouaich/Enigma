#pragma once
#ifndef ENIGMA_SIZE_UTILS_H
#define ENIGMA_SIZE_UTILS_H

#include <Core/Core.hpp>
#include <sstream>	// std::ostringstream
#include <iomanip>	// std::setprecision
#include <cmath>	// std::log, std::floor

NS_ENIGMA_BEGIN

class ENIGMA_API SizeUtils final
{
	ENIGMA_STATIC_CLASS(SizeUtils);
public:
	/*
	*	Converts bytes to human friendly readable format, e.g (100 KB, 30 Bytes, 5.23 MB...)
	*/
	static String FriendlySize(const size_t bytes) noexcept
	{
		constexpr const size_t KB = 1024;

		if (bytes < KB)  
			return std::to_string(bytes) + " Bytes";

		constexpr const char* sizes[]{ "Bytes", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB" };
		const size_t i = static_cast<size_t>(std::floor(static_cast<f64>(std::log(bytes) / std::log(KB))));

		std::ostringstream oss{};
		oss << std::fixed
			<< std::setprecision(2)
			<< static_cast<f32>(bytes / std::pow(KB, i))
			<< ' '
			<< sizes[i];

		return oss.str();
	}

};

/// Convert bytes to kb, mb, gb, tb
#define ENIGMA_BYTES_TO_KB(bytes) (static_cast<::Enigma::f32>(bytes) / 1024.0f)
#define ENIGMA_BYTES_TO_MB(bytes) (static_cast<::Enigma::f32>(bytes) / 1024.0f / 1024.0f)
#define ENIGMA_BYTES_TO_GB(bytes) (static_cast<::Enigma::f32>(bytes) / 1024.0f / 1024.0f / 1024.0f)
#define ENIGMA_BYTES_TO_TB(bytes) (static_cast<::Enigma::f32>(bytes) / 1024.0f / 1024.0f / 1024.0f / 1024.0f)

/// Convert mb to bytes, kb, gb, tb
#define ENIGMA_MB_TO_BYTES(mb) (static_cast<::Enigma::size_t>(mb) * 1024 * 1024)
#define ENIGMA_MB_TO_KB(mb)    (static_cast<::Enigma::size_t>(mb) * 1024)
#define ENIGMA_MB_TO_GB(mb)    (static_cast<::Enigma::f32>(mb) / 1024.0f)
#define ENIGMA_MB_TO_TB(mb)    (static_cast<::Enigma::f32>(mb) / 1024.0f / 1024.0f)

NS_ENIGMA_END

#endif // !ENIGMA_SIZE_UTILS_H
