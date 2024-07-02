#pragma once
#include <Core/Core.hpp>
#include <array>
#include <fstream>
#include <md2.h> // MD2
#include <md4.h> // MD4
#include <md5.h> // MD5
#include <sha.h> // SHA1, SHA244, SHA256, SHA384, SHA512
#include <keccak.h> // Keccak
#include <shake.h> // Shake
#include <tiger.h> // Tiger
#include <whrlpool.h> // Whirlpool
#include <filters.h> // VectorSource, StringSource, VectorSink, StringSink
#include <hex.h> // HexEncoder
#include <files.h> // HexEncoder


NS_ENIGMA_BEGIN

class HashUtils final {
    ENIGMA_STATIC_CLASS(HashUtils);

  public:
    template<typename Algo = CryptoPP::SHA256>
    static std::array<byte, Algo::DIGESTSIZE> bytes(const std::vector<byte>& buffer) {
      std::array<byte, Algo::DIGESTSIZE> out{};
      Algo algo{};
      const CryptoPP::VectorSource vs(buffer, true, new CryptoPP::HashFilter(algo, new CryptoPP::ArraySink(out.data(), out.size())));
      return out;
    }

    template<typename Algo = CryptoPP::SHA256>
    static std::string str(const std::vector<byte>& buffer, const bool uppercase = false) {
      std::string out{};
      Algo algo{};
      const CryptoPP::VectorSource vs(buffer, true, new CryptoPP::HashFilter(algo, new CryptoPP::HexEncoder(new CryptoPP::StringSink(out), uppercase)));
      return out;
    }

    template<typename Algo = CryptoPP::SHA256>
    static std::array<byte, Algo::DIGESTSIZE> fileBytes(const fs::path& filename) {
      std::array<byte, Algo::DIGESTSIZE> out{};
      Algo algo{};
      std::ifstream file{filename};
      ENIGMA_ASSERT_OR_THROW(file.good(), "No such file " + filename.string());
      const CryptoPP::FileSource fs(file,
                              true,
                              new CryptoPP::HashFilter(algo, new CryptoPP::ArraySink(out.data(), out.size())));
      file.close();
      return out;
    }

    template<typename Algo = CryptoPP::SHA256>
    static std::string fileStr(const fs::path& filename, const bool uppercase = false) {
      std::string out{};
      Algo algo{};
      std::ifstream file{filename, std::ios::binary};
      ENIGMA_ASSERT_OR_THROW(file.good(), "No such file " + filename.string());
      const CryptoPP::FileSource fs(file, true, new CryptoPP::HashFilter(algo, new CryptoPP::HexEncoder(new CryptoPP::StringSink(out), uppercase)));
      file.close();
      return out;
    }

    template<typename Algo = CryptoPP::SHA256>
    static std::string stringify(const std::array<byte, Algo::DIGESTSIZE>& hash, const bool uppercase = false)
    {
      std::ostringstream out{};
      for(const byte digest : hash) {
        out << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(digest);
      }
      return out.str();
    }
};

NS_ENIGMA_END
