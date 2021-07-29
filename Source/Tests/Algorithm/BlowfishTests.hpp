#pragma once
#include <catch2/catch_all.hpp>
#include <Algorithm/Blowfish/Blowfish.hpp>
#include <System/Dialogs/OpenFileDialog.hpp>
#include <Utility/FileUtils.hpp>
#include <Tests/TestData.hpp>
#include <Utility/SizeUtils.hpp>
#include <iostream>

using namespace Enigma;
using namespace Catch::Matchers;

TEST_CASE("Blowfish-GCM Encryption and Decryption")
{
	std::cout << "\n======[ " << Catch::getResultCapture().getCurrentTestName() << " ]======\n";

	// Make Blowfish algorithm with intention to Encrypt and Decrypt
	std::unique_ptr<Blowfish> blowfish(new Blowfish(Blowfish::Intent::Encrypt | Blowfish::Intent::Decrypt));

	// Buffer to encrypt
	String buffer = GenerateRandomString(ENIGMA_MB_TO_BYTES(Random::Int<size_t>(1, 50)));
	// Encryption password
	String password = GenerateRandomString(ENIGMA_MB_TO_BYTES(Random::Int<size_t>(1, 5)));

	// Encrypted buffer (aka cipher)
	String encrypted = blowfish->Encrypt(password, buffer);
	// Decrypted cipher (aka recovered)
	String decrypted = blowfish->Decrypt(password, encrypted);

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
