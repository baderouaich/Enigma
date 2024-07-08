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

TEST_CASE("RSA getMaximumBufferSizeFromKeySize") {
  const std::size_t keySizes[] = {1024, 2048, 3072, 4096 /* 8192, 16384, 32768*/};
  for (std::size_t keySize: keySizes) {
    std::unique_ptr<RSA> rsa(new RSA(RSA::Intent::Encrypt));
    RSA::RSASettings settings{};
    settings.keySize = keySize;
    rsa->setSettings(std::move(settings));
    REQUIRE(rsa->getMaximumBufferSize() == RSA::getMaximumBufferSizeFromKeySize(keySize));
  }
}

TEST_CASE("RSA Encryption and Decryption") {
  try {
    ENIGMA_BEGIN_TIMER(t1);
    // Make IDEA algorithm with intention to Encrypt and Decrypt
    std::unique_ptr<RSA> encrypter(new RSA(RSA::Intent::Encrypt));
    const std::size_t keySize = 2048;
    RSA::RSASettings settings =
      {
        .keySize = keySize};
    encrypter->setSettings(std::move(settings));
    // Buffer to encrypt
    std::size_t maxBufferSize = encrypter->getMaximumBufferSize();
    REQUIRE(maxBufferSize == RSA::getMaximumBufferSizeFromKeySize(keySize));
    std::cout << "maximum buffer length u can encrypt with keysize " << keySize << " is " << maxBufferSize << std::endl;

    std::vector<byte> buffer;
    REQUIRE(FileUtils::Read(fs::path(TEST_DATA_DIR) / "lorem_ipsum.txt", buffer));

    std::cout << "Encrypting... " << std::endl
              << std::endl;
    std::vector<byte> cipher = encrypter->Encrypt("", buffer);
    auto s = ENIGMA_END_TIMER(t1, double, std::milli) / 1000.0;

    std::string private_key = encrypter->getPrivateKey();
    std::string public_key = encrypter->getPublicKey();
    std::cout << "took: " << s << "s to encrypt with rsa key size " << keySize << std::endl
              << std::endl;
    std::cout << "buffer: " << std::string(buffer.begin(), buffer.end()) << std::endl
              << std::endl;
    std::cout << "cipher: " << std::string(cipher.begin(), cipher.end()) << std::endl
              << std::endl;
    std::cout << "private_key: \n"
              << private_key << std::endl
              << std::endl;
    std::cout << "public_key: \n"
              << public_key << std::endl
              << std::endl;


    std::unique_ptr<RSA> decrypter(new RSA(RSA::Intent::Decrypt));
    RSA::RSASettings settings1 =
      {
        .keySize = keySize,
        .privateKey = private_key};
    decrypter->setSettings(std::move(settings1));
    std::cout << "\nDecrypting... " << std::endl
              << std::endl;
    std::cout << "Setting Priv key... " << std::endl;
    //decrypter->setPrivateKey(private_key);
    std::cout << "Setting Pub key... " << std::endl;
    //decrypter->setPublicKey(public_key);
    std::vector<byte> recovered = decrypter->Decrypt("", cipher);
    std::cout << "recovered: " << std::string(recovered.begin(), recovered.end()) << std::endl
              << std::endl;

    REQUIRE(buffer != cipher);
    REQUIRE(buffer == recovered);

  } catch (const CryptoPP::Exception& e) {
    std::cerr << Enigma::CryptoPPUtils::GetFullErrorMessage(e) << '\n';
    REQUIRE(false);
  }
}


TEST_CASE("RSA Encryption and Decryption - various buffer sizes and key sizes") {

    static const std::vector<std::size_t> keySizes = {1024, 1536, 2048, 4096, 5120, 6144, 8192};
    for(const std::size_t keySize : keySizes) {
    const std::size_t bufferSize = Random::Int<std::size_t>(1, ENIGMA_MB_TO_BYTES(10));
    std::cout << "Key size: " << keySize << "\nBuffer size:" << SizeUtils::FriendlySize(bufferSize) << std::endl;

    try {
      ENIGMA_BEGIN_TIMER(t1);
      // Make IDEA algorithm with intention to Encrypt and Decrypt
      std::unique_ptr<RSA> encrypter(new RSA(RSA::Intent::Encrypt));
      RSA::RSASettings settings =
        {
          .keySize = keySize};
      encrypter->setSettings(std::move(settings));
      // Buffer to encrypt
      std::size_t maxBufferSize = encrypter->getMaximumBufferSize();
      REQUIRE(maxBufferSize == RSA::getMaximumBufferSizeFromKeySize(keySize));
      std::vector<byte> buffer = Random::Bytes(bufferSize);

      std::cout << "Encrypting... " << std::endl
                << std::endl;
      std::vector<byte> cipher = encrypter->Encrypt("", buffer);
      auto s = ENIGMA_END_TIMER(t1, double, std::milli) / 1000.0;

      std::string private_key = encrypter->getPrivateKey();
      std::string public_key = encrypter->getPublicKey();
      std::cout <<"Took: " << s << "s to encrypt with rsa key size " << keySize << std::endl
                << std::endl;


      std::unique_ptr<RSA> decrypter(new RSA(RSA::Intent::Decrypt));
      RSA::RSASettings settings1 =
        {
          .keySize = keySize,
          .privateKey = private_key};
      decrypter->setSettings(std::move(settings1));
      std::cout << "\nDecrypting... " << std::endl
                << std::endl;
      std::vector<byte> recovered = decrypter->Decrypt("", cipher);
      REQUIRE(buffer != cipher);
      REQUIRE(buffer == recovered);

    } catch (const CryptoPP::Exception& e) {
      std::cerr << Enigma::CryptoPPUtils::GetFullErrorMessage(e) << '\n';
      REQUIRE(false);
    }
  }
}
