#pragma once

#include <Utility/HashUtils.hpp>
#include <catch2/catch_all.hpp>
#include <iostream>

using namespace Enigma;
using namespace Catch;
using namespace Catch::Matchers;

static constexpr std::string_view SHA256_OF_HELLO = "2cf24dba5fb0a30e26e83b2ac5b9e29e1b161e5c1fa7425e73043362938b9824";
static constexpr std::string_view SHA256_OF_HELLO_WORLD = "dffd6021bb2bd5b0af676290809ec3a53191dd81c7f70a4b28688a362182986f";

TEST_CASE("HashUtils", "SHA256") {
  std::cout << "\n======[ " << Catch::getResultCapture().getCurrentTestName() << " ]======\n";
  std::vector<byte> buffer = {'h', 'e', 'l', 'l', 'o'};

  std::array<byte, CryptoPP::SHA256::DIGESTSIZE> hash = HashUtils::bytes<CryptoPP::SHA256>(buffer);
  std::string hashStr = HashUtils::str<CryptoPP::SHA256>(buffer);
  REQUIRE(hash.size() == hashStr.size() / 2);
  REQUIRE(hashStr == SHA256_OF_HELLO);
  std::ostringstream oss{};
  for (byte digest: hash) {
    oss
      << std::hex << std::setfill('0') << std::setw(2)
      << static_cast<int>(digest);
  }
  REQUIRE(oss.str() == hashStr);

  buffer = {'H', 'e', 'l', 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd', '!'};
  hash = HashUtils::bytes<CryptoPP::SHA256>(buffer);
  hashStr = HashUtils::str<CryptoPP::SHA256>(buffer);
  REQUIRE(hash.size() == hashStr.size() / 2);
  REQUIRE(hashStr == SHA256_OF_HELLO_WORLD);
  oss.str({});
  for (byte digest: hash) {
    oss
      << std::hex << std::setfill('0') << std::setw(2)
      << static_cast<int>(digest);
  }
  REQUIRE(oss.str() == hashStr);
}
