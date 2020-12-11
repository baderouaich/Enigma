#pragma once
#include <catch2/catch_all.hpp>
#include <Algorithm/AES/AES.hpp>
using namespace Enigma;
using namespace Catch::Matchers;


TEST_CASE("AES Encryption and AES Decryption")
{
	using std::cout, std::cin, std::endl;

	std::unique_ptr<Enigma::AES> aes_encryptor = std::make_unique<Enigma::AES>(Enigma::AES::Intent::Encrypt);
	std::unique_ptr<Enigma::AES> aes_decryptor = std::make_unique<Enigma::AES>(Enigma::AES::Intent::Decrypt);

	String buffer, password;
	String encrypted, decrypted;

	cout << "\nEnter buffer to encrypt: ";
	getline(cin, buffer);
	cout << "\nEnter password (encryption key): ";
	getline(cin, password);


	encrypted = aes_encryptor->Encrypt(password, buffer); // iv + cipher
	cout << "\nEncrypted: " << encrypted << endl;

	decrypted = aes_decryptor->Decrypt(password, encrypted);
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
