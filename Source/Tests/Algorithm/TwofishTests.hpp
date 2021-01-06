#pragma once
#include <catch2/catch_all.hpp>
#include <Algorithm/Twofish/Twofish.hpp>
using namespace Enigma;
using namespace Catch::Matchers;
using namespace std;


TEST_CASE("Twofish Encryption and Decryption")
{
	cout << "\n======[ " << Catch::getResultCapture().getCurrentTestName() << " ]======\n";

	std::unique_ptr<Enigma::Twofish> twofish_encryptor = std::make_unique<Enigma::Twofish>(Enigma::Twofish::Intent::Encrypt);
	std::unique_ptr<Enigma::Twofish> twofish_decryptor = std::make_unique<Enigma::Twofish>(Enigma::Twofish::Intent::Decrypt);

	String buffer, password;
	String encrypted, decrypted;

	cout << "\nEnter buffer to encrypt: ";
	getline(cin, buffer);
	cout << "\nEnter password (encryption key): ";
	getline(cin, password);


	encrypted = twofish_encryptor->Encrypt(password, buffer); // iv + cipher
	cout << "\nEncrypted: " << encrypted << endl;

	decrypted = twofish_decryptor->Decrypt(password, encrypted);
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
