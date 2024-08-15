#pragma once

#include <Utility/HashUtils.hpp>
#include <catch2/catch_all.hpp>
#include <iostream>

using namespace Enigma;
using namespace Catch;
using namespace Catch::Matchers;


TEST_CASE("HashUtils - SHA256") {
  std::cout << "\n======[ " << Catch::getResultCapture().getCurrentTestName() << " ]======\n";
  std::vector<byte> buffer = {'H', 'e', 'l', 'l', 'o'};
  static constexpr std::string_view SHA256_OF_HELLO = "185f8db32271fe25f561a6fc938b2e264306ec304eda518007d1764826381969";
  static constexpr std::string_view SHA256_OF_HELLO_WORLD = "dffd6021bb2bd5b0af676290809ec3a53191dd81c7f70a4b28688a362182986f";

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

  SECTION("SHA256 - File") {
    std::string ORIGIN_SHA256;
    FileUtils::ReadString(fs::path(TEST_DATA_DIR) / "file.bin.sha256", ORIGIN_SHA256);
    fs::path filename = fs::path(TEST_DATA_DIR) / "file.bin";
    const auto hash = HashUtils::fileStr<CryptoPP::SHA256>(filename);
    REQUIRE(hash == ORIGIN_SHA256);
  }
}

TEST_CASE("HashUtils - SHA512") {
  std::cout << "\n======[ " << Catch::getResultCapture().getCurrentTestName() << " ]======\n";
  std::vector<byte> buffer = {'H', 'e', 'l', 'l', 'o'};
  static constexpr std::string_view SHA512_OF_HELLO = "3615f80c9d293ed7402687f94b22d58e529b8cc7916f8fac7fddf7fbd5af4cf777d3d795a7a00a16bf7e7f3fb9561ee9baae480da9fe7a18769e71886b03f315";
  static constexpr std::string_view SHA512_OF_HELLO_WORLD = "374d794a95cdcfd8b35993185fef9ba368f160d8daf432d08ba9f1ed1e5abe6cc69291e0fa2fe0006a52570ef18c19def4e617c33ce52ef0a6e5fbe318cb0387";
  std::array<byte, CryptoPP::SHA512::DIGESTSIZE> hash = HashUtils::bytes<CryptoPP::SHA512>(buffer);
  std::string hashStr = HashUtils::str<CryptoPP::SHA512>(buffer);
  REQUIRE(hash.size() == hashStr.size() / 2);
  REQUIRE(hashStr == SHA512_OF_HELLO);
  std::ostringstream oss{};
  for (byte digest: hash) {
    oss
      << std::hex << std::setfill('0') << std::setw(2)
      << static_cast<int>(digest);
  }
  REQUIRE(oss.str() == hashStr);

  buffer = {'H', 'e', 'l', 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd', '!'};
  hash = HashUtils::bytes<CryptoPP::SHA512>(buffer);
  hashStr = HashUtils::str<CryptoPP::SHA512>(buffer);
  REQUIRE(hash.size() == hashStr.size() / 2);
  REQUIRE(hashStr == SHA512_OF_HELLO_WORLD);
  oss.str({});
  for (byte digest: hash) {
    oss
      << std::hex << std::setfill('0') << std::setw(2)
      << static_cast<int>(digest);
  }
  REQUIRE(oss.str() == hashStr);


  SECTION("SHA512 - File") {
    std::string ORIGIN_SHA512;
    FileUtils::ReadString(fs::path(TEST_DATA_DIR) / "file.bin.sha512", ORIGIN_SHA512);
    fs::path filename = fs::path(TEST_DATA_DIR) / "file.bin";
    const auto hash = HashUtils::fileStr<CryptoPP::SHA512>(filename);
    REQUIRE(hash == ORIGIN_SHA512);
  }
}

TEST_CASE("HashUtils - String case") {
  fs::path filename = fs::path(TEST_DATA_DIR) / "file.bin";
  const std::string uppercaseHash = HashUtils::fileStr<CryptoPP::SHA256>(filename, true);
  const std::string lowercaseHash = HashUtils::fileStr<CryptoPP::SHA256>(filename, false);
  REQUIRE(std::all_of(uppercaseHash.begin(), uppercaseHash.end(), [](char c) -> bool {
    return std::isdigit(c) or std::isupper(c);
  }));
  REQUIRE(std::all_of(lowercaseHash.begin(), lowercaseHash.end(), [](char c) -> bool {
    return std::isdigit(c) or std::islower(c);
  }));
}