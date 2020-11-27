#pragma once
#include <catch2/catch_all.hpp>
#include "Algorithm/AES/AESEncryption.hpp"
#include "Algorithm/AES/AESDecryption.hpp"
using namespace std;
using namespace Enigma;


TEST_CASE("AESEncryption & AESDecryption")
{
	UniquePtr<AESEncryption> aes_encyptor = MakeUnique<Enigma::AESEncryption>();
	UniquePtr<AESDecryption> aes_decryptor = MakeUnique<Enigma::AESDecryption>();
	String buffer, password;
	String encrypted, decrypted;

	cout << "\nEnter buffer to encrypt: ";
	getline(cin, buffer);
	cout << "\nEnter password (encryption key): ";
	getline(cin, password);


	encrypted = aes_encyptor->Encrypt(password, buffer); // iv + cipher
	cout << "\nEncrypted: " << encrypted;

	decrypted = aes_decryptor->Decrypt(password, encrypted);;
	cout << "\nDecrypted: " << decrypted << endl;

	REQUIRE(buffer != encrypted);
	REQUIRE(buffer == decrypted);



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
