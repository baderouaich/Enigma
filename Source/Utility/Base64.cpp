#include <pch.hpp>
#include "Base64.hpp"


NS_ENIGMA_BEGIN

String Base64::Decode(const StringView& encoded_buffer)
{
	using Constants::Utility::BASE64_CHARACTERS;

	const size_t in_length = encoded_buffer.size();
	// Should be enough to store 6-bit buffers in 8-bit buffers
	const size_t out_length = ((in_length * 3) / 4) + 1;
	String output(out_length, '\000');

	static byte inalphabet[256]{'\000'}, decoder[256]{'\000'};
	i32 i, bits, c = 0, i8_count; //, errors = 0;
	ui32 input_idx = 0;
	ui32 output_idx = 0;

	for (i = sizeof(BASE64_CHARACTERS) - 1; i >= 0; i--)
	{
		inalphabet[BASE64_CHARACTERS[i]] = 1;
		decoder[BASE64_CHARACTERS[i]] = static_cast<byte>(i);
	}

	i8_count = 0;
	bits = 0;
	for (input_idx = 0; input_idx < in_length; input_idx++)
	{
		c = encoded_buffer[input_idx];
		if (c == '=')
			break;
		if (c > 255 || !inalphabet[c])
			continue;
		bits += decoder[c];
		i8_count++;
		if (i8_count == 4)
		{
			output[output_idx++] = (bits >> 16);
			output[output_idx++] = ((bits >> 8) & 0xff);
			output[output_idx++] = (bits & 0xff);
			bits = 0;
			i8_count = 0;
		}
		else
		{
			bits <<= 6;
		}
	}

	if (c == '=')
	{
		switch (i8_count)
		{
		case 1:
			//errors++;
			//break;
			ENIGMA_CORE_ERROR("Base64 Decode invalid character");
		case 2:
			output[output_idx++] = (bits >> 10);
			break;
		case 3:
			output[output_idx++] = (bits >> 16);
			output[output_idx++] = ((bits >> 8) & 0xff);
			break;
		}
	}
	else if (input_idx < in_length)
	{
		if (i8_count)
		{
			ENIGMA_CORE_ERROR("Base64 Decode invalid character");
		}
	}

	// Remove extra \0\0\0, clip at output_idx 
	output.resize(output_idx);

	return output;
}

String Base64::Encode(const StringView& buffer)
{
	using Constants::Utility::BASE64_CHARACTERS;

	const size_t in_length = buffer.size();
	// Should be enough to store 8-bit buffers in 6-bit buffers
	const size_t out_length = ((in_length * 4) / 3) + (in_length % 3 > 0 ? 4 : 0);
	String output(out_length + 1, '\000');

	size_t i8_count;
	size_t bits;
	size_t input_idx = 0;
	size_t output_idx = 0;

	i8_count = 0;
	bits = 0;
	for (input_idx = 0; input_idx < in_length; input_idx++)
	{
		bits |= static_cast<const byte&>(buffer[input_idx]);

		i8_count++;
		if (i8_count == 3)
		{
			output[output_idx++] = BASE64_CHARACTERS[(bits >> 18) & 0x3f];
			output[output_idx++] = BASE64_CHARACTERS[(bits >> 12) & 0x3f];
			output[output_idx++] = BASE64_CHARACTERS[(bits >> 6) & 0x3f];
			output[output_idx++] = BASE64_CHARACTERS[bits & 0x3f];
			bits = 0;
			i8_count = 0;
		}
		else
		{
			bits <<= 8;
		}
	}

	if (i8_count)
	{
		if (i8_count == 1)
		{
			bits <<= 8;
		}

		output[output_idx++] = BASE64_CHARACTERS[(bits >> 18) & 0x3f];
		output[output_idx++] = BASE64_CHARACTERS[(bits >> 12) & 0x3f];
		if (i8_count > 1)
		{
			output[output_idx++] = BASE64_CHARACTERS[(bits >> 6) & 0x3f];
		}
		else
		{
			output[output_idx++] = '=';
		}
		output[output_idx++] = '=';
	}
	output[output_idx++] = 0;
	return output;
}

inline const bool Base64::IsBase64(const ui8& c) noexcept
{
	return std::isalnum(c) || (c == '+') || (c == '/');
}

NS_ENIGMA_END

