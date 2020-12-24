#pragma once
#ifndef ENIGMA_CRYPTOPP_UTILS_H
#define ENIGMA_CRYPTOPP_UTILS_H
#include <Core/Core.hpp>
#include <cryptlib.h>

NS_ENIGMA_BEGIN

class ENIGMA_API CryptoPPUtils
{
public:
	/*
	*	Converts Crypto++ ErrorType into String
	*/
	static const String GetCryptoPPErrorString(const enum CryptoPP::Exception::ErrorType& _enum) noexcept
	{
#define CASE_ENUM(e) case CryptoPP::Exception::ErrorType::e: return #e
		switch (_enum)
		{
			/// \brief A method was called which was not implemented
			CASE_ENUM(NOT_IMPLEMENTED);
			/// \brief An invalid argument was detected
			CASE_ENUM(INVALID_ARGUMENT);
			/// \brief BufferedTransformation received a Flush(true) signal but can't flush buffers
			CASE_ENUM(CANNOT_FLUSH);
			/// \brief Data integerity check, such as CRC or MAC, failed
			CASE_ENUM(DATA_INTEGRITY_CHECK_FAILED);
			/// \brief Input data was received that did not conform to expected format
			CASE_ENUM(INVALID_DATA_FORMAT);
			/// \brief Error reading from input device or writing to output device
			CASE_ENUM(IO_ERROR);
			/// \brief Some other error occurred not belonging to other categories
			default:
			CASE_ENUM(OTHER_ERROR);
		}
#undef CASE_ENUM
	}
};

NS_ENIGMA_END
#endif // !ENIGMA_CRYPTOPP_UTILS_H
