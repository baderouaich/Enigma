#pragma once
#ifndef ENIGMA_BASE64_H
#define ENIGMA_BASE64_H

#include <Core/Core.hpp>

NS_ENIGMA_BEGIN
class ENIGMA_API Base64
{
public:
    /*
    *   Decodes a 64base encoded memory. The decoded memory is
    *   expected to be freed by the caller by calling `free()`
    *   returns the length of the out buffer
    */
    static size_t Decode(const byte* in, size_t inLength, byte** out);

    /*
     *  Encodes bytes into a 64base encoded memory with terminating '\0' character.
     *  The encoded memory is expected to be freed by the caller by calling `free()`
     *  returns the length of the out buffer
     */
    static size_t Encode(const byte* in, size_t inLength, byte** out);
};
NS_ENIGMA_END

#endif // !ENIGMA_BASE64_H
