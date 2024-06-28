#pragma once
#ifndef ENIGMA_BASE64_H
#define ENIGMA_BASE64_H

#include <Core/Core.hpp>

NS_ENIGMA_BEGIN
class ENIGMA_API Base64 final
{
	ENIGMA_STATIC_CLASS(Base64);
public:
	/*
	*	Encodes string bytes into Base64 string
	* https://www.cryptopp.com/wiki/Base64Encoder
	*/
	static std::string Encode(const std::string_view& buffer);

	/*
	* Decodes a Base64 string
	* https://www.cryptopp.com/wiki/Base64Decoder
	*/
	static std::string Decode(const std::string_view& encoded_buffer);
};
NS_ENIGMA_END

#endif // !ENIGMA_BASE64_H
