#pragma once
#ifndef ENIGMA_STRING_UTILS_H
#define ENIGMA_STRING_UTILS_H

#include <Core/Types.hpp>	// Enigma Types

#include <string>			// std::string, std::string_view
#include <sstream>		// std::stringstream
#include <algorithm>	// std::all_of
#include <utility>		// std::transform
#include <codecvt>		// helps converting between UTF-X strings
#include <locale>			// required for Linux & Darwin causes error: �wstring_convert� is not a member of �std�
#include <vector>

NS_ENIGMA_BEGIN
/*
*	UTF-8 & UTF-16 std::string Utility
*/
class ENIGMA_API StringUtils final
{
	ENIGMA_STATIC_CLASS(StringUtils);
public:
	/*
	*		Removes leading and trailing spaces from a string
	*/
	template<typename StringType>
	static void Trim(StringType& str)
	{
		TrimLeft(str);
		TrimRight(str);
	}


	/*
	*   Trim string from left
	*/
	template<typename StringType>
	static void TrimLeft(StringType& str)
	{
		str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](const auto& c)
			{
				return !std::isspace(c);
			}));
	}


	/*
	*   Trim string from right
	*/
	template<typename StringType>
	static void TrimRight(StringType& str)
	{
		str.erase(std::find_if(str.rbegin(), str.rend(), [](const auto& c)
			{
				return !std::isspace(c);
			}).base(), str.end());
	}


	/*
	*   Lowercase a string
	*/
	template<typename StringType>
	static void Lower(StringType& str)
	{
		std::transform(str.begin(), str.end(), str.begin(), [](const auto& c)
			{
				return static_cast<typename StringType::value_type>(std::tolower(c));
			});
	}


	/*
	*   Lowercase a string and return a copy of it
	*/
	template<typename StringType>
	static auto LowerCopy(const StringType& str)
	{
		StringType cstr = str;
		std::transform(cstr.begin(), cstr.end(), cstr.begin(), [](const auto& c)
			{
				return static_cast<typename StringType::value_type>(std::tolower(c));
			});
		return cstr;
	}


	/*
	*   Uppercase a string
	*/
	template<typename StringType>
	static void Upper(StringType& str)
	{
		std::transform(str.begin(), str.end(), str.begin(), [](const auto& c)
			{
				return static_cast<typename StringType::value_type>(std::toupper(c));
			});
	}


	/*
	*   Uppercase a string and return a copy of it
	*/
	template<typename StringType>
	static auto UpperCopy(const StringType& str)
	{
		StringType cstr = str;
		std::transform(cstr.begin(), cstr.end(), cstr.begin(), [](const auto& c)
			{
				return static_cast<typename StringType::value_type>(std::toupper(c));
			});
		return cstr;
	}


	/*
	*   Check if a string starts with a prefix
	*/
	template<typename StringType>
	static bool StartsWith(const StringType& str, const StringType& prefix)
	{
		return str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0;
	}


	/*
	*   Check if a string ends with a suffix
	*/
	template<typename StringType>
	static bool EndsWith(const StringType& str, const StringType& suffix)
	{
		return str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
	}

	/*
	*	Splits string at a delimiter into parts
	*/
	template<typename StringType>
	static std::vector<StringType> Split(const StringType& str, const typename std::string::value_type delimiter)
	{
		std::vector<StringType> parts{};
		std::stringstream ss(str);
		std::string line{};
		while (std::getline(ss, line, delimiter))
		{
			parts.push_back(line);
		}
		return parts;
	}

	/*
	*   Check if a string contains another
	*/
	template<typename StringType>
	static bool Contains(const StringType& str, const StringType& other)
	{
		return str.find(other) != StringType::npos;
	}


	/*
	*	Converts UTF-8 std::string to UTF-16 std::wstring
	*/
	static std::wstring StringToWString(const std::string& str)
	{
		using convert_type = std::codecvt_utf8<wchar_t>;
		static std::wstring_convert<convert_type, wchar_t> converter;
		return converter.from_bytes(str);
	}


	/*
	*	Converts UTF-16 std::wstring to UTF-8 std::string
	*/
	static std::string WStringToString(const std::wstring& wstr)
	{
		using convert_type = std::codecvt_utf8<wchar_t>;
		static std::wstring_convert<convert_type, wchar_t> converter;
		return converter.to_bytes(wstr);
	}


	/*
	*	Convert string to any other simple type
	*/
	template <typename T, typename StringType>
	static T To(const StringType& str)
	{
		T t{};
		std::basic_stringstream<typename StringType::value_type> ss(str);
		ss >> t;
		return t;
	}

#if 0

	/*
	*	Cleanup a string (remove string from string)
	*/
	static std::string Cleanup(const std::string& expr, const std::string& remove)
	{
		const std::size_t N = expr.size();
		const std::size_t K = remove.size();
		std::string result(N, '\000');
		std::size_t srcIndex = 0;
		std::size_t dstIndex = 0;
		while (srcIndex < N)
		{
			std::size_t matchIndex = 0;
			while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 && expr[srcIndex + matchIndex] == remove[matchIndex])
			{
				matchIndex++;
				if (matchIndex == K - 1)
					srcIndex += matchIndex;
			}
			result[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
			srcIndex++;
		}
		result.resize(dstIndex, '\000'); // resize to fit string after removing other str
		return result;
	}
	static std::wstring Cleanup(const std::wstring& expr, const std::wstring& remove)
	{
		const std::size_t N = expr.size();
		const std::size_t K = remove.size();
		std::wstring result(N, L'\000');
		std::size_t srcIndex = 0;
		std::size_t dstIndex = 0;
		while (srcIndex < N)
		{
			std::size_t matchIndex = 0;
			while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 && expr[srcIndex + matchIndex] == remove[matchIndex])
				matchIndex++;
			if (matchIndex == K - 1)
				srcIndex += matchIndex;
			result[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
			srcIndex++;
		}
		result.resize(dstIndex, L'\000'); // resize to fit string after removing other str
		return result;
	}

	template <std::size_t N>
	struct CleanupResult // wrapper to keep data variable alive in the stack after going out of scope of Cleanup function
	{
		char data[N];
	};
	template <std::size_t N, std::size_t K>
	static auto Cleanup(const char(&expr)[N], const char(&remove)[K])
	{
		CleanupResult<N> result{};
		std::size_t srcIndex = 0;
		std::size_t dstIndex = 0;
		//while constexpr (srcIndex < N)
		while (srcIndex < N)
		{
			std::size_t matchIndex = 0;
			while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 && expr[srcIndex + matchIndex] == remove[matchIndex])
				matchIndex++;
			if (matchIndex == K - 1)
				srcIndex += matchIndex;
			result.data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
			srcIndex++;
		}
		return result;
	}
#endif
};
NS_ENIGMA_END

#endif // !ENIGMA_STRING_UTILS_H
