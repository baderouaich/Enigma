#include <pch.hpp>
#include "Base64.hpp"
#include <base64.h> // Crypto++'s Base64 Encode/Decoder

NS_ENIGMA_BEGIN

std::vector<byte> Base64::Encode(const std::vector<byte>& buffer)
{
  return Encode(buffer.data(), buffer.size());
}
std::vector<byte> Base64::Encode(const byte *buffer, const std::size_t buffLen) {
  std::vector<byte> encoded{};

  const auto encoder = std::make_unique<CryptoPP::Base64Encoder>();
  encoder->Put(buffer, buffLen);
  encoder->MessageEnd();

  const CryptoPP::word64 size = encoder->MaxRetrievable();
  if (size)
  {
    encoded.resize(size, '\000');
    encoder->Get(&encoded[0], encoded.size());
  }

  return encoded;
}

std::vector<byte> Base64::Decode(const std::vector<byte>& encoded)
{
  return Decode(encoded.data(), encoded.size());
}

std::vector<byte> Base64::Decode(const byte *buffer, const std::size_t buffLen) {
  std::vector<byte> decoded{};

  const auto decoder = std::make_unique<CryptoPP::Base64Decoder>();
  decoder->Put(buffer, buffLen);
  decoder->MessageEnd();

  const CryptoPP::word64 size = decoder->MaxRetrievable();
  if (size && size <= std::numeric_limits<decltype(size)>::max())
  {
    decoded.resize(size, '\000');
    decoder->Get(&decoded[0], decoded.size());
  }

  return decoded;
}
std::string Base64::Encode(const std::string& buffer) {
  std::string encoded{};

  const auto encoder = std::make_unique<CryptoPP::Base64Encoder>();
  encoder->Put(reinterpret_cast<const byte*>(buffer.data()), buffer.size());
  encoder->MessageEnd();

  const CryptoPP::word64 size = encoder->MaxRetrievable();
  if (size)
  {
    encoded.resize(size, '\000');
    encoder->Get(reinterpret_cast<byte*>(&encoded[0]), encoded.size());
  }

  return encoded;
}
std::string Base64::Decode(const std::string& encoded) {
  std::string decoded{};

  const auto decoder = std::make_unique<CryptoPP::Base64Decoder>();
  decoder->Put(reinterpret_cast<const byte*>(encoded.data()), encoded.size());
  decoder->MessageEnd();

  const CryptoPP::word64 size = decoder->MaxRetrievable();
  if (size && size <= std::numeric_limits<decltype(size)>::max())
  {
    decoded.resize(size, '\000');
    decoder->Get(reinterpret_cast<byte*>(&decoded[0]), decoded.size());
  }

  return decoded;
}


NS_ENIGMA_END

