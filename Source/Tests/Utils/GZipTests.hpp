#pragma once
#include <catch2/catch_all.hpp>
#include <Utility/GZip.hpp>
#include <Tests/TestData.hpp>
using namespace Enigma;
using namespace Catch;
using namespace Catch::Matchers;

TEST_CASE("Compress & Decompress Buffer", "GZip")
{
	using std::cout, std::cin, std::endl;
	cout << "\n======[ " << Catch::getResultCapture().getCurrentTestName() << " ]======\n";

	String compressed, decompressed;
	String buffer = LOREM_IPSUM * 3000; // 3000 paragraphs of lorem ipsum dummy text
	cout << "Buffer Size: " << ENIGMA_BYTES_TO_MB( buffer.size() ) << "mb\n";
	compressed = GZip::Compress(buffer);
	cout << "Compressed Size: " << ENIGMA_BYTES_TO_MB(compressed.size()) << "mb\n";
	decompressed = GZip::Decompress(compressed);
	cout << "Decompressed Size: " << ENIGMA_BYTES_TO_MB(decompressed.size()) << "mb\n";
	
	REQUIRE_THAT(compressed, !Equals(buffer));
	REQUIRE(compressed.size() < buffer.size());
	REQUIRE_THAT(decompressed, Equals(buffer));
	REQUIRE(decompressed.size() == buffer.size());
}
