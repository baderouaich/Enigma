#pragma once
#include <catch2/catch_all.hpp>
#include <Algorithm/ChaCha20/ChaCha20.hpp>
using namespace Enigma;
using namespace Catch::Matchers;


TEST_CASE("ChaCha20 Encryption and Decryption")
{
	using std::cout, std::cin, std::endl;
	cout << "\n======[ " << Catch::getResultCapture().getCurrentTestName() << " ]======\n";

	std::unique_ptr<Enigma::ChaCha20> chacha_encryptor = std::make_unique<Enigma::ChaCha20>(Enigma::ChaCha20::Intent::Encrypt);
	std::unique_ptr<Enigma::ChaCha20> chacha_decryptor = std::make_unique<Enigma::ChaCha20>(Enigma::ChaCha20::Intent::Decrypt);

	String buffer, password;
	String encrypted, decrypted;

	cout << "\nEnter buffer to encrypt: ";
	getline(cin, buffer);
	cout << "\nEnter password (encryption key): ";
	getline(cin, password);


	encrypted = chacha_encryptor->Encrypt(password, buffer); // iv + cipher
	cout << "\nEncrypted: " << encrypted << endl;

	decrypted = chacha_decryptor->Decrypt(password, encrypted);
	cout << "\nDecrypted: " << decrypted << endl;

	REQUIRE_THAT(buffer, !Equals(encrypted));
	REQUIRE_THAT(buffer, Equals(decrypted));


	SECTION("Clearing buffers")
	{
		buffer.clear();
		encrypted.clear();
		decrypted.clear();

		REQUIRE(buffer.size() == 0);
		REQUIRE(encrypted.size() == 0);
		REQUIRE(decrypted.size() == 0);
	}
}
