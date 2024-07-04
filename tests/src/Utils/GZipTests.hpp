#pragma once
#include "TestsData.hpp"
#include <Utility/GZip.hpp.ignore>
#include <Utility/SizeUtils.hpp>
#include <catch2/catch_all.hpp>
using namespace Enigma;
using namespace Catch;
using namespace Catch::Matchers;

TEST_CASE("Compress & Decompress Buffer", "GZip")
{
	std::cout << "\n======[ " << Catch::getResultCapture().getCurrentTestName() << " ]======\n";

	const std::string buffer = LOREM_IPSUM * 10000; // 10000 paragraphs of lorem ipsum dummy text about ~6MB
	std::cout << "Buffer Size: " << SizeUtils::FriendlySize( buffer.size() ) << std::endl;
	std::string compressed = GZip::Compress(buffer);
	std::cout << "Compressed Size: " << SizeUtils::FriendlySize(compressed.size()) << std::endl;
	std::string decompressed = GZip::Decompress(compressed);
	std::cout << "Decompressed Size: " << SizeUtils::FriendlySize(decompressed.size()) << std::endl;
	
	REQUIRE_THAT(compressed, !Equals(buffer));
	REQUIRE(compressed.size() <= buffer.size());
	REQUIRE_THAT(decompressed, Equals(buffer));
	REQUIRE(decompressed.size() == buffer.size());
}
