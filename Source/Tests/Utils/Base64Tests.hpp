#pragma once
#include <catch2/catch_all.hpp>
#include <Utility/Base64.hpp>
#include <Algorithm/AES/AES.hpp>
#include <Tests/TestData.hpp>
using namespace Enigma;
using namespace Catch;
using namespace Catch::Matchers;

#define BEGIN_TIMER(var) auto var = std::chrono::steady_clock::now()
#define END_TIMER(var, unit)  std::chrono::duration_cast<std::chrono::unit>(std::chrono::steady_clock::now() - var).count()


TEST_CASE("Base64 Encode And Decode Cipher With AES", "Base64")
{
	using std::cout, std::cin, std::endl;
	cout << "\n======[ " << Catch::getResultCapture().getCurrentTestName() << " ]======\n";

	std::unique_ptr<Enigma::AES> aes_encryptor = std::make_unique<Enigma::AES>(Enigma::AES::Intent::Encrypt);
	std::unique_ptr<Enigma::AES> aes_decryptor = std::make_unique<Enigma::AES>(Enigma::AES::Intent::Decrypt);

    String buffer, password;
    String encrypted, decrypted;
    String encrypted_encoded_base64, encrypted_decoded_base64;

	cout << "Base64 - Enter buffer to encrypt: " << endl;
    getline(cin, buffer);
	cout << "Base64 - Enter password (encryption key): " << endl;
    getline(cin, password);


    encrypted = aes_encryptor->Encrypt(password, buffer); // iv + cipher
    cout << "\nEncrypted: " << encrypted;
    encrypted_encoded_base64 = Base64::Encode(encrypted);
    cout << "\nEncrypted encoded base64: " << encrypted_encoded_base64;
    encrypted_decoded_base64 = Base64::Decode(encrypted_encoded_base64);
    cout << "\nEncrypted decoded base64: " << encrypted_decoded_base64 << endl;
	decrypted = aes_decryptor->Decrypt(password, encrypted_decoded_base64);
    cout << "\nDecrypted decoded base64: " << decrypted << endl;
	
	REQUIRE_THAT(encrypted, Equals(encrypted_decoded_base64));
	REQUIRE_THAT(buffer, Equals(decrypted));
}

TEST_CASE("Encode_And_Decode_Text", "Base64")
{
	using std::cout, std::cin, std::endl;


	BEGIN_TIMER(t1);
    String base64_encoded = Base64::Encode(buffer);
	auto en_ms = END_TIMER(t1, milliseconds);
	cout << "Encoded " << buffer.size() << " bytes in " << en_ms << "ms\n";

	BEGIN_TIMER(t2);
	String base64_decoded = Base64::Decode(base64_encoded);
	auto de_ms = END_TIMER(t2, milliseconds);
	cout << "Decoded " << base64_encoded.size() << " bytes in " << de_ms << "ms\n";

   REQUIRE(buffer != base64_encoded);
   REQUIRE(buffer == base64_decoded);
	
}
