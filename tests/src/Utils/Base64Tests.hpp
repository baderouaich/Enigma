#pragma once
#include <Core/Core.hpp>
#include <catch2/catch_all.hpp>
#include <Utility/Base64.hpp>
#include <Utility/SizeUtils.hpp>
#include <iostream>
#include "TestsData.hpp"

using namespace Enigma;
using namespace Catch;
using namespace Catch::Matchers;

#define BEGIN_TIMER(var) auto var = std::chrono::steady_clock::now()
#define END_TIMER(var, unit)  std::chrono::duration_cast<std::chrono::unit>(std::chrono::steady_clock::now() - var).count()

TEST_CASE("Base64 Encode & Decode - Buffer")
{
  std::cout << "\n======[ " << Catch::getResultCapture().getCurrentTestName() << " ]======\n";
  std::vector<byte> bytes = Random::Bytes(Random::Int<std::size_t>(1, ENIGMA_MB_TO_BYTES(50)));
  // Encode bytes to Base64
  std::vector<byte> encodedBytes = Base64::Encode(bytes);
  // Decode base64 to bytes
  std::vector<byte> decodedBytes = Base64::Decode(encodedBytes);

  REQUIRE_THAT(bytes, !Equals(encodedBytes));
  REQUIRE_THAT(decodedBytes, !Equals(encodedBytes));
  REQUIRE_THAT(bytes, Equals(decodedBytes));
}

TEST_CASE("Base64 Encode & Decode - Text")
{
  std::cout << "\n======[ " << Catch::getResultCapture().getCurrentTestName() << " ]======\n";

  std::string buffer = LOREM_IPSUM * Random::Int<std::size_t>(10, 10000);

  BEGIN_TIMER(t1);
  std::string base64_encoded = Base64::Encode(buffer);
  auto en_ms = END_TIMER(t1, milliseconds);
  std::cout << "Encoded " << SizeUtils::FriendlySize(buffer.size()) << " in " << en_ms << "ms\n";

  BEGIN_TIMER(t2);
  std::string base64_decoded = Base64::Decode(base64_encoded);
  auto de_ms = END_TIMER(t2, milliseconds);
  std::cout << "Decoded " << SizeUtils::FriendlySize(base64_encoded.size()) << " in " << de_ms << "ms\n";

  REQUIRE(buffer != base64_encoded);
  REQUIRE(buffer == base64_decoded);
}
