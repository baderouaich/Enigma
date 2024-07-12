#pragma once
#include <Core/Core.hpp>
#include <array>
#include <files.h>   // HexEncoder
#include <filters.h> // VectorSource, StringSource, VectorSink, StringSink
#include <fstream>
#include <hex.h>      // HexEncoder
#include <keccak.h>   // Keccak
#include <md2.h>      // MD2
#include <md4.h>      // MD4
#include <md5.h>      // MD5
#include <sha.h>      // SHA1, SHA244, SHA256, SHA384, SHA512
#include <shake.h>    // Shake
#include <tiger.h>    // Tiger
#include <whrlpool.h> // Whirlpool


NS_ENIGMA_BEGIN

class HashUtils final {
    ENIGMA_STATIC_CLASS(HashUtils);

  public:
    template<typename Algo = CryptoPP::SHA256>
    static std::array<byte, Algo::DIGESTSIZE> bytes(const byte *buffer, const std::size_t buffSize) {
      std::array<byte, Algo::DIGESTSIZE> out{};
      Algo algo{};
      const CryptoPP::ArraySource vs(buffer, buffSize, true, new CryptoPP::HashFilter(algo, new CryptoPP::ArraySink(out.data(), out.size())));
      return out;
    }

    template<typename Algo = CryptoPP::SHA256>
    static std::array<byte, Algo::DIGESTSIZE> bytes(const std::vector<byte>& buffer) {
      return bytes<Algo>(buffer.data(), buffer.size());
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
#if OLD_IMPL
      std::array<byte, Algo::DIGESTSIZE> out{};
      Algo algo{};
      std::ifstream file{filename, std::ios::binary}; // very important to open file in binary mode otherwise you will get
                                                      // different hash values in different OS (unix2dos dos2unix kinda crap)

      ENIGMA_ASSERT_OR_THROW(file.good(), "No such file " + filename.string());
      const CryptoPP::FileSource fs(file,
                                    true,
                                    new CryptoPP::HashFilter(algo, new CryptoPP::ArraySink(out.data(), out.size())));
      file.close();
      return out;
#else
      Algo algo{};
      std::array<byte, Algo::DIGESTSIZE> out{};
      const CryptoPP::FileSource fileSource(filename.string().c_str(), true,
                                            new CryptoPP::HashFilter(
                                              algo,
                                              new CryptoPP::ArraySink(out.data(), out.size())));
      return out;
#endif
    }

    template<typename Algo = CryptoPP::SHA256>
    static std::string fileStr(const fs::path& filename, const bool uppercase = false) {
#if OLD_IMPL
      std::string out{};
      std::ifstream file{filename, std::ios::binary}; // very important to open file in binary mode otherwise you will get
                                                      // different hash values in different OS (unix2dos dos2unix kinda crap)
      ENIGMA_ASSERT_OR_THROW(file.good(), "No such file " + filename.string());
      const CryptoPP::FileSource fs(file, true, new CryptoPP::HashFilter(Algo(), new CryptoPP::HexEncoder(new CryptoPP::StringSink(out), uppercase)));
      file.close();
      return out;
#else
      Algo algo{};
      std::string out{};
      ENIGMA_ASSERT_OR_THROW(fs::is_regular_file(filename), "No such file " + filename.string());
      const CryptoPP::FileSource fileSource(filename.string().c_str(), true,
                                            new CryptoPP::HashFilter(
                                              algo,
                                              new CryptoPP::HexEncoder(
                                                new CryptoPP::StringSink(out),
                                                uppercase
                                                )));
      return out;
#endif
    }

    template<typename Algo = CryptoPP::SHA256>
    static std::string stringify(const std::array<byte, Algo::DIGESTSIZE>& hash, const bool uppercase = false) {
      std::ostringstream out{};
      for (const byte digest: hash) {
        out << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(digest);
      }
      return out.str();
    }
};

NS_ENIGMA_END
