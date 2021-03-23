#pragma once
#ifndef ENIGMA_STRING_UTILS_H
#define ENIGMA_STRING_UTILS_H

#include <Core/Core.hpp>	// Enigma Types

#include <string>		// std::string, std::string_view
#include <algorithm>	// std::all_of
#include <utility>		// std::transform
#include <codecvt>		// helps converting between UTF-X strings

NS_ENIGMA_BEGIN
/*
*	UTF-8 & UTF-16 String Utils
*/
class ENIGMA_API StringUtils
{
public:
    /*
    *   Removes leading and trailing spaces from a string
    */
    static void Trim(String& str, const char t[] = " \t\n\r\f\v")
    {
        TrimLeft(str, t);
        TrimRight(str, t);
    }
	static void Trim(WString& wstr, const wchar_t t[] = L" \t\n\r\f\v")
	{
		TrimLeft(wstr, t);
		TrimRight(wstr, t);
	}

    /*
    *   Trim string from left
    */
    static void TrimLeft(String& str, const char t[] = " \t\n\r\f\v")
    {
        str.erase(0, str.find_first_not_of(t));
    }
	static void TrimLeft(WString& wstr, const wchar_t t[] = L" \t\n\r\f\v")
	{
		wstr.erase(0, wstr.find_first_not_of(t));
	}

    /*
    *   Trim string from right
    */
    static void TrimRight(String& str, const char t[] = " \t\n\r\f\v")
    {
        str.erase(str.find_last_not_of(t) + 1);
    }
	static void TrimRight(WString& wstr, const wchar_t t[] = L" \t\n\r\f\v")
	{
		wstr.erase(wstr.find_last_not_of(t) + 1);
	}

	/*
	*   Lowercase a string
	*/
	static void Lower(String& str)
	{
		std::transform(str.begin(), str.end(), str.begin(), [](const char& c)
			{
				return static_cast<char>(std::tolower(c));
			});
	}
	static void Lower(WString& wstr)
	{
		std::transform(wstr.begin(), wstr.end(), wstr.begin(), [](const wchar_t& c)
			{
				return static_cast<wchar_t>(std::tolower(c));
			});
	}

	/*
	*   Lowercase a string and return a copy of it
	*/
	static String LowerCopy(const String& str)
	{
		String cstr = str;
		std::transform(cstr.begin(), cstr.end(), cstr.begin(), [](const char& c)
			{
				return static_cast<char>(std::tolower(c));
			});
		return cstr;
	}
	static WString LowerCopy(const WString& wstr)
	{
		WString cwstr = wstr;
		std::transform(cwstr.begin(), cwstr.end(), cwstr.begin(), [](const wchar_t& c)
			{
				return static_cast<wchar_t>(std::tolower(c));
			});
		return cwstr;
	}

	/*
	*   Uppercase a string
	*/
	static void Upper(String& str)
	{
		std::transform(str.begin(), str.end(), str.begin(), [](const char& c)
			{
				return static_cast<char>(std::toupper(c));
			});
	}
	static void Upper(WString& wstr)
	{
		std::transform(wstr.begin(), wstr.end(), wstr.begin(), [](const wchar_t& c)
			{
				return static_cast<wchar_t>(std::toupper(c));
			});
	}

	/*
	*   Uppercase a string and return a copy of it
	*/
	static String UpperCopy(const String& str)
	{
		String cstr = str;
		std::transform(cstr.begin(), cstr.end(), cstr.begin(), [](const char& c)
			{
				return static_cast<char>(std::toupper(c));
			});
		return cstr;
	}
	static WString UpperCopy(const WString& wstr)
	{
		WString cwstr = wstr;
		std::transform(cwstr.begin(), cwstr.end(), cwstr.begin(), [](const wchar_t& c)
			{
				return static_cast<wchar_t>(std::toupper(c));
			});
		return cwstr;
	}


	/*
	*   Check if a string starts with a prefix
	*/
	static bool StartsWith(const String& str, const String& prefix)
	{
		return str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0;
	}
	static bool StartsWith(const WString& wstr, const WString& wprefix)
	{
		return wstr.size() >= wprefix.size() && wstr.compare(0, wprefix.size(), wprefix) == 0;
	}

	/*
	*   Check if a string ends with a suffix
	*/
	static bool EndsWith(const String& str, const String& suffix)
	{
		return str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
	}
	static bool EndsWith(const WString& wstr, const WString& wsuffix)
	{
		return wstr.size() >= wsuffix.size() && wstr.compare(wstr.size() - wsuffix.size(), wsuffix.size(), wsuffix) == 0;
	}


	/*
	*	Converts UTF-8 std::string to UTF-16 std::wstring
	*/
	static WString StringToWString(const String& str)
	{
		using convert_type = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_type, wchar_t> converter;

		return converter.from_bytes(str);
	}

	/*
	*	Converts UTF-16 std::wstring to UTF-8 std::string
	*/
	static String WStringToString(const WString& wstr)
	{
		using convert_type = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_type, wchar_t> converter;
		sizeof convert_type;
		return converter.to_bytes(wstr);
	}

};
NS_ENIGMA_END

#endif // !ENIGMA_STRING_UTILS_H
