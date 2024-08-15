#pragma once
#include "TestsData.hpp"
#include "Utility/FinalAction.hpp"
#include "Utility/HashUtils.hpp"
#include <Algorithm/Blowfish/Blowfish.hpp>
#include <System/Dialogs/OpenFileDialog.hpp>
#include <Utility/FileUtils.hpp>
#include <Utility/SizeUtils.hpp>
#include <catch2/catch_all.hpp>
#include <iostream>

using namespace Enigma;
using namespace Catch::Matchers;

TEST_CASE("Blowfish-GCM Encryption and Decryption - Text") {
  std::cout << "\n======[ " << Catch::getResultCapture().getCurrentTestName() << " ]======\n";

  // Make Blowfish algorithm with intention to Encrypt and Decrypt
  std::unique_ptr<Blowfish> blowfish(new Blowfish(Blowfish::Intent::Encrypt | Blowfish::Intent::Decrypt));

  // Buffer to encrypt
  std::string randomStr = Random::Str(ENIGMA_MB_TO_BYTES(Random::Int<std::size_t>(1, 50)));
  std::vector<byte> buffer(randomStr.begin(), randomStr.end());
  // Encryption password
  const std::string password = Random::Str(ENIGMA_MB_TO_BYTES(Random::Int<std::size_t>(1, 5)));

  // Encrypted buffer (aka cipher)
  std::vector<byte> encrypted = blowfish->Encrypt(password, buffer);
  // Decrypted cipher (aka recovered)
  std::vector<byte> decrypted = blowfish->Decrypt(password, encrypted);

  SECTION("Comparing buffers") {
    // Buffer must not match cipher
    REQUIRE_THAT(buffer, !Equals(encrypted));
    // Buffer must match decrypted cipher
    REQUIRE_THAT(buffer, Equals(decrypted));
  }
}

TEST_CASE("Blowfish-GCM Encryption and Decryption - File") {
  std::cout << "\n======[ " << Catch::getResultCapture().getCurrentTestName() << " ]======\n";

  // Make Blowfish algorithm with intention to Encrypt and Decrypt
  std::unique_ptr<Blowfish> blowfish(new Blowfish(Blowfish::Intent::Encrypt | Blowfish::Intent::Decrypt));

  const std::string password = Random::Str(ENIGMA_MB_TO_BYTES(Random::Int<std::size_t>(1, 5)));

  // File to encrypt
  fs::path filenameToEncrypt = fs::path(TEST_DATA_DIR) / "file.bin";
  fs::path encryptedFilename = fs::temp_directory_path() / ("Enigma_tmp_" + Random::Str(16) + "_file.bin.enigma");
  FinalAction encryptedFilenameDeleter{[encryptedFilename] { fs::remove(encryptedFilename); }};
  fs::path recoveredFilename = fs::temp_directory_path() / ("Enigma_tmp_" + Random::Str(16) + "_file.bin.recovered");
  FinalAction recoveredFilenameDeleter{[recoveredFilename] { fs::remove(recoveredFilename); }};

  blowfish->Encrypt(password, filenameToEncrypt, encryptedFilename);
  blowfish->Decrypt(password, encryptedFilename, recoveredFilename);

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

TEST_CASE("Decrypt file_Blowfish.bin.enigma") {
  const fs::path originalFilename = fs::path(TEST_DATA_DIR) / "file.bin";
  const fs::path encryptedFilename = fs::path(TEST_DATA_DIR) / "file_Blowfish.bin.enigma";
  const fs::path decryptedFilename = fs::temp_directory_path() / "file_Blowfish.bin";
  FinalAction decryptedFileDeleter([decryptedFilename] {
    fs::remove(decryptedFilename);
  });

  try {
    Blowfish blowfish{Blowfish::Intent::Decrypt};
    blowfish.Decrypt("enigma@123", encryptedFilename, decryptedFilename);

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