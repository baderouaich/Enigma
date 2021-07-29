#pragma once
#include <catch2/catch_all.hpp>
#include <Algorithm/Twofish/Twofish.hpp>
#include <Tests/TestData.hpp>
#include <Utility/SizeUtils.hpp>

using namespace Enigma;
using namespace Catch::Matchers;


TEST_CASE("Twofish Encryption and Decryption")
{
	std::cout << "\n======[ " << Catch::getResultCapture().getCurrentTestName() << " ]======\n";

	// Make TripleDES algorithm with intention to Encrypt and Decrypt
	std::unique_ptr<Twofish> twofish(new Twofish(Twofish::Intent::Encrypt | Twofish::Intent::Decrypt));

	// Buffer to encrypt
	String buffer = GenerateRandomString(ENIGMA_MB_TO_BYTES(Random::Int<size_t>(1, 50)));
	// Encryption password
	String password = GenerateRandomString(ENIGMA_MB_TO_BYTES(Random::Int<size_t>(1, 5)));

	// Encrypted buffer (aka cipher)
	String encrypted = twofish->Encrypt(password, buffer);
	// Decrypted cipher (aka recovered)
	String decrypted = twofish->Decrypt(password, encrypted);

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
