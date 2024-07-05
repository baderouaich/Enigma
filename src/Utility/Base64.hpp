#pragma once
#ifndef ENIGMA_BASE64_H
#define ENIGMA_BASE64_H

#include <Core/Core.hpp>

NS_ENIGMA_BEGIN
class Base64 final {
    ENIGMA_STATIC_CLASS(Base64);

  public:
    /*
    *	Encodes string bytes into Base64 string
    * https://www.cryptopp.com/wiki/Base64Encoder
    */
    static std::vector<byte> Encode(const std::vector<byte>& buffer);
    static std::vector<byte> Encode(const byte *buffer, const std::size_t buffLen);
    static std::string Encode(const std::string& buffer);

    /*
    * Decodes a Base64 string
    * https://www.cryptopp.com/wiki/Base64Decoder
    */
    static std::vector<byte> Decode(const std::vector<byte>& encoded_buffer);
    static std::vector<byte> Decode(const byte *buffer, const std::size_t buffLen);
    static std::string Decode(const std::string& encoded);
};
NS_ENIGMA_END

#endif // !ENIGMA_BASE64_H
