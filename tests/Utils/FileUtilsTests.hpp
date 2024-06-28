#pragma once
#include <Enigma.hpp>
#include <iostream>
#include <catch2/catch_all.hpp>
#include <Utility/FileUtils.hpp>
#include <Tests/TestsData.hpp>

using namespace Enigma;
using namespace Catch;
using namespace Catch::Matchers;

TEST_CASE("File Utils Tests", "FileUtils")
{
	std::cout << "\n======[ " << Catch::getResultCapture().getCurrentTestName() << " ]======\n";

	const auto filename = FileUtils::GetEnigmaExecutableDir() / fs::path(Enigma::Constants::Config::WINDOW_CONFIG_FILE_PATH);

	std::vector<Enigma::byte> buffer{};

	const std::size_t max_chunk_size = 9; // 9 bytes per chunk
	Enigma::FileUtils::ReadChunks(filename, max_chunk_size,
		[&buffer](std::vector<Enigma::byte>&& chunk) -> bool
		{
			std::cout << "Read Chunk Size: " << chunk.size() << std::endl;
			buffer.insert(buffer.begin(), chunk.begin(), chunk.end());

			return false; // keep reading chunks,, don't stop
		});

	// buffer size must match file size
	REQUIRE(buffer.size() == fs::file_size(filename));

	// buffer must match buffer from file
	String buffer_from_file;
	REQUIRE(FileUtils::Read(filename, buffer_from_file));
	REQUIRE(buffer.size() == buffer_from_file.size());
}
