#pragma once
#ifndef ENIGMA_SIZE_UTILS_H
#define ENIGMA_SIZE_UTILS_H

#include <Core/Core.hpp>
#include <sstream>	// std::ostringstream
#include <iomanip>	// std::setprecision
#include <cmath>	// std::log, std::floor

NS_ENIGMA_BEGIN

class ENIGMA_API SizeUtils
{
public:
	/*
	*	Converts bytes to human friendly readable format, e.g (100 KB, 30 Bytes, 5.23 MB...)
	*/
	static String FriendlySize(const size_t bytes) noexcept
	{
		constexpr const i32 KB = 1024;

		if (bytes < KB)  
			return std::to_string(bytes) + " Bytes";

		std::ostringstream oss{};
		constexpr const char* sizes[]{ "Bytes", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB" };
		const size_t i = static_cast<size_t>(std::floor(std::log(bytes) / std::log(KB)));

		oss << std::fixed
			<< std::setprecision(2)
			<< static_cast<f32>(bytes / std::pow(KB, i))
			<< ' '
			<< sizes[i];

		return oss.str();

#if 0
		if (bytes == 0)  return String("0 Bytes");
		
		std::ostringstream oss{};
		constexpr const i32 KB = 1024;
		constexpr const char* sizes[]{ "Bytes", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB" };
		const size_t i = static_cast<size_t>(std::floor(std::log(bytes) / std::log(KB)));
		
		oss << std::fixed
			<< std::setprecision(2)
			<< static_cast<f32>(bytes / std::pow(KB, i)) 
			<< ' ' 
			<< sizes[i];

		return oss.str();
#endif
	}

};

/// Convert bytes to kb, mb, gb, tb
#define ENIGMA_BYTES_TO_KB(bytes) (static_cast<::Enigma::f32>(bytes) / 1024.0f)
#define ENIGMA_BYTES_TO_MB(bytes) (static_cast<::Enigma::f32>(bytes) / 1024.0f / 1024.0f)
#define ENIGMA_BYTES_TO_GB(bytes) (static_cast<::Enigma::f32>(bytes) / 1024.0f / 1024.0f / 1024.0f)
#define ENIGMA_BYTES_TO_TB(bytes) (static_cast<::Enigma::f32>(bytes) / 1024.0f / 1024.0f / 1024.0f / 1024.0f)

/// Helper operators for compile time inline conversion e.g( size_t three_mb = 3_MB; ...)
constexpr size_t operator""_KB(const size_t N) noexcept
{
	return N * 1024;
}
constexpr size_t operator""_MB(const size_t N) noexcept
{
	return N * 1024_KB;
}
constexpr size_t operator""_GB(const size_t N) noexcept
{
	return N * 1024_MB;
}
constexpr size_t operator""_TB(const size_t N) noexcept
{
	return N * 1024_GB;
}
constexpr size_t operator""_PB(const size_t N) noexcept
{
	return N * 1024_TB;
}
constexpr size_t operator""_EB(const size_t N) noexcept
{
	return N * 1024_PB;
}
constexpr size_t operator""_ZB(const size_t N) noexcept
{
	return N * 1024_EB;
}
constexpr size_t operator""_YB(const size_t N) noexcept
{
	return N * 1024_ZB;
}

NS_ENIGMA_END

#endif // !ENIGMA_SIZE_UTILS_H
