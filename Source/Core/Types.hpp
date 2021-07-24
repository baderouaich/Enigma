#pragma once
#ifndef ENIGMA_TYPES_H
#define ENIGMA_TYPES_H

#include <cstdint>	//	std::int*_t
#include <cstddef>	//	std::size_t
#include <string>	//	std::string, std::string_view
#include <sstream>	//	std::stringstream
#include <imgui.h>	//	ImVec4, ImVec2

namespace Enigma
{
	using i8	= std::int8_t;
	using i16	= std::int16_t;
	using i32	= std::int32_t;
	using i64	= std::int64_t;
	using ui8	= std::uint8_t;
	using ui16	= std::uint16_t;
	using ui32	= std::uint32_t;
	using ui64	= std::uint64_t;
	using f32	= float;
	using f64	= double;
	using lf64	= long double;
	using byte	= ui8;
	using size_t	= std::size_t;
	using String	= std::string;	// UTF-8 string (std::uint8_t)
	using WString	= std::wstring;	// UTF-16 string (std::uint16_t)


	using Color4f	= ImVec4;
	using Vec2f		= ImVec2;
}

#endif // !ENIGMA_TYPES_H
