#pragma once
#ifndef ENIGMA_STRING_UTILS_H
#define ENIGMA_STRING_UTILS_H

#include <Core/Core.hpp>
#include <Logger/Logger.hpp>

#include <string>	// std::string, std::string_view
#include <algorithm> // std::all_of
#include <utility> // std::transform

NS_ENIGMA_BEGIN
class ENIGMA_API StringUtils
{
public:

	/*
	*	Check whether all string characters are the same. 
	*	used to check if string is empty when memory is allocated with '\000' and size is not 0 (or use strlen())
	*/
	template<typename T>
	static bool IsAll(String& str, const T& v)
	{
		return std::all_of(str.begin(), str.end(), [&v](const auto& c)
			{
				return c == v;
			});
	}

    /*
    *   Removes leading and trailing spaces from a string
    */
    static void Trim(String& str, const char t[] = " \t\n\r\f\v")
    {
        TrimLeft(str, t);
        TrimRight(str, t);
    }

    /*
    *   Trim string from left
    */
    static void TrimLeft(String& str, const char t[] = " \t\n\r\f\v")
    {
        str.erase(0, str.find_first_not_of(t));
    }

    /*
    *   Trim string from right
    */
    static void TrimRight(String& str, const char t[] = " \t\n\r\f\v")
    {
        str.erase(str.find_last_not_of(t) + 1);
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

};
NS_ENIGMA_END

#endif // !ENIGMA_STRING_UTILS_H
