#pragma once
#include <Algorithm/RSA/RSA.hpp>
#include "TestsData.hpp"
#include "Utility/DateTimeUtils.hpp"
#include <iostream>
#include "Utility/FinalAction.hpp"
#include "Utility/HashUtils.hpp"
#include <System/Dialogs/OpenFileDialog.hpp>
#include <System/Dialogs/SaveFileDialog.hpp>
#include <Utility/FileUtils.hpp>
#include <Utility/Random.hpp>
#include <Utility/SizeUtils.hpp>
#include <catch2/catch_all.hpp>

using namespace Enigma;
using namespace Catch::Matchers;

using namespace Enigma;
using namespace Catch::Matchers;

TEST_CASE("RSA Encryption and Decryption")
{
#if 0
	std::cout << "\n======[ " << Catch::getResultCapture().getCurrentTestName() << " ]======\n";
	std::unique_ptr<RSA> rsa_encryptor(new RSA(RSA::Intent::Encrypt));
	std::unique_ptr<RSA> rsa_decryptor(new RSA(RSA::Intent::Decrypt));
	rsa_encryptor->SetKeySize(4096);
	auto max_buffer_length = rsa_encryptor->GetMaximumBufferSize();
	std::cout << "maximum buffer length u can encrypt with keysize " << rsa_encryptor->GetKeySize() << " is " << max_buffer_length << std::endl;
	std::string buffer = Random::Str(max_buffer_length);

	std::cout << "Encrypting... " << std::endl << std::endl;
	std::string cipher = rsa_encryptor->Encrypt("", buffer);

	std::string private_key = rsa_encryptor->GetPrivateKey();
	std::string public_key = rsa_encryptor->GetPublicKey();
	std::cout << "buffer: " << buffer << std::endl << std::endl;
	std::cout << "cipher: " << cipher << std::endl << std::endl;
	std::cout << "private_key: \n" << private_key << std::endl << std::endl;
	std::cout << "public_key: \n" << public_key << std::endl << std::endl;
	
	std::cout << "\nDecrypting... " << std::endl << std::endl;
	rsa_decryptor->SetPrivateKey(private_key);
	std::string recovered = rsa_decryptor->Decrypt("", cipher);
	std::cout << "recovered: " << recovered << std::endl << std::endl;

	REQUIRE(buffer != cipher);
	REQUIRE(buffer == recovered);
#else
	try
	{
		ENIGMA_BEGIN_TIMER(t1);
		// Make IDEA algorithm with intention to Encrypt and Decrypt
		std::unique_ptr<RSA> encrypter(new RSA(RSA::Intent::Encrypt));
    const std::size_t keySize = 1024*8;
    RSA::RSASettings settings =
      {
        .keySize = keySize
      };
    encrypter->setSettings(std::move(settings));
		// Buffer to encrypt
		std::size_t max_buffer_length = encrypter->getMaximumBufferSize();
		std::cout << "maximum buffer length u can encrypt with keysize " <<keySize << " is " << max_buffer_length << std::endl;
		std::vector<byte> buffer = {'H','e','l','l','o'};

		std::cout << "Encrypting... " << std::endl << std::endl;
    std::vector<byte> cipher = encrypter->Encrypt("", buffer);
		auto s = ENIGMA_END_TIMER(t1, double, std::milli) / 1000.0;

		std::string private_key = encrypter->getPrivateKey();
		std::string public_key = encrypter->getPublicKey();
		std::cout << "took: " << s << "s to encrypt with rsa key size " << keySize << std::endl << std::endl;
		std::cout << "buffer: " << std::string(buffer.begin(), buffer.end()) << std::endl << std::endl;
		std::cout << "cipher: " << std::string(cipher.begin(), cipher.end()) << std::endl << std::endl;
		std::cout << "private_key: \n" << private_key << std::endl << std::endl;
		std::cout << "public_key: \n" << public_key << std::endl << std::endl;


    std::unique_ptr<RSA> decrypter(new RSA(RSA::Intent::Decrypt));
    RSA::RSASettings settings1 =
      {
        .keySize = keySize,
        .privateKey = private_key
      };
    decrypter->setSettings(std::move(settings1));
		std::cout << "\nDecrypting... " << std::endl << std::endl;
		std::cout << "Setting Priv key... " << std::endl;
    decrypter->setPrivateKey(private_key);
    std::cout << "Setting Pub key... " << std::endl;
    decrypter->setPublicKey(public_key);
		std::vector<byte> recovered = decrypter->Decrypt("", cipher);
		std::cout << "recovered: " << std::string(recovered.begin(), recovered.end()) << std::endl << std::endl;

		REQUIRE(buffer != cipher);
		REQUIRE(buffer == recovered);

	}
	catch (const CryptoPP::Exception& e)
	{
		std::cerr << Enigma::CryptoPPUtils::GetFullErrorMessage(e) << '\n';
		REQUIRE(false);
	}
#endif
}

