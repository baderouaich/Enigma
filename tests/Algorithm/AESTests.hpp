#pragma once
#include <catch2/catch_all.hpp>
#include <Algorithm/AES/AES.hpp>
#include <Utility/FileUtils.hpp>
#include <Utility/SizeUtils.hpp>
#include <Utility/Random.hpp>
#include <System/Dialogs/OpenFileDialog.hpp>
#include <Tests/TestsData.hpp>

using namespace Enigma;
using namespace Catch::Matchers;
using namespace std;

#define TESTING_FILES false

#if TESTING_FILES

String GetSelectedFilePath()
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
	String password = Random::Str(4096);

	// File to encrypt
	String filename_to_encrypt = GetSelectedFilePath();
	String buffer;
	if (!FileUtils::Read(filename_to_encrypt, buffer))
	{
		std::cerr << "Failed to read file\n";
		std::exit(EXIT_FAILURE);
	}
	
	// Encrypt file buffer
	String encrypted = aes->Encrypt(password, buffer);

	// Decrypt file buffer
	String decrypted = aes->Decrypt(password, encrypted);
	
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
	String buffer = Random::Str(ENIGMA_MB_TO_BYTES(Random::Int<std::size_t>(1, 50)), true);
	// Encryption password
	String password = Random::Str(ENIGMA_MB_TO_BYTES(Random::Int<std::size_t>(1, 5)), true);

	// Encrypted buffer (aka cipher)
	String encrypted = aes->Encrypt(password, buffer);
	// Decrypted cipher (aka recovered)
	String decrypted = aes->Decrypt(password, encrypted);

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
