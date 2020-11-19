#include <pch.hpp>
#include "Base64.hpp"


NS_ENIGMA_BEGIN

constexpr const ui8 m_characters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

i32 _base64Decode(const byte* input, size_t input_len, byte* output, size_t* output_len)
{
	static byte inalphabet[256], decoder[256];
	i32 i, bits, c = 0, i8_count, errors = 0;
	ui32 input_idx = 0;
	ui32 output_idx = 0;

	for (i = (sizeof(m_characters)) - 1; i >= 0; i--) 
	{
		inalphabet[m_characters[i]] = 1;
		decoder[m_characters[i]] = i;
	}

	i8_count = 0;
	bits = 0;
	for (input_idx = 0; input_idx < input_len; input_idx++) 
	{
		c = input[input_idx];
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
			errors++;
			break;
		case 2:
			output[output_idx++] = (bits >> 10);
			break;
		case 3:
			output[output_idx++] = (bits >> 16);
			output[output_idx++] = ((bits >> 8) & 0xff);
			break;
		}
	}
	else if (input_idx < input_len)
	{
		if (i8_count)
		{
			errors++;
		}
	}

	*output_len = output_idx;
	return errors;
}

void _base64Encode(const byte* input, size_t input_len, byte* output)
{
	size_t i8_count;
	size_t bits;
	size_t input_idx = 0;
	size_t output_idx = 0;

	i8_count = 0;
	bits = 0;
	for (input_idx = 0; input_idx < input_len; input_idx++) 
	{
		bits |= input[input_idx];

		i8_count++;
		if (i8_count == 3) 
		{
			output[output_idx++] = m_characters[(bits >> 18) & 0x3f];
			output[output_idx++] = m_characters[(bits >> 12) & 0x3f];
			output[output_idx++] = m_characters[(bits >> 6) & 0x3f];
			output[output_idx++] = m_characters[bits & 0x3f];
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

		output[output_idx++] = m_characters[(bits >> 18) & 0x3f];
		output[output_idx++] = m_characters[(bits >> 12) & 0x3f];
		if (i8_count > 1) 
		{
			output[output_idx++] = m_characters[(bits >> 6) & 0x3f];
		}
		else 
		{
			output[output_idx++] = '=';
		}
		output[output_idx++] = '=';
	}

	output[output_idx++] = 0;
}

size_t Base64::Decode(const byte* in, size_t inLength, byte** out)
{
	size_t outLength = 0;

	//should be enough to store 6-bit buffers in 8-bit buffers
	*out = (byte*)malloc(static_cast<size_t>(((inLength * 3) / 4) + 1));
	if (*out)
	{
		size_t ret = _base64Decode(in, inLength, *out, &outLength);

		if (ret > 0)
		{
			free(*out);
			*out = nullptr;
			outLength = 0;
		}
	}
	return outLength;
}

size_t Base64::Encode(const byte* in, size_t inLength, byte** out)
{
	size_t outLength = inLength * 4 / 3 + (inLength % 3 > 0 ? 4 : 0);
	//should be enough to store 8-bit buffers in 6-bit buffers
	*out = (byte*)malloc(outLength + 1);
	if (*out) 
	{
		_base64Encode(in, inLength, *out);
	}
	return outLength;
}
NS_ENIGMA_END
