#include "RSA.hpp"
#include <pch.hpp>

NS_ENIGMA_BEGIN

RSA::RSA(const Algorithm::Intent intent) noexcept
    : Algorithm(Algorithm::Type::RSA, intent) {
}

RSA::~RSA() noexcept {
}

std::vector<byte> RSA::Encrypt(const std::string& password, const byte *buffer, const std::size_t buffSize) {
  ENIGMA_ASSERT_OR_THROW(m_settings, "RSA settings was not set");
  ENIGMA_ASSERT_OR_THROW(m_public_key, "RSA Public key is not initialized properly");
  ENIGMA_ASSERT_OR_THROW(m_private_key, "RSA Private key is not initialized properly");
  ENIGMA_ASSERT_OR_THROW(password.empty(), "RSA Algorithm does not need a password");
  ENIGMA_ASSERT_OR_THROW(m_auto_seeded_random_pool, "AutoSeededRandomPool is not initialized properly");

  Meta::EnigmaFooter footer{};
  footer.magic = Meta::ENIGMA_MAGIC;
  footer.version = ENIGMA_VERSION_MAJOR * 100'000 + ENIGMA_VERSION_MINOR * 1000 + ENIGMA_VERSION_PATCH;
  footer.algo = this->GetType();
  footer.iv = {}; // no iv
  footer.hash = HashUtils::bytes<CryptoPP::SHA256>(buffer, buffSize);
  std::vector<byte> extra(sizeof(m_settings->keySize), '\000');
  std::memcpy(extra.data(), &m_settings->keySize, sizeof(m_settings->keySize));
  footer.extra = std::move(extra); // remember keysize


  const std::size_t maxBufferSize = m_rsa_encryptor->FixedMaxPlaintextLength();
  std::size_t offset = 0;

  std::vector<byte> out;
  while (offset < buffSize) {
    std::size_t bufferChunkSize = std::min(maxBufferSize, buffSize - offset);
    const byte *bufferChunk = buffer + offset;

    // Encrypt chunk
    std::vector<byte> cipherChunk;
    const CryptoPP::ArraySource vs(
      bufferChunk,
      bufferChunkSize,
      true,
      new CryptoPP::PK_EncryptorFilter(
        *m_auto_seeded_random_pool,
        *m_rsa_encryptor,
        new CryptoPP::VectorSink(cipherChunk)));

    // Append cipher chunk size
    const std::size_t cipherChunkSize = cipherChunk.size();
    std::vector<byte> cipherChunkSizeBytes(sizeof(cipherChunkSize), '\000');
    std::memcpy(cipherChunkSizeBytes.data(), &cipherChunkSize, sizeof(cipherChunkSize));
    out.insert(out.end(), cipherChunkSizeBytes.begin(), cipherChunkSizeBytes.end());
    // Append cipher chunk
    out.insert(out.end(), cipherChunk.begin(), cipherChunk.end());

    // Advance to next chunk
    offset += bufferChunkSize;
  }

  // Append EnigmaFooter
  std::vector<byte> footerBytes = footer.toBytes();
  out.insert(out.end(), footerBytes.begin(), footerBytes.end());
  return out;

#if SINGLE_CHUNK // This old implementation requires the input buffer to be less or equal to the FixedMaxPlaintextLength(), \
                 // Which means we'l have to limit user input.
  Meta::EnigmaFooter footer{};
  footer.magic = Meta::ENIGMA_MAGIC;
  footer.version = ENIGMA_VERSION_MAJOR * 100'000 + ENIGMA_VERSION_MINOR * 1000 + ENIGMA_VERSION_PATCH;
  footer.algo = this->GetType();
  footer.iv = {}; // no iv
  footer.hash = HashUtils::bytes<CryptoPP::SHA256>(buffer, buffSize);
  footer.extra = {};

  // Encrypt
  std::vector<byte> cipher;
  const CryptoPP::ArraySource vs(
    buffer,
    buffSize,
    true,
    new CryptoPP::PK_EncryptorFilter(
      *m_auto_seeded_random_pool,
      *m_rsa_encryptor,
      new CryptoPP::VectorSink(cipher))); //NOTE: VectorSource will auto clean the allocated memory


  std::vector<byte> out;
  // Append Cipher
  out.insert(out.end(), cipher.begin(), cipher.end());
  // Append EnigmaFooter
  std::vector<byte> footerBytes = footer.toBytes();
  out.insert(out.end(), footerBytes.begin(), footerBytes.end());
  return out;
#endif
}

std::vector<byte> RSA::Encrypt(const std::string& password, const std::vector<byte>& buffer) {
  return Encrypt(password, buffer.data(), buffer.size());
}

std::vector<byte> RSA::Decrypt(const std::string& password, const byte *cipher, const std::size_t cipherSize) {
  ENIGMA_ASSERT_OR_THROW(m_settings, "RSA settings was not set");
  ENIGMA_ASSERT_OR_THROW(Meta::isEnigmaCipher(cipher, cipherSize), "Given cipher is malformed or was not encrypted with Enigma");
  ENIGMA_ASSERT_OR_THROW(m_private_key, "RSA Private key is not initialized properly"); // We only need priv key to decrypt
  ENIGMA_ASSERT_OR_THROW(password.empty(), "RSA Algorithm does not need a password");
  ENIGMA_ASSERT_OR_THROW(m_rsa_decryptor, "RSA Decrypter is not initialized properly");
  ENIGMA_ASSERT_OR_THROW(m_auto_seeded_random_pool, "AutoSeededRandomPool is not initialized properly");

  // Extract footer
  Meta::EnigmaFooter footer = Meta::EnigmaFooter::fromBytes(cipher, cipherSize);

  // Process chunks...
  // Note, each chunk starts with its size of bytes
  std::vector<byte> out;
  std::size_t offset = 0;
  while (offset < cipherSize - footer.sizeInBytes()) {
    std::size_t cipherChunkSize{};
    std::memcpy(&cipherChunkSize, cipher + offset, sizeof(cipherChunkSize));
    const byte *cipherChunk = cipher + sizeof(cipherChunkSize) + offset;

    // Decrypt cipher chunk
    std::vector<byte> decrypted;
    [[maybe_unused]] const auto ss = CryptoPP::ArraySource(
      cipherChunk,
      cipherChunkSize,
      true,
      new CryptoPP::PK_DecryptorFilter(
        *m_auto_seeded_random_pool,
        *m_rsa_decryptor,
        new CryptoPP::VectorSink(decrypted)));

    // Append buffer
    out.insert(out.end(), decrypted.begin(), decrypted.end());

    // Advance to next chunk
    offset += sizeof(cipherChunkSize) + cipherChunkSize;
  }

  return out;
}

std::vector<byte> RSA::Decrypt(const std::string& password, const std::vector<byte>& cipher) {
  return Decrypt(password, cipher.data(), cipher.size());
}


void RSA::Encrypt(const std::string& password, const fs::path& in_filename, const fs::path& out_filename) {
  ENIGMA_ASSERT_OR_THROW(!Meta::isEnigmaFile(in_filename), in_filename.filename().string() + " is already encrypted with Enigma");
  ENIGMA_ASSERT_OR_THROW(m_settings, "RSA settings was not set");
  ENIGMA_ASSERT_OR_THROW(m_public_key, "RSA Public key is not initialized properly");
  ENIGMA_ASSERT_OR_THROW(m_private_key, "RSA Private key is not initialized properly");
  ENIGMA_ASSERT_OR_THROW(password.empty(), "RSA Algorithm does not need a password");
  ENIGMA_ASSERT_OR_THROW(m_rsa_encryptor, "RSA Encryptor is not initialized properly");
  ENIGMA_ASSERT_OR_THROW(m_auto_seeded_random_pool, "AutoSeededRandomPool is not initialized properly");

  Meta::EnigmaFooter footer{};
  footer.magic = Meta::ENIGMA_MAGIC;
  footer.version = ENIGMA_VERSION_MAJOR * 100'000 + ENIGMA_VERSION_MINOR * 1000 + ENIGMA_VERSION_PATCH;
  footer.algo = this->GetType();
  footer.iv = {}; // no iv
  std::vector<byte> extra(sizeof(m_settings->keySize), '\000');
  std::memcpy(extra.data(), &m_settings->keySize, sizeof(m_settings->keySize));
  footer.extra = std::move(extra); // remember keysize

  const std::size_t maxBufferSize = m_rsa_encryptor->FixedMaxPlaintextLength();

  // Now encrypt file chunk by chunk
  std::ofstream ofs{out_filename, std::ios::binary};
  CryptoPP::SHA256 sha256{};
  bool ok = ofs.good();
  FileUtils::ReadChunks(in_filename, maxBufferSize, [this, &ok, &ofs, &sha256](std::vector<byte>&& chunk) -> bool {
    try {
      // Encrypt chunk
      Meta::EnigmaCipherChunk cipherChunk{};
      cipherChunk.magic = Meta::ENIGMA_CIPHER_CHUNK_MAGIC;
      const CryptoPP::ArraySource vs(
        chunk.data(),
        chunk.size(),
        true,
        new CryptoPP::PK_EncryptorFilter(
          *m_auto_seeded_random_pool,
          *m_rsa_encryptor,
          new CryptoPP::VectorSink(cipherChunk.cipher)));
      // Save chunk bytes to ofs
      const std::vector<byte> cipherChunkBytes = cipherChunk.toBytes();
      ok &= ofs.write(reinterpret_cast<const char *>(cipherChunkBytes.data()), cipherChunkBytes.size()).good();
      // Also profit from the occasion to calculate sha256 hash
      sha256.Update(chunk.data(), chunk.size());
      return ok;
    } catch (const CryptoPP::Exception& e) {
      ok &= false;
      ENIGMA_ERROR("Could not encrypt chunk: {}", e.what());
    } catch (const std::exception& e) {
      ok &= false;
      ENIGMA_ERROR("Could not encrypt chunk: {}", e.what());
    } catch (...) {
      ok &= false;
      ENIGMA_ERROR("Could not encrypt chunk: UNKNOWN ERROR");
    }
    return ok;
  });

  // No need to keep out_filename on failed encryption
  if (!ok) {
    ofs.close();
    fs::remove(out_filename);
    throw std::runtime_error("Could not encrypt file " + in_filename.string());
  }

  // Alles gut!
  // Append enigma footer info
  ofs.seekp(0, std::ios::end);
  sha256.Final(footer.hash.data());
  std::vector<byte> footerBytes = footer.toBytes();
  ofs.write(reinterpret_cast<const char *>(footerBytes.data()), footerBytes.size());
  ENIGMA_ASSERT_OR_THROW(ofs.good(), "Failed to write footer bytes");
  ofs.close();
}

void RSA::Decrypt(const std::string& password, const fs::path& in_filename, const fs::path& out_filename) {
  ENIGMA_ASSERT_OR_THROW(Meta::isEnigmaFile(in_filename), "Given file " + in_filename.string() + " is malformed or was not encrypted with Enigma");
  ENIGMA_ASSERT_OR_THROW(m_settings, "RSA settings was not set");
  ENIGMA_ASSERT_OR_THROW(m_private_key, "RSA Private key is not initialized properly"); // We only need private key to decrypt
  ENIGMA_ASSERT_OR_THROW(password.empty(), "RSA Algorithm does not need a password");
  ENIGMA_ASSERT_OR_THROW(m_rsa_decryptor, "RSA Decrypter is not initialized properly");
  ENIGMA_ASSERT_OR_THROW(m_auto_seeded_random_pool, "AutoSeededRandomPool is not initialized properly");

  // Extract footer from encrypted file
  Meta::EnigmaFooter footer = Meta::EnigmaFooter::fromFile(in_filename);

  // Decrypt file chunk by chunk
  std::ofstream ofs{out_filename, std::ios::binary};
  CryptoPP::SHA256 sha256;
  bool ok = ofs.good();
  Meta::readCipherChunks(in_filename, [this, &ok, &ofs, &sha256](Meta::EnigmaCipherChunk&& cipherChunk) -> bool {
    try {
      // Decrypt cipher chunk
      std::vector<byte> decrypted;
      [[maybe_unused]] const auto ss = CryptoPP::ArraySource(
        cipherChunk.cipher.data(),
        cipherChunk.cipher.size(),
        true,
        new CryptoPP::PK_DecryptorFilter(
          *m_auto_seeded_random_pool,
          *m_rsa_decryptor,
          new CryptoPP::VectorSink(decrypted)));

      // Write decrypted chunk to ofs
      ok &= ofs.write(reinterpret_cast<const char *>(decrypted.data()), decrypted.size()).good();
      // Also profit from the occasion to calculate sha256 hash
      sha256.Update(decrypted.data(), decrypted.size());
      return ok;
    } catch (const CryptoPP::Exception& e) {
      ok &= false;
      ENIGMA_ERROR("Could not decrypt chunk: {}", e.what());
    } catch (const std::exception& e) {
      ok &= false;
      ENIGMA_ERROR("Could not decrypt chunk: {}", e.what());
    } catch (...) {
      ok &= false;
      ENIGMA_ERROR("Could not decrypt chunk: UNKNOWN ERROR");
    }
    return ok;
  });
  // No need to keep out_filename on failed decryption
  if (!ok) {
    ofs.close();
    fs::remove(out_filename);
    throw std::runtime_error("Could not decrypt file " + in_filename.string());
  }
  ofs.close();

  // Ensure decryption is successful
  std::array<byte, CryptoPP::SHA256::DIGESTSIZE> digest{};
  sha256.Final(digest.data());
  ENIGMA_INFO("Verifying SHA256 hash of {} ...", out_filename.filename().string());
  ENIGMA_ASSERT_OR_THROW(digest == footer.hash, "Decryption failure. Original SHA256 hash of file does not match decrypted hash");
}

NS_ENIGMA_END
