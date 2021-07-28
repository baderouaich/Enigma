#pragma once
#include <catch2/catch_all.hpp>
#include <Algorithm/Blowfish/Blowfish.hpp>
#include <System/Dialogs/OpenFileDialog.hpp>
#include <filesystem>
#include <Utility/FileUtils.hpp>
#include <Tests/TestData.hpp>

namespace fs = std::filesystem;
using namespace Enigma;
using namespace Catch::Matchers;
using namespace std;

#define TEST_FILES false

#if TEST_FILES
String GetSelectedFilePath()
{
	Enigma::OpenFileDialog ofd("Select a file to encrypt", ".", false);
	return *ofd.Show().begin();
}

TEST_CASE("Blowfish File Encryption and Decryption")
{
	using std::cout, std::cin, std::cerr, std::endl, std::getline;

	std::unique_ptr<Enigma::Blowfish> blowfish_encryptor = std::make_unique<Enigma::Blowfish>(Enigma::Blowfish::Intent::Encrypt);
	std::unique_ptr<Enigma::Blowfish> blowfish_decryptor = std::make_unique<Enigma::Blowfish>(Enigma::Blowfish::Intent::Decrypt);

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
		encrypted = blowfish_encryptor->Encrypt(password, buffer); // iv + cipher
		if (FileUtils::Write(filename_to_encrypt + ".encrypted.enigma", encrypted))
			cout << "\nEncrypted buffer saved to : " << filename_to_encrypt + ".enigma" << endl;

	}
	{
		decrypted = blowfish_decryptor->Decrypt(password, encrypted);
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
TEST_CASE("Blowfish-GCM Encryption and Decryption")
{
	cout << "\n======[ " << Catch::getResultCapture().getCurrentTestName() << " ]======\n";

	std::unique_ptr<Enigma::Blowfish> blowfish_encryptor = std::make_unique<Enigma::Blowfish>(Enigma::Blowfish::Intent::Encrypt);
	std::unique_ptr<Enigma::Blowfish> blowfish_decryptor = std::make_unique<Enigma::Blowfish>(Enigma::Blowfish::Intent::Decrypt);

	String buffer, password;
	String encrypted, decrypted;

	buffer = RandomString(4096);
	password = RandomString(1024);

	//cout << "\nEnter buffer to encrypt: ";
	//getline(cin, buffer);
	//cout << "\nEnter password (encryption key): ";
	//getline(cin, password);


	encrypted = blowfish_encryptor->Encrypt(password, buffer); // iv + cipher
	cout << "\nEncrypted: " << encrypted << endl;

	decrypted = blowfish_decryptor->Decrypt(password, encrypted);
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
#endif
