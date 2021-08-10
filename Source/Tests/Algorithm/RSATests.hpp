#pragma once
#include <catch2/catch_all.hpp>
#include <Algorithm/RSA/RSA.hpp>
#include <Utility/SizeUtils.hpp>
#include <Tests/TestsData.hpp>

using namespace Enigma;
using namespace Catch::Matchers;

TEST_CASE("RSA Encryption and Decryption")
{
	std::cout << "\n======[ " << Catch::getResultCapture().getCurrentTestName() << " ]======\n";

	// Make IDEA algorithm with intention to Encrypt and Decrypt
	std::unique_ptr<RSA> rsa(new RSA(RSA::Intent::Encrypt | RSA::Intent::Decrypt));
	rsa->SetKeySize(30720);
	// Buffer to encrypt
	String buffer = "Hello World";

	std::cout << "Encrypting... " << std::endl << std::endl;
	String cipher = rsa->Encrypt("", buffer);

	String private_key = rsa->GetPrivateKey();
	String public_key = rsa->GetPublicKey();


	std::cout << "buffer: " << buffer << std::endl << std::endl;
	std::cout << "cipher: " << cipher << std::endl << std::endl;
	std::cout << "private_key: " << private_key << std::endl << std::endl;
	std::cout << "public_key: " << public_key << std::endl << std::endl;
	/*
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
	}*/
}
