#pragma once
#include <catch2/catch_all.hpp>
#include <Algorithm/TripleDES/TripleDES.hpp>
using namespace Enigma;
using namespace Catch::Matchers;


TEST_CASE("TripleDES Encryption and Decryption")
{
	using std::cout, std::cin, std::endl;
	cout << "\n======[ " << Catch::getResultCapture().getCurrentTestName() << " ]======\n";

	std::unique_ptr<Enigma::TripleDES> tripledes_encryptor = std::make_unique<Enigma::TripleDES>(Enigma::TripleDES::Intent::Encrypt);
	std::unique_ptr<Enigma::TripleDES> tripledes_decryptor = std::make_unique<Enigma::TripleDES>(Enigma::TripleDES::Intent::Decrypt);

	String buffer, password;
	String encrypted, decrypted;

	cout << "\nEnter buffer to encrypt: ";
	getline(cin, buffer);
	cout << "\nEnter password (encryption key): ";
	getline(cin, password);


	encrypted = tripledes_encryptor->Encrypt(password, buffer); // iv + cipher
	cout << "\nEncrypted: " << encrypted << endl;

	decrypted = tripledes_decryptor->Decrypt(password, encrypted);
	cout << "\nDecrypted: " << decrypted << endl;

	REQUIRE_THAT(buffer, !Equals(encrypted)); // buffer must not equal cipher
	REQUIRE_THAT(buffer, Equals(decrypted)); // buffer must equal decrypted cipher


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
