#pragma once
#include <catch2/catch_all.hpp>
#include <Algorithm/AES/AES.hpp>
#include <Utility/FileUtils.hpp>
#include <System/Dialogs/OpenFileDialog.hpp>
using namespace Enigma;
using namespace Catch::Matchers;
#define TEST_FILES false

#if TEST_FILES
String GetSelectedFilePath()
{
	Enigma::OpenFileDialog ofd("Select a file to encrypt", ".", false);
	return *ofd.Show().begin();
}

TEST_CASE("AES File Encryption and Decryption")
{
	using std::cout, std::cin, std::cerr, std::endl, std::getline;

	std::unique_ptr<Enigma::AES> aes_encryptor = std::make_unique<Enigma::AES>(Enigma::AES::Intent::Encrypt);
	std::unique_ptr<Enigma::AES> aes_decryptor = std::make_unique<Enigma::AES>(Enigma::AES::Intent::Decrypt);

	String buffer, password, filename_to_encrypt;
	String encrypted, decrypted;

	// Encryption Password
	{
		cout << "\nEnter password (encryption key): ";
		getline(cin, password);
	}
	//File to encrypt
	{
		cout << "\nSelect a file to encrypt: ";
		Enigma::OpenFileDialog ofd("Select a file to encrypt", ".", false);
		const auto selections = ofd.Show();
		if (selections.empty())
		{
			cerr << "No file was selected\n";
			exit(EXIT_FAILURE);
		}
		filename_to_encrypt = selections.back();
	}
	if (!fs::exists(filename_to_encrypt)) 
	{
		cerr << "File does not exist\n";
		exit(EXIT_FAILURE);
	}
	if (!FileUtils::Read(filename_to_encrypt, buffer))
	{
		cerr << "Failed to read file\n";
		exit(EXIT_FAILURE);
	}
	{
		encrypted = aes_encryptor->Encrypt(password, buffer); // iv + cipher
		if(FileUtils::Write(filename_to_encrypt + ".encrypted.enigma", encrypted))
			cout << "\nEncrypted buffer saved to : " << filename_to_encrypt + ".enigma" << endl;

	}
	{
		decrypted = aes_decryptor->Decrypt(password, encrypted);
		if (FileUtils::Write(filename_to_encrypt + ".decrypted.enigma", decrypted))
			cout << "\nDecrypted buffer saved to " << filename_to_encrypt + ".decrypted.enigma" << endl;
	}

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

#else
TEST_CASE("AES Encryption and Decryption")
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
#endif
