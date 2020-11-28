#pragma once
#include <catch2/catch_all.hpp>
#include "Algorithm/AES/AESEncryption.hpp"
#include "Algorithm/AES/AESDecryption.hpp"
using namespace Enigma;
using namespace Catch::Matchers;


TEST_CASE("AESEncryption & AESDecryption")
{
	using std::cout, std::cin, std::endl;

	UniquePtr<AESEncryption> aes_encyptor = MakeUnique<AESEncryption>();
	UniquePtr<AESDecryption> aes_decryptor = MakeUnique<AESDecryption>();

	String buffer, password;
	String encrypted, decrypted;

	cout << "\nEnter buffer to encrypt: ";
	getline(cin, buffer);
	cout << "\nEnter password (encryption key): ";
	getline(cin, password);

	// now let's benchmark:
	//BENCHMARK("AESEncryption::Encrypt") 
	//{
	//	encrypted = aes_encyptor->Encrypt(password, buffer); // iv + cipher
	//};
	encrypted = aes_encyptor->Encrypt(password, buffer); // iv + cipher
	cout << "\nEncrypted: " << encrypted << endl;
	auto s =GENERATE(Catch::Generators::as<String>{}, "aa", "b", "c");

	// now let's benchmark:
	//BENCHMARK("AESDecryption::Decrypt")
	//{
	//	decrypted = aes_decryptor->Decrypt(password, encrypted);
	//};
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
