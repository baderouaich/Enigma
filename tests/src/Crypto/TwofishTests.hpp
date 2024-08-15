#pragma once
#include "TestsData.hpp"
#include "Utility/FinalAction.hpp"
#include "Utility/HashUtils.hpp"
#include <Algorithm/Twofish/Twofish.hpp>
#include <Utility/SizeUtils.hpp>
#include <catch2/catch_all.hpp>
#include <iostream>
using namespace Enigma;
using namespace Catch::Matchers;


TEST_CASE("Twofish Encryption and Decryption - Text") {
  std::cout << "\n======[ " << Catch::getResultCapture().getCurrentTestName() << " ]======\n";

  // Make TripleDES algorithm with intention to Encrypt and Decrypt
  std::unique_ptr<Twofish> twofish(new Twofish(Twofish::Intent::Encrypt | Twofish::Intent::Decrypt));

  // Buffer to encrypt
  std::string randomStr = Random::Str(ENIGMA_MB_TO_BYTES(Random::Int<std::size_t>(1, 50)));
  std::vector<byte> buffer(randomStr.begin(), randomStr.end());
  // Encryption password
  const std::string password = Random::Str(ENIGMA_MB_TO_BYTES(Random::Int<std::size_t>(1, 5)));

  // Encrypted buffer (aka cipher)
  std::vector<byte> encrypted = twofish->Encrypt(password, buffer);
  // Decrypted cipher (aka recovered)
  std::vector<byte> decrypted = twofish->Decrypt(password, encrypted);

  SECTION("Comparing buffers") {
    // Buffer must not match cipher
    REQUIRE_THAT(buffer, !Equals(encrypted));
    // Buffer must match decrypted cipher
    REQUIRE_THAT(buffer, Equals(decrypted));
  }
}

TEST_CASE("Twofish Encryption and Decryption - File") {
  std::cout << "\n======[ " << Catch::getResultCapture().getCurrentTestName() << " ]======\n";


  // Make TripleDES algorithm with intention to Encrypt and Decrypt
  std::unique_ptr<Twofish> twofish(new Twofish(Twofish::Intent::Encrypt | Twofish::Intent::Decrypt));
  const std::string password = Random::Str(ENIGMA_MB_TO_BYTES(Random::Int<std::size_t>(1, 5)));

  // File to encrypt
  fs::path filenameToEncrypt = fs::path(TEST_DATA_DIR) / "file.bin";
  fs::path encryptedFilename = fs::temp_directory_path() / ("Enigma_tmp_" + Random::Str(16) + "_file.bin.enigma");
  FinalAction encryptedFilenameDeleter{[encryptedFilename] { fs::remove(encryptedFilename); }};
  fs::path recoveredFilename = fs::temp_directory_path() / ("Enigma_tmp_" + Random::Str(16) + "_file.bin.recovered");
  FinalAction recoveredFilenameDeleter{[recoveredFilename] { fs::remove(recoveredFilename); }};

  twofish->Encrypt(password, filenameToEncrypt, encryptedFilename);
  twofish->Decrypt(password, encryptedFilename, recoveredFilename);

  SECTION("Comparing files") {
    std::string originalFileHash = HashUtils::fileStr<CryptoPP::SHA512>(filenameToEncrypt);
    std::string encryptedFileHash = HashUtils::fileStr<CryptoPP::SHA512>(encryptedFilename);
    std::string recoveredFileHash = HashUtils::fileStr<CryptoPP::SHA512>(recoveredFilename);
    // Original file must not match cipher file
    REQUIRE_THAT(originalFileHash, !Equals(encryptedFileHash));
    // Original must match decrypted file
    REQUIRE_THAT(originalFileHash, Equals(recoveredFileHash));
  }
}

TEST_CASE("Decrypt file_Twofish.bin.enigma") {
  const fs::path originalFilename = fs::path(TEST_DATA_DIR) / "file.bin";
  const fs::path encryptedFilename = fs::path(TEST_DATA_DIR) / "file_Twofish.bin.enigma";
  const fs::path decryptedFilename = fs::temp_directory_path() / "file_Twofish.bin";
  FinalAction decryptedFileDeleter([decryptedFilename] {
    fs::remove(decryptedFilename);
  });

  try {
    Twofish twofish{Twofish::Intent::Decrypt};
    twofish.Decrypt("enigma@123", encryptedFilename, decryptedFilename);

    // Ensure recovered file and original file match
    std::array<byte, CryptoPP::SHA512::DIGESTSIZE> originalFileHash = HashUtils::fileBytes<CryptoPP::SHA512>(originalFilename);
    std::array<byte, CryptoPP::SHA512::DIGESTSIZE> encryptedFileHash = HashUtils::fileBytes<CryptoPP::SHA512>(encryptedFilename);
    std::array<byte, CryptoPP::SHA512::DIGESTSIZE> decryptedFileHash = HashUtils::fileBytes<CryptoPP::SHA512>(decryptedFilename);

    REQUIRE(originalFileHash == decryptedFileHash);
    REQUIRE(originalFileHash != encryptedFileHash);

  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    REQUIRE(false);
  } catch (...) {
    REQUIRE(false);
  }
}