#pragma once
#include <catch2/catch_all.hpp>
#include <Utility/GZip.hpp>
using namespace Enigma;
using namespace Catch;
using namespace Catch::Matchers;

const String buffer = R"(
Lorem Ipsum is simply dummy text of the printing and typesetting industry.
Lorem Ipsum has been the industry's standard dummy text ever since the 1500s,
when an unknown printer took a galley of type and scrambled it to make a type specimen book.
It has survived not only five centuries, but also the leap into electronic typesetting,
remaining essentially unchanged. It was popularised in the 1960s with the release
of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop
publishing software like Aldus PageMaker including versions of Lorem Ipsum.
)";

TEST_CASE("Compress & Decompress Buffer", "GZip")
{
	using std::cout, std::cin, std::endl;
	cout << "\n======[ " << Catch::getResultCapture().getCurrentTestName() << " ]======\n";

	String compressed, decompressed;
	
	cout << "Buffer Size: " << buffer.size() << '\n';
	compressed = GZip::Compress(buffer);
	cout << "Compressed Size: " << compressed.size() << '\n';
	decompressed = GZip::Decompress(compressed);
	cout << "Decompressed Size: " << decompressed.size() << '\n';
	
	REQUIRE_THAT(compressed, !Equals(buffer));
	REQUIRE(compressed.size() < buffer.size());
	REQUIRE_THAT(decompressed, Equals(buffer));
	REQUIRE(decompressed.size() == buffer.size());
}
