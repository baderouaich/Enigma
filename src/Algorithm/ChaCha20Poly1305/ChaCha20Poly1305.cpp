#include <pch.hpp>
#include "ChaCha20Poly1305.hpp"

NS_ENIGMA_BEGIN

ChaCha20Poly1305::ChaCha20Poly1305(const Algorithm::Intent intent) noexcept
    : Algorithm(Algorithm::Type::ChaCha20Poly1305, intent) {

  if (static_cast<bool>(intent & Intent::Encrypt))
    m_chacha_encryptor = std::make_unique<CryptoPP::ChaCha20Poly1305::Encryption>();
  if (static_cast<bool>(intent & Intent::Decrypt))
    m_chacha_decryptor = std::make_unique<CryptoPP::ChaCha20Poly1305::Decryption>();
}

ChaCha20Poly1305::~ChaCha20Poly1305() noexcept {
}

std::vector<byte> ChaCha20Poly1305::Encrypt(const std::string& password, const byte *buffer, const std::size_t buffSize) {
  ENIGMA_ASSERT_OR_THROW(m_chacha_encryptor, "ChaCha20Poly1305 Encryptor is not initialized properly");
  ENIGMA_ASSERT_OR_THROW(password.size() >= Constants::Algorithm::MINIMUM_PASSWORD_LENGTH, "For security reasons, ChaCha20Poly1305 minimum password length is " + std::to_string(Constants::Algorithm::MINIMUM_PASSWORD_LENGTH));

  Meta::EnigmaFooter footer{};
  footer.magic = Meta::ENIGMA_MAGIC;
  footer.version = ENIGMA_VERSION_MAJOR * 100'000 + ENIGMA_VERSION_MINOR * 1000 + ENIGMA_VERSION_PATCH;
  footer.algo = this->GetType();
  footer.iv = Algorithm::GenerateRandomIV(m_chacha_encryptor->MaxIVLength());
  footer.hash = HashUtils::bytes<CryptoPP::SHA256>(buffer, buffSize);

  // Prepare key
  CryptoPP::SecByteBlock key(m_chacha_encryptor->MaxKeyLength() + m_chacha_encryptor->MaxIVLength());

  // Convert key to KDF SHA-256, which allows you to use a password smaller or larger than the cipher's key size
  CryptoPP::HKDF<CryptoPP::SHA256> hkdf{};
  hkdf.DeriveKey(
    key,
    key.size(),
    reinterpret_cast<const byte *>(password.data()),
    password.size(),
    footer.iv.data(),
    footer.iv.size(),
    nullptr,
    0);

  // Set Key and IV to the encryptor
  m_chacha_encryptor->SetKeyWithIV(key, m_chacha_encryptor->MaxKeyLength(), key + m_chacha_encryptor->MaxKeyLength()); // key, kl, iv, ivl

  // Output encrypted buffer
  std::vector<byte> cipher(buffSize, '\000');
  // Output calculated MAC
  std::vector<byte> mac(16, '\000');

  // Encrypt & Authenticate (Thanks to @mcoret who mentioned this in https://github.com/baderouaich/Enigma/issues/4)
  m_chacha_encryptor->EncryptAndAuthenticate(
    cipher.data(),                      // output cipher (encrypted buffer)
    mac.data(), mac.size(),             // output calculated MAC
    footer.iv.data(), footer.iv.size(), // iv
    nullptr, 0,                         // aad buffer (additional authenticated data)
    buffer, buffSize                    // buffer to encrypt
  );
  // Make sure you store mac as an extra
  footer.extra = std::move(mac);

  std::vector<byte> out;
  // Append Cipher
  out.insert(out.end(), cipher.begin(), cipher.end());
  // Append EnigmaFooter
  std::vector<byte> footerBytes = footer.toBytes();
  out.insert(out.end(), footerBytes.begin(), footerBytes.end());

  return out;
}

std::vector<byte> ChaCha20Poly1305::Encrypt(const std::string& password, const std::vector<byte>& buffer) {
  return Encrypt(password, buffer.data(), buffer.size());
}

std::vector<byte> ChaCha20Poly1305::Decrypt(const std::string& password, const byte *cipher, const std::size_t cipherSize) {
  ENIGMA_ASSERT_OR_THROW(Meta::isEnigmaCipher(cipher, cipherSize), "Given cipher is malformed or was not encrypted with Enigma");
  ENIGMA_ASSERT_OR_THROW(m_chacha_decryptor, "ChaCha20Poly1305 Decryptor is not initialized properly");

  // Extract footer
  Meta::EnigmaFooter footer = Meta::EnigmaFooter::fromBytes(cipher, cipherSize);
  const std::vector<byte>& mac = footer.extra;
  // Prepare Key
  CryptoPP::SecByteBlock key(m_chacha_decryptor->MaxKeyLength() + m_chacha_decryptor->MaxIVLength());
  // Convert key to KDF SHA-256, which allows you to use a password smaller or larger than the cipher's key size
  CryptoPP::HKDF<CryptoPP::SHA256> hkdf;
  hkdf.DeriveKey(
    key,
    key.size(),
    reinterpret_cast<const byte *>(password.data()),
    password.size(),
    footer.iv.data(),
    footer.iv.size(),
    nullptr,
    0);

  // Set Key and IV to the decryptor
  m_chacha_decryptor->SetKeyWithIV(key, m_chacha_decryptor->MaxKeyLength(), key + m_chacha_decryptor->MaxKeyLength()); // key, kl, iv, ivl

  // Recovered cipher
  std::vector<byte> decrypted(cipherSize - footer.sizeInBytes(), '\000'); // make sure your exclude footer

  // Decrypt and verify MAC
  const bool macVerified = m_chacha_decryptor->DecryptAndVerify(
    decrypted.data(),                         // output buffer (decrypted cipher)
    mac.data(), mac.size(),                   // input MAC (calculated in encryption)
    footer.iv.data(), footer.iv.size(),       // input IV (generated in encryption)
    nullptr, 0,                               // aad buffer (additional authenticated data)
    cipher, cipherSize - footer.sizeInBytes() // cipher to decrypt (make sure your exclude footer)
  );

  // Throw an exception if mac was not verified (mac is auto calculated at encryption by encryptor->EncryptAndAuthenticate)
  ENIGMA_ASSERT_OR_THROW(macVerified, ("Failed to verify ChaCha20Poly1305 MAC"));

  // Ensure decryption is successful
  ENIGMA_ASSERT_OR_THROW(HashUtils::bytes<CryptoPP::SHA256>(decrypted) == footer.hash, "Decryption failure. Original SHA256 hash of buffer does not match decrypted hash");

  return decrypted;
}
std::vector<byte> ChaCha20Poly1305::Decrypt(const std::string& password, const std::vector<byte>& cipher) {
  return Decrypt(password, cipher.data(), cipher.size());
}

void ChaCha20Poly1305::Encrypt(const std::string& password, const fs::path& in_filename, const fs::path& out_filename) {
  ENIGMA_ASSERT_OR_THROW(!Meta::isEnigmaFile(in_filename), in_filename.filename().string() + " is already encrypted with Enigma");
  ENIGMA_ASSERT_OR_THROW(password.size() >= Constants::Algorithm::MINIMUM_PASSWORD_LENGTH, "For security reasons, ChaCha20Poly1305 Minimum Password Length is " + std::to_string(Constants::Algorithm::MINIMUM_PASSWORD_LENGTH));
  ENIGMA_ASSERT_OR_THROW(m_chacha_encryptor, "ChaCha20Poly1305 Encryptor is not initialized properly");

  Meta::EnigmaFooter footer{};
  footer.magic = Meta::ENIGMA_MAGIC;
  footer.version = ENIGMA_VERSION_MAJOR * 100'000 + ENIGMA_VERSION_MINOR * 1000 + ENIGMA_VERSION_PATCH;
  footer.algo = this->GetType();
  footer.iv = Algorithm::GenerateRandomIV(m_chacha_encryptor->MaxIVLength());

  // Prepare key
  CryptoPP::SecByteBlock key(m_chacha_encryptor->MaxKeyLength() + m_chacha_encryptor->MaxIVLength());

  // Convert key to KDF SHA-256, which allows you to use a password smaller or larger than the cipher's key size
  CryptoPP::HKDF<CryptoPP::SHA256> hkdf{};
  hkdf.DeriveKey(
    key,
    key.size(),
    reinterpret_cast<const byte *>(password.data()),
    password.size(),
    footer.iv.data(),
    footer.iv.size(),
    nullptr,
    0);

  // Now encrypt file chunk by chunk
  std::ofstream ofs{out_filename, std::ios::binary};
  CryptoPP::SHA256 sha256{};
  bool ok = ofs.good();
  FileUtils::ReadChunks(in_filename, Meta::ENIGMA_BUFFER_DEFAULT_SIZE, [this, &ok, &ofs, &key, &sha256, &footer](std::vector<byte>&& chunk) -> bool {
    try {
      // Encrypt chunk
      // Set Key and IV to the encryptor
      m_chacha_encryptor->SetKeyWithIV(key, m_chacha_encryptor->MaxKeyLength(), key + m_chacha_encryptor->MaxKeyLength()); // key, kl, iv, ivl

      Meta::EnigmaCipherChunk cipherChunk{};
      cipherChunk.magic = Meta::ENIGMA_CIPHER_CHUNK_MAGIC;
      cipherChunk.cipher.resize(chunk.size(), '\000');
      // Output calculated MAC
      std::vector<byte> mac(16, '\000');
      // Encrypt & Authenticate (Thanks to @mcoret who mentioned this in https://github.com/baderouaich/Enigma/issues/4)
      m_chacha_encryptor->EncryptAndAuthenticate(
        cipherChunk.cipher.data(),          // output cipher (encrypted buffer)
        mac.data(), mac.size(),             // output calculated MAC
        footer.iv.data(), footer.iv.size(), // iv
        nullptr, 0,                         // aad buffer (additional authenticated data)
        chunk.data(), chunk.size()          // buffer to encrypt
      );
      // Make sure you export mac too since we need it in decryption
      cipherChunk.extra = std::move(mac);

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

void ChaCha20Poly1305::Decrypt(const std::string& password, const fs::path& in_filename, const fs::path& out_filename) {
  ENIGMA_ASSERT_OR_THROW(Meta::isEnigmaFile(in_filename), "Given file " + in_filename.string() + " is malformed or was not encrypted with Enigma");
  ENIGMA_ASSERT_OR_THROW(m_chacha_decryptor, "ChaCha20Poly1305 Decryptor is not initialized properly");

  // Extract footer from encrypted file
  Meta::EnigmaFooter footer = Meta::EnigmaFooter::fromFile(in_filename);

  // Prepare key
  CryptoPP::SecByteBlock key(m_chacha_decryptor->MaxKeyLength() + m_chacha_decryptor->MaxIVLength());

  // Convert key to KDF SHA-256, which allows you to use a password smaller or larger than the cipher's key size
  CryptoPP::HKDF<CryptoPP::SHA256> hkdf{};
  hkdf.DeriveKey(
    key,
    key.size(),
    reinterpret_cast<const byte *>(password.data()),
    password.size(),
    footer.iv.data(),
    footer.iv.size(),
    nullptr,
    0);

  // Decrypt file chunk by chunk
  std::ofstream ofs{out_filename, std::ios::binary};
  CryptoPP::SHA256 sha256;
  bool ok = ofs.good();
  Meta::readCipherChunks(in_filename, [this, &ok, &ofs, &key, &sha256, &footer](Meta::EnigmaCipherChunk&& cipherChunk) -> bool {
    try {
      // Decrypt chunk
      // Set Key and IV to the decrypter
      m_chacha_decryptor->SetKeyWithIV(key, m_chacha_decryptor->MaxKeyLength(), key + m_chacha_decryptor->MaxKeyLength()); // key, kl, iv, ivl

      std::vector<byte> decrypted(cipherChunk.cipher.size() /*- footer.sizeInBytes()*/, '\000'); // (make sure your exclude footer)
      const std::vector<byte>& mac = cipherChunk.extra;
      ENIGMA_ASSERT_OR_THROW(!mac.empty(), "Could not read ChaCha20Poly1305 MAC from EnigmaCipherChunk");

      // Decrypt and verify MAC
      const bool macVerified = m_chacha_decryptor->DecryptAndVerify(
        decrypted.data(),                                                                // output buffer (decrypted cipher)
        mac.data(), mac.size(),                                                          // input MAC (calculated in encryption)
        footer.iv.data(), footer.iv.size(),                                              // input IV (generated in encryption)
        nullptr, 0,                                                                      // aad buffer (additional authenticated data)
        cipherChunk.cipher.data(), cipherChunk.cipher.size() /* - footer.sizeInBytes()*/ // cipher to decrypt (make sure your exclude footer)
      );
      // Throw an exception if mac was not verified (mac is auto calculated at encryption by encryptor->EncryptAndAuthenticate)
      ENIGMA_ASSERT_OR_THROW(macVerified, "Failed to verify ChaCha20Poly1305 MAC");

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
