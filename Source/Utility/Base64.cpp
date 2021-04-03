#include <pch.hpp>
#include "Base64.hpp"
#include <base64.h> // Crypto++'s Base64 Encode/Decoder


NS_ENIGMA_BEGIN

String Base64::Encode(const std::string_view& buffer)
{
	String encoded{};
	CryptoPP::StringSource ss(buffer.data(),
		true,
		new CryptoPP::Base64Encoder(new CryptoPP::StringSink(encoded)
		)
	);
	return encoded;
}

String Base64::Decode(const std::string_view& encoded_buffer)
{
	String decoded{};
	CryptoPP::StringSource ss(encoded_buffer.data(), true,
		new CryptoPP::Base64Decoder(new CryptoPP::StringSink(decoded)));
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

