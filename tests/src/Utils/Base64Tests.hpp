#pragma once
#include <Core/Core.hpp>
#include <catch2/catch_all.hpp>
#include <Utility/Base64.hpp>
#include <Utility/SizeUtils.hpp>
#include <Algorithm/AES/AES.hpp>
#include "TestsData.hpp"

using namespace Enigma;
using namespace Catch;
using namespace Catch::Matchers;

#define BEGIN_TIMER(var) auto var = std::chrono::steady_clock::now()
#define END_TIMER(var, unit)  std::chrono::duration_cast<std::chrono::unit>(std::chrono::steady_clock::now() - var).count()

TEST_CASE("Base64 Encode And Decode Cipher With AES", "Base64")
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
	// Encode cipher to Base64
	std::string encoded_cipher = Base64::Encode(encrypted);
	// Decode encoded cipher base64 to cipher
	std::string decoded_cipher = Base64::Decode(encoded_cipher);
	// Decrypted cipher (aka recovered)
	std::string decrypted = aes->Decrypt(password, decoded_cipher);
	

	// Decoded must match cipher
	REQUIRE_THAT(decoded_cipher, Equals(encrypted));
	// Encoded must not match cipher
	REQUIRE_THAT(encoded_cipher, !Equals(encrypted));
	// Buffer must not match cipher
	REQUIRE_THAT(buffer, !Equals(encrypted));
	// Buffer must match decrypted cipher
	REQUIRE_THAT(buffer, Equals(decrypted));

}

TEST_CASE("Encode and Decode text", "Base64")
{
	std::cout << "\n======[ " << Catch::getResultCapture().getCurrentTestName() << " ]======\n";

	std::string buffer = LOREM_IPSUM * Random::Int<std::size_t>(10, 10000);

	BEGIN_TIMER(t1);
    std::string base64_encoded = Base64::Encode(buffer);
	auto en_ms = END_TIMER(t1, milliseconds);
	std::cout << "Encoded " << SizeUtils::FriendlySize(buffer.size()) << " in " << en_ms << "ms\n";

	BEGIN_TIMER(t2);
	std::string base64_decoded = Base64::Decode(base64_encoded);
	auto de_ms = END_TIMER(t2, milliseconds);
	std::cout << "Decoded " << SizeUtils::FriendlySize(base64_encoded.size()) << " in " << de_ms << "ms\n";

	REQUIRE(buffer != base64_encoded);
	REQUIRE(buffer == base64_decoded);
}
