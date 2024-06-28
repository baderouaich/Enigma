#include <pch.hpp>
#include "Base64.hpp"
#include <base64.h> // Crypto++'s Base64 Encode/Decoder

NS_ENIGMA_BEGIN

std::string Base64::Encode(const std::string_view& buffer)
{
	std::string encoded{};

	const auto encoder = std::make_unique<CryptoPP::Base64Encoder>();
	encoder->Put(reinterpret_cast<const byte*>(buffer.data()), buffer.size());
	encoder->MessageEnd();

	const CryptoPP::word64 size = encoder->MaxRetrievable();
	if (size)
	{
		encoded.resize(size, '\000');
		encoder->Get(reinterpret_cast<byte*>(&encoded[0]), encoded.size());
	}
	
	return encoded;
}

std::string Base64::Decode(const std::string_view& encoded_buffer)
{
	std::string decoded{};

	const auto decoder = std::make_unique<CryptoPP::Base64Decoder>();
	decoder->Put(reinterpret_cast<const byte*>(encoded_buffer.data()), encoded_buffer.size());
	decoder->MessageEnd();

	const CryptoPP::word64 size = decoder->MaxRetrievable();
	if (size && size <= std::numeric_limits<decltype(size)>::max())
	{
		decoded.resize(size, '\000');
		decoder->Get(reinterpret_cast<byte*>(&decoded[0]), decoded.size());
	}

	return decoded;
}

NS_ENIGMA_END

