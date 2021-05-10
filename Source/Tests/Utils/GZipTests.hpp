#pragma once
#include <catch2/catch_all.hpp>
#include <Utility/GZip.hpp>
#include <Tests/TestData.hpp>
using namespace Enigma;
using namespace Catch;
using namespace Catch::Matchers;

TEST_CASE("Compress & Decompress Buffer", "GZip")
{
	std::cout << "\n======[ " << Catch::getResultCapture().getCurrentTestName() << " ]======\n";

	const String buffer = LOREM_IPSUM * 10000; // 10000 paragraphs of lorem ipsum dummy text about ~6MB
	String compressed, decompressed;
	std::cout << "Buffer Size: " << SizeUtils::FriendlySize( buffer.size() ) << std::endl;
	compressed = GZip::Compress(buffer);
	std::cout << "Compressed Size: " << SizeUtils::FriendlySize(compressed.size()) << std::endl;
	decompressed = GZip::Decompress(compressed);
	std::cout << "Decompressed Size: " << SizeUtils::FriendlySize(decompressed.size()) << std::endl;
	
	REQUIRE_THAT(compressed, !Equals(buffer));
	REQUIRE(compressed.size() <= buffer.size());
	REQUIRE_THAT(decompressed, Equals(buffer));
	REQUIRE(decompressed.size() == buffer.size());
}
