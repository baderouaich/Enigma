#pragma once
#include <catch2/catch_all.hpp>
#include <Algorithm/IDEA/IDEA.hpp>
#include <Utility/SizeUtils.hpp>
#include <Tests/TestsData.hpp>

using namespace Enigma;
using namespace Catch::Matchers;

TEST_CASE("IDEA Encryption and Decryption")
{
	std::cout << "\n======[ " << Catch::getResultCapture().getCurrentTestName() << " ]======\n";
	
	// Make IDEA algorithm with intention to Encrypt and Decrypt
	std::unique_ptr<IDEA> idea(new IDEA(IDEA::Intent::Encrypt | IDEA::Intent::Decrypt));

	// Buffer to encrypt
	String buffer = Random::Str(ENIGMA_MB_TO_BYTES(Random::Int<size_t>(1, 50)), true);
	// Encryption password
	String password = Random::Str(ENIGMA_MB_TO_BYTES(Random::Int<size_t>(1, 5)), true);

	// Encrypted buffer (aka cipher)
	String encrypted = idea->Encrypt(password, buffer);
	// Decrypted cipher (aka recovered)
	String decrypted = idea->Decrypt(password, encrypted);

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
