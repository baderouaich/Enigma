#include <pch.hpp>
#include "Base64.hpp"
#include <base64.h> // Crypto++'s Base64 Encode/Decoder


NS_ENIGMA_BEGIN

String Base64::Encode(const std::string_view& buffer)
{
	String encoded{};
	const auto encoder = std::make_unique<CryptoPP::Base64Encoder>();
	encoder->Put(reinterpret_cast<const byte*>(buffer.data()), buffer.size());
	encoder->MessageEnd();

	const CryptoPP::word64 size = encoder->MaxRetrievable();
	if (size)
	{
		encoded.resize(size);
		encoder->Get((byte*)&encoded[0], encoded.size());
	}
	
	return encoded;
}

String Base64::Decode(const std::string_view& encoded_buffer)
{
	String decoded{};

	const auto decoder = std::make_unique<CryptoPP::Base64Decoder>();
	decoder->Put(reinterpret_cast<const byte*>(encoded_buffer.data()), encoded_buffer.size());
	decoder->MessageEnd();

	const CryptoPP::word64 size = decoder->MaxRetrievable();
	if (size && size <= std::numeric_limits<decltype(size)>::max())
	{
		decoded.resize(size);
		decoder->Get((byte*)&decoded[0], decoded.size());
	}

	return decoded;
}

inline const bool Base64::IsBase64(const byte& c) noexcept
{
	return !!std::isalnum(c) || (c == '+') || (c == '/');
}

const bool Base64::IsBase64(const std::string_view& str) noexcept
{
	for (const char& c : str)
	{
		if (!IsBase64(static_cast<const byte&>(c)))
		{
			return false;
		}
	}
	return true;
}

NS_ENIGMA_END

