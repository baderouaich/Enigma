#pragma once
#include <catch2/catch_all.hpp>
#include <Algorithm/RSA/RSA.hpp>
#include <Utility/SizeUtils.hpp>
#include <Tests/TestsData.hpp>

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
	String buffer = Random::Str(max_buffer_length);

	std::cout << "Encrypting... " << std::endl << std::endl;
	String cipher = rsa_encryptor->Encrypt("", buffer);

	String private_key = rsa_encryptor->GetPrivateKey();
	String public_key = rsa_encryptor->GetPublicKey();
	std::cout << "buffer: " << buffer << std::endl << std::endl;
	std::cout << "cipher: " << cipher << std::endl << std::endl;
	std::cout << "private_key: \n" << private_key << std::endl << std::endl;
	std::cout << "public_key: \n" << public_key << std::endl << std::endl;
	
	std::cout << "\nDecrypting... " << std::endl << std::endl;
	rsa_decryptor->SetPrivateKey(private_key);
	String recovered = rsa_decryptor->Decrypt("", cipher);
	std::cout << "recovered: " << recovered << std::endl << std::endl;

	REQUIRE(buffer != cipher);
	REQUIRE(buffer == recovered);
#else
	try
	{
		ENIGMA_BEGIN_TIMER(t1);
		// Make IDEA algorithm with intention to Encrypt and Decrypt
		std::unique_ptr<RSA> rsa(new RSA(RSA::Intent::Encrypt | RSA::Intent::Decrypt));
		rsa->SetKeySize(8192);
		// Buffer to encrypt
		auto max_buffer_length = rsa->GetMaximumBufferSize();
		std::cout << "maximum buffer length u can encrypt with keysize " << rsa->GetKeySize() << " is " << max_buffer_length << std::endl;
		String buffer = Random::Str(max_buffer_length);

		std::cout << "Encrypting... " << std::endl << std::endl;
		String cipher = rsa->Encrypt("", buffer);
		auto s = ENIGMA_END_TIMER(t1, f64, std::milli) / 1000.0;

		String private_key = rsa->GetPrivateKey();
		String public_key = rsa->GetPublicKey();
		std::cout << "took: " << s << "s to generate " << rsa->GetKeySize() << std::endl << std::endl;
		std::cout << "buffer: " << buffer << std::endl << std::endl;
		std::cout << "cipher: " << cipher << std::endl << std::endl;
		std::cout << "private_key: \n" << private_key << std::endl << std::endl;
		std::cout << "public_key: \n" << public_key << std::endl << std::endl;


		std::cout << "\nDecrypting... " << std::endl << std::endl;
		rsa->SetPrivateKey(private_key);
		rsa->SetPublicKey(public_key);
		String recovered = rsa->Decrypt("", cipher);
		std::cout << "recovered: " << recovered << std::endl << std::endl;

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
