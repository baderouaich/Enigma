#pragma once
#ifndef ENIGMA_STRING_UTILS_H
#define ENIGMA_STRING_UTILS_H

#include <Core/Core.hpp>
#include <Logger/Logger.hpp>

#include <string>

NS_ENIGMA_BEGIN
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

};
NS_ENIGMA_END

#endif // !ENIGMA_STRING_UTILS_H
