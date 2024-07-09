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

using namespace Enigma;
using namespace Catch::Matchers;


TEST_CASE("Endianness") {
  Catch::cout() << "Endianness: ";
  if (std::endian::native == std::endian::big) {
    Catch::cout() << "Big";
  } else if (std::endian::native == std::endian::little) {
    Catch::cout() << "Little";
  } else [[unlikely]] {
    REQUIRE(false);
  }
  std::cout << std::endl;
}