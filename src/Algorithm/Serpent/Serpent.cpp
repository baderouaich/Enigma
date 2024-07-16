#include <pch.hpp>
#include "Serpent.hpp"

NS_ENIGMA_BEGIN

Serpent::Serpent(const Algorithm::Intent intent) noexcept
    : Algorithm(Algorithm::Type::Serpent, intent) {
  if (static_cast<bool>(intent & Intent::Encrypt))
    m_serpent_encryptor = std::make_unique<CryptoPP::GCM<CryptoPP::Serpent>::Encryption>();
  if (static_cast<bool>(intent & Intent::Decrypt))
    m_serpent_decryptor = std::make_unique<CryptoPP::GCM<CryptoPP::Serpent>::Decryption>();
}

Serpent::~Serpent() noexcept {
}

std::vector<byte> Serpent::Encrypt(const std::string& password, const byte *buffer, const std::size_t buffSize) {
  ENIGMA_ASSERT_OR_THROW(m_serpent_encryptor, "Serpent Encryptor is not initialized properly");
  ENIGMA_ASSERT_OR_THROW(password.size() >= Constants::Algorithm::MINIMUM_PASSWORD_LENGTH, "For security reasons, Serpent minimum password length is " + std::to_string(Constants::Algorithm::MINIMUM_PASSWORD_LENGTH));

  Meta::EnigmaFooter footer{};
  footer.magic = Meta::ENIGMA_MAGIC;
  footer.version = ENIGMA_VERSION_MAJOR * 100'000 + ENIGMA_VERSION_MINOR * 1000 + ENIGMA_VERSION_PATCH;
  footer.algo = this->GetType();
  footer.iv = Algorithm::GenerateRandomIV(CryptoPP::Serpent::BLOCKSIZE);
  footer.hash = HashUtils::bytes<CryptoPP::SHA256>(buffer, buffSize);
  footer.extra = {};

  // Encryption key to be generated from user password + IV
  CryptoPP::SecByteBlock key(static_cast<std::size_t>(CryptoPP::Serpent::MAX_KEYLENGTH) + static_cast<std::size_t>(CryptoPP::Serpent::BLOCKSIZE));
  // Convert key to KDF SHA-256, which allows you to use a password smaller or larger than the cipher's key size
  CryptoPP::HKDF<CryptoPP::SHA256> hkdf{};
  hkdf.DeriveKey(
    key, key.size(),
    reinterpret_cast<const byte *>(password.data()), password.size(),
    reinterpret_cast<const byte *>(footer.iv.data()), footer.iv.size(),
    nullptr, 0);
  // Set Key and IV to the encryptor
  m_serpent_encryptor->SetKeyWithIV(key, static_cast<std::size_t>(CryptoPP::Serpent::MAX_KEYLENGTH), key + static_cast<std::size_t>(CryptoPP::Serpent::MAX_KEYLENGTH)); // key, kl, iv, ivl

  // Encrypt
  std::vector<byte> cipher;
  const CryptoPP::ArraySource vs(
    buffer,
    buffSize,
    true,
    new CryptoPP::AuthenticatedEncryptionFilter( // note: for GCM mode, use AuthenticatedEncryptionFilter instead of StreamTransformationFilter
      *m_serpent_encryptor,
      new CryptoPP::VectorSink(cipher))); //NOTE: VectorSource will auto clean the allocated memory

  std::vector<byte> out;
  // Append Cipher
  out.insert(out.end(), cipher.begin(), cipher.end());
  // Append EnigmaFooter
  std::vector<byte> footerBytes = footer.toBytes();
  out.insert(out.end(), footerBytes.begin(), footerBytes.end());
  return out;
}

std::vector<byte> Serpent::Encrypt(const std::string& password, const std::vector<byte>& buffer) {
  return Encrypt(password, buffer.data(), buffer.size());
}

std::vector<byte> Serpent::Decrypt(const std::string& password, const byte *cipher, const std::size_t cipherSize) {
  ENIGMA_ASSERT_OR_THROW(Meta::isEnigmaCipher(cipher, cipherSize), "Given cipher is malformed or was not encrypted with Enigma");
  ENIGMA_ASSERT_OR_THROW(m_serpent_decryptor, "Serpent Decryptor is not initialized properly");

  // Extract footer
  Meta::EnigmaFooter footer = Meta::EnigmaFooter::fromBytes(cipher, cipherSize);
  // Prepare Key
  CryptoPP::SecByteBlock key(static_cast<std::size_t>(CryptoPP::Serpent::MAX_KEYLENGTH) + static_cast<std::size_t>(CryptoPP::Serpent::BLOCKSIZE));
  // Convert key to KDF SHA-256, which allows you to use a password smaller or larger than the cipher's key size
  CryptoPP::HKDF<CryptoPP::SHA256> hkdf;
  hkdf.DeriveKey(
    key, key.size(),
    reinterpret_cast<const byte *>(password.data()), password.size(),
    reinterpret_cast<const byte *>(footer.iv.data()), footer.iv.size(),
    nullptr, 0);

  // Set Key and IV to the decrypter
  m_serpent_decryptor->SetKeyWithIV(key, static_cast<std::size_t>(CryptoPP::Serpent::MAX_KEYLENGTH), key + static_cast<std::size_t>(CryptoPP::Serpent::MAX_KEYLENGTH)); // key, kl, iv, ivl

  // Decrypt
  std::vector<byte> decrypted;
  [[maybe_unused]] const auto ss = CryptoPP::ArraySource(
    cipher,
    cipherSize - footer.sizeInBytes(),
    true,
    new CryptoPP::AuthenticatedDecryptionFilter(
      *m_serpent_decryptor,
      new CryptoPP::VectorSink(decrypted)));

  // Ensure decryption is successful
  ENIGMA_ASSERT_OR_THROW(HashUtils::bytes<CryptoPP::SHA256>(decrypted) == footer.hash, "Decryption failure. Original SHA256 hash of buffer does not match decrypted hash");
  return decrypted;
}

std::vector<byte> Serpent::Decrypt(const std::string& password, const std::vector<byte>& cipher) {
  return Decrypt(password, cipher.data(), cipher.size());
}


void Serpent::Encrypt(const std::string& password, const fs::path& in_filename, const fs::path& out_filename) {
  ENIGMA_ASSERT_OR_THROW(!Meta::isEnigmaFile(in_filename), in_filename.filename().string() + " is already encrypted with Enigma");
  ENIGMA_ASSERT_OR_THROW(password.size() >= Constants::Algorithm::MINIMUM_PASSWORD_LENGTH, "Serpent Minimum Password Length is " + std::to_string(Constants::Algorithm::MINIMUM_PASSWORD_LENGTH));
  ENIGMA_ASSERT_OR_THROW(m_serpent_encryptor, "Serpent Encryptor is not initialized properly");

  Meta::EnigmaFooter footer{};
  footer.magic = Meta::ENIGMA_MAGIC;
  footer.version = ENIGMA_VERSION_MAJOR * 100'000 + ENIGMA_VERSION_MINOR * 1000 + ENIGMA_VERSION_PATCH;
  footer.algo = this->GetType();
  footer.iv = Algorithm::GenerateRandomIV(CryptoPP::Serpent::BLOCKSIZE);
  footer.extra = {};

  // Encryption key to be generated from user password + IV
  CryptoPP::SecByteBlock key(static_cast<std::size_t>(CryptoPP::Serpent::MAX_KEYLENGTH) + static_cast<std::size_t>(CryptoPP::Serpent::BLOCKSIZE));
  // Convert key to KDF SHA-256, which allows you to use a password smaller or larger than the cipher's key size
  CryptoPP::HKDF<CryptoPP::SHA256> hkdf{};
  hkdf.DeriveKey(
    key, key.size(),
    reinterpret_cast<const byte *>(password.data()), password.size(),
    reinterpret_cast<const byte *>(footer.iv.data()), footer.iv.size(),
    nullptr, 0);

  // Now encrypt file chunk by chunk
  std::ofstream ofs{out_filename, std::ios::binary};
  CryptoPP::SHA256 sha256{};
  bool ok = ofs.good();
  FileUtils::ReadChunks(in_filename, Meta::ENIGMA_BUFFER_DEFAULT_SIZE, [this, &ok, &ofs, &key, &sha256](std::vector<byte>&& chunk) -> bool {
    try {
      // Encrypt chunk
      // Set Key and IV to the encryptor
      m_serpent_encryptor->SetKeyWithIV(key, static_cast<std::size_t>(CryptoPP::Serpent::MAX_KEYLENGTH), key + static_cast<std::size_t>(CryptoPP::Serpent::MAX_KEYLENGTH)); // key, kl, iv, ivl

      Meta::EnigmaCipherChunk cipherChunk{};
      cipherChunk.magic = Meta::ENIGMA_CIPHER_CHUNK_MAGIC;
      const CryptoPP::VectorSource vs(
        chunk,
        true,
        new CryptoPP::AuthenticatedEncryptionFilter( // note: for GCM mode, use AuthenticatedEncryptionFilter instead of StreamTransformationFilter
          *m_serpent_encryptor,
          new CryptoPP::VectorSink(cipherChunk.cipher))); //NOTE: VectorSource will auto clean the allocated memory
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

void Serpent::Decrypt(const std::string& password, const fs::path& in_filename, const fs::path& out_filename) {
  ENIGMA_ASSERT_OR_THROW(Meta::isEnigmaFile(in_filename), "Given file " + in_filename.string() + " is malformed or was not encrypted with Enigma");
  ENIGMA_ASSERT_OR_THROW(m_serpent_decryptor, "Serpent Decryptor is not initialized properly");

  // Extract footer from encrypted file
  Meta::EnigmaFooter footer = Meta::EnigmaFooter::fromFile(in_filename);
  // Prepare Key
  CryptoPP::SecByteBlock key(static_cast<std::size_t>(CryptoPP::Serpent::MAX_KEYLENGTH) + static_cast<std::size_t>(CryptoPP::Serpent::BLOCKSIZE));
  // Convert key to KDF SHA-256, which allows you to use a password smaller or larger than the cipher's key size
  CryptoPP::HKDF<CryptoPP::SHA256> hkdf;
  hkdf.DeriveKey(
    key, key.size(),
    reinterpret_cast<const byte *>(password.data()), password.size(),
    reinterpret_cast<const byte *>(footer.iv.data()), footer.iv.size(),
    nullptr, 0);

  // Decrypt file chunk by chunk
  std::ofstream ofs{out_filename, std::ios::binary};
  CryptoPP::SHA256 sha256;
  bool ok = ofs.good();
  Meta::readCipherChunks(in_filename, [this, &ok, &ofs, &key, &sha256](Meta::EnigmaCipherChunk&& cipherChunk) -> bool {
    try {
      // Decrypt chunk
      // Set Key and IV to the decrypter
      m_serpent_decryptor->SetKeyWithIV(key, static_cast<std::size_t>(CryptoPP::Serpent::MAX_KEYLENGTH), key + static_cast<std::size_t>(CryptoPP::Serpent::MAX_KEYLENGTH)); // key, kl, iv, ivl

      std::vector<byte> decrypted{};
      [[maybe_unused]] const auto ss = CryptoPP::VectorSource(
        cipherChunk.cipher,
        true,
        new CryptoPP::AuthenticatedDecryptionFilter(
          *m_serpent_decryptor,
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
