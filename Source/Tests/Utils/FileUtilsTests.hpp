#pragma once
#include <Enigma.hpp>
#include <iostream>
#include <catch2/catch_all.hpp>
using namespace Enigma;
using namespace Catch;
using namespace Catch::Matchers;

TEST_CASE("File Utils Tests", "FileUtils")
{
	std::cout << "\n======[ " << Catch::getResultCapture().getCurrentTestName() << " ]======\n";

	constexpr auto filename = Enigma::Constants::Config::WINDOW_CONFIG_FILE_PATH;

	std::vector<Enigma::byte> buffer{};

	size_t max_chunk_size = 9; // 9 bytes
	Enigma::FileUtils::ReadChunks(filename, max_chunk_size,
		[&buffer](std::vector<Enigma::byte>&& chunk) -> void 
		{
			std::cout << "Read Chunk Size: " << chunk.size() << std::endl;
			buffer.insert(buffer.begin(), chunk.begin(), chunk.end());
		});

	REQUIRE(buffer.size() == fs::file_size(filename));
}
