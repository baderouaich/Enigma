#pragma once
#include "TestsData.hpp"
#include "Utility/DateTimeUtils.hpp"
#include "Utility/FinalAction.hpp"
#include "Utility/HashUtils.hpp"
#include <Utility/FileUtils.hpp>
#include <Utility/Random.hpp>
#include <Utility/SizeUtils.hpp>
#include <catch2/catch_all.hpp>

#include <bit> // std::endian::native
#include <Meta/Endianness.hpp>

using namespace Enigma;
using namespace Catch::Matchers;

TEST_CASE("Endianness - Check Conversions") {
  std::uint64_t value = 0xbadcafe;
  std::uint64_t valueLE = LittleEndian::toHost(value);
  REQUIRE(value == valueLE);

  std::uint64_t valueBE = BigEndian::toHost(value);
  REQUIRE(valueBE != value);
  std::uint64_t valueBE_Recovered = BigEndian::fromHost(valueBE);
  REQUIRE(valueLE == valueBE_Recovered);
}


TEST_CASE("Endianness - Check Little Endian") {
  Catch::cout() << "Endianness: ";
  if (std::endian::native == std::endian::big) {
    Catch::cout() << "Big";
    REQUIRE(false);
  } else if (std::endian::native == std::endian::little) {
    Catch::cout() << "Little";
  } else [[unlikely]] {
    REQUIRE(false);
  }
  Catch::cout() << std::endl;
}

