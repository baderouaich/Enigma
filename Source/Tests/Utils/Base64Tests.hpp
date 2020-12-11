#pragma once
#include <Core/Core.hpp>
#ifdef ENIGMA_DEBUG
#include <catch2/catch_all.hpp>
#include <Utility/Base64.hpp>
#include <Algorithm/AES/AES.hpp>
using namespace Enigma;
using namespace Catch;
using namespace Catch::Matchers;

TEST_CASE("Encode And Decode Cipher", "Base64")
{
	using std::cout, std::cin, std::endl;

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

    String buffer = R"(
Lorem Ipsum is simply dummy text of the printing and typesetting industry.
Lorem Ipsum has been the industry's standard dummy text ever since the 1500s,
when an unknown printer took a galley of type and scrambled it to make a type specimen book.
It has survived not only five centuries, but also the leap into electronic typesetting,
remaining essentially unchanged. It was popularised in the 1960s with the release
of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop
publishing software like Aldus PageMaker including versions of Lorem Ipsum.
)";

    //cout << "Buffer: " << buffer << endl;
    String base64_encoded = Base64::Encode(buffer);
    //cout << "Base64 encoded: " << base64_encoded << endl;
    String base64_decoded = Base64::Decode(base64_encoded);
    //cout << "Base64 decoded: " << base64_decoded << endl;

   REQUIRE(buffer != base64_encoded);
   REQUIRE(buffer == base64_decoded);
	
}


#endif
