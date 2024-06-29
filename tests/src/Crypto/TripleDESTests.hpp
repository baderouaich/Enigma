#pragma once
#include <catch2/catch_all.hpp>
#include <Algorithm/TripleDES/TripleDES.hpp>
#include <Utility/SizeUtils.hpp>
#include "TestsData.hpp"

using namespace Enigma;
using namespace Catch::Matchers;


TEST_CASE("TripleDES Encryption and Decryption")
{
	std::cout << "\n======[ " << Catch::getResultCapture().getCurrentTestName() << " ]======\n";
	
	// Make TripleDES algorithm with intention to Encrypt and Decrypt
	std::unique_ptr<TripleDES> idea(new TripleDES(TripleDES::Intent::Encrypt | TripleDES::Intent::Decrypt));

	// Buffer to encrypt
	std::string buffer = Random::Str(ENIGMA_MB_TO_BYTES(Random::Int<std::size_t>(1, 50)));
	// Encryption password
	std::string password = Random::Str(ENIGMA_MB_TO_BYTES(Random::Int<std::size_t>(1, 5)));

	// Encrypted buffer (aka cipher)
	std::string encrypted = idea->Encrypt(password, buffer);
	// Decrypted cipher (aka recovered)
	std::string decrypted = idea->Decrypt(password, encrypted);

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
