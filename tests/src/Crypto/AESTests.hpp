#pragma once
#include <catch2/catch_all.hpp>
#include <Algorithm/AES/AES.hpp>
#include <Utility/FileUtils.hpp>
#include <Utility/SizeUtils.hpp>
#include <Utility/Random.hpp>
#include <System/Dialogs/OpenFileDialog.hpp>
#include "TestsData.hpp"

using namespace Enigma;
using namespace Catch::Matchers;
using namespace std;

#define TESTING_FILES false

#if TESTING_FILES

std::string GetSelectedFilePath()
{
	Enigma::OpenFileDialog ofd("Select a file to encrypt", ".", false);
	if (const auto files = ofd.Show(); !files.empty())
	{
		return files[0];
	}
	else
	{
		std::cout << "No files were selected.\n";
		return GetSelectedFilePath(); // i'l be damned if i let u go without selecting a file XD (its just testing i mean no harm :v)
	}
	
}

TEST_CASE("AES File Encryption and Decryption")
{
	std::unique_ptr<AES> aes(new AES(AES::Intent::Encrypt | AES::Intent::Decrypt));

	// Encryption password
	std::string password = Random::Str(4096);

	// File to encrypt
	std::string filename_to_encrypt = GetSelectedFilePath();
	std::string buffer;
	if (!FileUtils::Read(filename_to_encrypt, buffer))
	{
		std::cerr << "Failed to read file\n";
		std::exit(EXIT_FAILURE);
	}
	
	// Encrypt file buffer
	std::string encrypted = aes->Encrypt(password, buffer);

	// Decrypt file buffer
	std::string decrypted = aes->Decrypt(password, encrypted);
	
	// Buffer must not match cipher
	REQUIRE_THAT(buffer, !Equals(encrypted));
	// Buffer must match decrypted cipher
	REQUIRE_THAT(buffer, Equals(decrypted));


	SECTION("Clearing buffers")
	{
		buffer.clear();
		password.clear();
		encrypted.clear();
		decrypted.clear();

		REQUIRE(buffer.size() == 0);
		REQUIRE(password.size() == 0);
		REQUIRE(encrypted.size() == 0);
		REQUIRE(decrypted.size() == 0);
	}
}

#else

TEST_CASE("AES-GCM Encryption and Decryption")
{
	std::cout << "\n======[ " << Catch::getResultCapture().getCurrentTestName() << " ]======\n";

	// Make AES algorithm with intention to Encrypt and Decrypt
	std::unique_ptr<AES> aes(new AES(AES::Intent::Encrypt | AES::Intent::Decrypt));

	// Buffer to encrypt
	std::string buffer = Random::Str(ENIGMA_MB_TO_BYTES(Random::Int<std::size_t>(1, 50)));
	// Encryption password
	std::string password = Random::Str(ENIGMA_MB_TO_BYTES(Random::Int<std::size_t>(1, 5)));

	// Encrypted buffer (aka cipher)
	std::string encrypted = aes->Encrypt(password, buffer);
	// Decrypted cipher (aka recovered)
	std::string decrypted = aes->Decrypt(password, encrypted);

	SECTION("Comparing buffers")
	{
		// Buffer must not match cipher
		REQUIRE_THAT(buffer, !Equals(encrypted));
		// Buffer must match decrypted cipher
		REQUIRE_THAT(buffer, Equals(decrypted));
	}

	SECTION("Clearing buffers")
	{
		buffer.clear();
		password.clear();
		encrypted.clear();
		decrypted.clear();

		REQUIRE(buffer.size() == 0);
		REQUIRE(password.size() == 0);
		REQUIRE(encrypted.size() == 0);
		REQUIRE(decrypted.size() == 0);
	}
}
#endif
