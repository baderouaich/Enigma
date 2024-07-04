#pragma once
#include <Enigma.hpp>
#include <iostream>
#include <catch2/catch_all.hpp>
#include <Utility/FileUtils.hpp>
#include "TestsData.hpp"
using namespace Enigma;
using namespace Catch;
using namespace Catch::Matchers;

TEST_CASE("File Utils Tests", "FileUtils")
{
	std::cout << "\n======[ " << Catch::getResultCapture().getCurrentTestName() << " ]======\n";

	const auto filename = fs::path(TEST_DATA_DIR) / "text_file.txt";

	std::vector<Enigma::byte> buffer{};

	const std::size_t max_chunk_size = 8192; // 8KB per chunk
	Enigma::FileUtils::ReadChunks(filename, max_chunk_size,
		[&buffer](std::vector<Enigma::byte>&& chunk) -> bool
		{
			//std::cout << "Read Chunk Size: " << chunk.size() << std::endl;
			buffer.insert(buffer.begin(), chunk.begin(), chunk.end());

			return true; // keep reading chunks.. don't stop
		});

	// buffer size must match file size
	REQUIRE(buffer.size() == fs::file_size(filename));

	// buffer must match buffer from file
	std::string buffer_from_file;
	REQUIRE(FileUtils::Read(filename, buffer_from_file));
	REQUIRE(buffer.size() == buffer_from_file.size());
}
