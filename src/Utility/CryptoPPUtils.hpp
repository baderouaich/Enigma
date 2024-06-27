#pragma once
#ifndef ENIGMA_CRYPTOPP_UTILS_H
#define ENIGMA_CRYPTOPP_UTILS_H
#include <Core/Core.hpp>
#include <cryptlib.h>

NS_ENIGMA_BEGIN

class ENIGMA_API CryptoPPUtils final
{
	ENIGMA_STATIC_CLASS(CryptoPPUtils);
public:
	/*
	*	Converts Crypto++ ErrorType into String
	*/
	static const String GetErrorString(const enum CryptoPP::Exception::ErrorType error_type) noexcept
	{
#define CASE_RETURN(e) case CryptoPP::Exception::ErrorType::e: return #e
		switch (error_type)
		{
			/// \brief A method was called which was not implemented
			CASE_RETURN(NOT_IMPLEMENTED);
			/// \brief An invalid argument was detected
			CASE_RETURN(INVALID_ARGUMENT);
			/// \brief BufferedTransformation received a Flush(true) signal but can't flush buffers
			CASE_RETURN(CANNOT_FLUSH);
			/// \brief Data integerity check, such as CRC or MAC, failed
			CASE_RETURN(DATA_INTEGRITY_CHECK_FAILED);
			/// \brief Input data was received that did not conform to expected format
			CASE_RETURN(INVALID_DATA_FORMAT);
			/// \brief Error reading from input device or writing to output device
			CASE_RETURN(IO_ERROR);
			/// \brief Some other error occurred not belonging to other categories
			default:
			CASE_RETURN(OTHER_ERROR);
		}
#undef CASE_RETURN
	}

	static const String GetErrorReason(const enum CryptoPP::Exception::ErrorType error_type)
	{
		using ErrorType = CryptoPP::Exception::ErrorType;
#define CASE_RETURN(c, ret) case c: return ret
		switch (error_type)
		{
			CASE_RETURN(ErrorType::INVALID_ARGUMENT, "An invalid argument was detected");
			CASE_RETURN(ErrorType::CANNOT_FLUSH, "BufferedTransformation received a Flush(true) signal but can't flush buffers");
			CASE_RETURN(ErrorType::DATA_INTEGRITY_CHECK_FAILED, "Data integerity check, such as CRC or MAC, failed");
			CASE_RETURN(ErrorType::INVALID_DATA_FORMAT, "Input data was received that did not conform to expected format");
			CASE_RETURN(ErrorType::IO_ERROR, "Error reading from input device or writing to output device");
			default: return "<unknown crypto++ error>";
		}
#undef CASE_RETURN
	}
	
	static const String GetFullErrorMessage(const CryptoPP::Exception& e)
	{
		std::ostringstream oss{};
		oss << "Error Type: " << GetErrorString(e.GetErrorType()) << '\n'
			<< "Message: " << e.GetWhat() << '\n' 
			<< "Reason: " << GetErrorReason(e.GetErrorType());
		return oss.str();
	}
};

NS_ENIGMA_END
#endif // !ENIGMA_CRYPTOPP_UTILS_H
