#include <pch.hpp>
#include "Meta.hpp"

NS_ENIGMA_BEGIN

Meta::size_type Meta::EnigmaFooter::sizeInBytes() const noexcept {
  // NOTE: make sure you include any new member size here
  return sizeof(magic) +
         sizeof(version) +
         sizeof(algo) +
         hash.size() +
         sizeof(size_type) + iv.size() +
         sizeof(size_type) + extra.size();
}

std::vector<byte> Meta::EnigmaFooter::toBytes() const {
  std::vector<byte> out(this->sizeInBytes(), '\000');
  // Keep track of the current position in the output vector
  byte *pos = out.data();

  // Copy extra
  std::memcpy(pos, extra.data(), LittleEndian::fromHost(extra.size()));
  pos += extra.size();

  // Copy extra size
  size_type extraSize = LittleEndian::fromHost(extra.size());
  std::memcpy(pos, &extraSize, sizeof(extraSize));
  pos += sizeof(extraSize);

  // Copy iv
  std::memcpy(pos, iv.data(), LittleEndian::fromHost(iv.size()));
  pos += iv.size();

  // Copy iv size
  size_type ivSize = LittleEndian::fromHost(iv.size());
  std::memcpy(pos, &ivSize, sizeof(ivSize));
  pos += sizeof(ivSize);

  // Copy hash
  std::memcpy(pos, hash.data(), LittleEndian::fromHost(hash.size()));
  pos += hash.size();

  // Copy algo
  /// Note: In the context of endianness, there is no need for conversion functions for
  /// std::uint8_t (byte) because endianness only affects data types larger than one byte.
  std::memcpy(pos, &algo, sizeof(algo));
  pos += sizeof(algo);

  // Copy version
  decltype(version) versionLE = LittleEndian::fromHost(version);
  std::memcpy(pos, &versionLE, sizeof(versionLE));
  pos += sizeof(versionLE);

  // Copy magic
  decltype(magic) magicLE = LittleEndian::fromHost(magic);
  std::memcpy(pos, &magicLE, sizeof(magicLE));
  return out;
}

Meta::EnigmaFooter Meta::EnigmaFooter::fromBytes(const byte *bytes, const std::size_t bytesSize) {
  if (!Meta::isEnigmaCipher(bytes, bytesSize)) {
    throw std::invalid_argument("Invalid/Corrupted Enigma Cipher");
  }
  EnigmaFooter footer{};
  const byte *pos = bytes + bytesSize;

  // Copy magic (reading from the end)
  pos -= sizeof(footer.magic);
  std::memcpy(&footer.magic, pos, sizeof(footer.magic));
  footer.magic = LittleEndian::toHost(footer.magic);

  // Copy version
  pos -= sizeof(footer.version);
  std::memcpy(&footer.version, pos, sizeof(footer.version));
  footer.version = LittleEndian::toHost(footer.version);

  // Copy algo
  pos -= sizeof(footer.algo);
  std::memcpy(&footer.algo, pos, sizeof(footer.algo));
  /// Note: In the context of endianness, there is no need for conversion functions for
  /// std::uint8_t (byte) because endianness only affects data types larger than one byte.

  // Copy hash
  pos -= footer.hash.size();
  std::memcpy(footer.hash.data(), pos, footer.hash.size());

  // Copy iv size
  size_type ivSize{};
  pos -= sizeof(size_type);
  std::memcpy(&ivSize, pos, sizeof(size_type));
  ivSize = LittleEndian::toHost(ivSize);

  // Copy iv
  footer.iv.resize(ivSize);
  pos -= ivSize;
  std::memcpy(footer.iv.data(), pos, footer.iv.size());

  // Copy extra size
  size_type extraSize{};
  pos -= sizeof(size_type);
  std::memcpy(&extraSize, pos, sizeof(size_type));
  extraSize = LittleEndian::toHost(extraSize);

  // Copy extra
  footer.extra.resize(extraSize);
  pos -= extraSize;
  std::memcpy(footer.extra.data(), pos, footer.extra.size());

  return footer;
}
Meta::EnigmaFooter Meta::EnigmaFooter::fromBytes(const std::vector<byte>& bytes) {
  return fromBytes(bytes.data(), bytes.size());
}

Meta::EnigmaFooter Meta::EnigmaFooter::fromFile(const fs::path& filename) {
  EnigmaFooter footer{};
  std::ifstream ifs{filename, std::ios::binary | std::ios::ate};
  if (!ifs) throw std::runtime_error("Could not open file " + filename.string());


  // seek backwards from the end
  std::streamoff offset = sizeof(footer.magic);
  // Read magic
  ifs.seekg(-offset, std::ios::end);
  ifs.read(reinterpret_cast<char *>(&footer.magic), sizeof(footer.magic));
  ENIGMA_ASSERT_OR_THROW(ifs.good(), "Failed to read EnigmaFooter::magic from file " + filename.string());
  footer.magic = LittleEndian::toHost(footer.magic);

  // Read version
  offset += sizeof(footer.version);
  ifs.seekg(-offset, std::ios::end);
  ifs.read(reinterpret_cast<char *>(&footer.version), sizeof(footer.version));
  ENIGMA_ASSERT_OR_THROW(ifs.good(), "Failed to read EnigmaFooter::version from file " + filename.string());
  footer.version = LittleEndian::toHost(footer.version);

  // Read algo
  offset += sizeof(footer.algo);
  ifs.seekg(-offset, std::ios::end);
  ifs.read(reinterpret_cast<char *>(&footer.algo), sizeof(footer.algo));
  ENIGMA_ASSERT_OR_THROW(ifs.good(), "Failed to read EnigmaFooter::algo from file " + filename.string());
  /// Note: In the context of endianness, there is no need for conversion functions for
  /// std::uint8_t (byte) because endianness only affects data types larger than one byte.

  // Read hash
  offset += footer.hash.size();
  ifs.seekg(-offset, std::ios::end);
  ifs.read(reinterpret_cast<char *>(footer.hash.data()), footer.hash.size());
  ENIGMA_ASSERT_OR_THROW(ifs.good(), "Failed to read EnigmaFooter::hash from file " + filename.string());

  // Read iv size
  size_type ivSize{};
  offset += sizeof(size_type);
  ifs.seekg(-offset, std::ios::end);
  ifs.read(reinterpret_cast<char *>(&ivSize), sizeof(ivSize));
  ENIGMA_ASSERT_OR_THROW(ifs.good(), "Failed to read EnigmaFooter::ivSize from file " + filename.string());
  ivSize = LittleEndian::toHost(ivSize);

  // Read iv
  offset += ivSize;
  footer.iv.resize(ivSize);
  ifs.seekg(-offset, std::ios::end);
  ifs.read(reinterpret_cast<char *>(footer.iv.data()), footer.iv.size());
  ENIGMA_ASSERT_OR_THROW(ifs.good(), "Failed to read EnigmaFooter::iv from file " + filename.string());

  // Read extra size
  size_type extraSize{};
  offset += sizeof(size_type);
  ifs.seekg(-offset, std::ios::end);
  ifs.read(reinterpret_cast<char *>(&extraSize), sizeof(extraSize));
  ENIGMA_ASSERT_OR_THROW(ifs.good(), "Failed to read EnigmaFooter::extraSize from file " + filename.string());
  extraSize = LittleEndian::toHost(extraSize);

  // Read extra
  offset += extraSize;
  footer.extra.resize(extraSize);
  ifs.seekg(-offset, std::ios::end);
  ifs.read(reinterpret_cast<char *>(footer.extra.data()), footer.extra.size());
  ENIGMA_ASSERT_OR_THROW(ifs.good(), "Failed to read EnigmaFooter::extra from file " + filename.string());

  ifs.close();

  return footer;
}
Meta::EnigmaFooter Meta::EnigmaFooter::fromBase64(const std::string& base64) {
  return fromBytes(Base64::Decode(reinterpret_cast<const byte *>(base64.data()), base64.size()));
}


Meta::size_type Meta::EnigmaCipherChunk::sizeInBytes() const noexcept {
  return sizeof(magic) +
         sizeof(size_type) /*cipherSize*/ + cipher.size() +
         sizeof(size_type) /*extraSize*/ + extra.size();
}

std::vector<byte> Meta::EnigmaCipherChunk::toBytes() const {
  std::vector<byte> out(this->sizeInBytes(), '\000');
  byte *pos = out.data();
  // Copy magic
  decltype(magic) magicLE = LittleEndian::fromHost(magic);
  std::memcpy(pos, &magicLE, sizeof(magicLE));
  pos += sizeof(magicLE);

  // Copy cipher size
  size_type cipherSize = LittleEndian::fromHost(cipher.size());
  std::memcpy(pos, &cipherSize, sizeof(cipherSize));
  pos += sizeof(cipherSize);

  // Copy cipher
  std::memcpy(pos, cipher.data(), cipher.size());
  pos += cipher.size();

  // Copy extra size
  size_type extraSize = LittleEndian::fromHost(extra.size());
  std::memcpy(pos, &extraSize, sizeof(extraSize));
  pos += sizeof(extraSize);

  // Copy extra
  std::memcpy(pos, extra.data(), extra.size());
  pos += extra.size();


  return out;
}


bool Meta::isEnigmaFile(const fs::path& filename) {
  if (!fs::is_regular_file(filename)) {
    ENIGMA_ERROR("Could not open file " + filename.string());
    return false;
  }
  std::ifstream ifs{filename, std::ios::binary | std::ios::ate};
  if (!ifs.good()) {
    ENIGMA_ERROR("Could not open file " + filename.string());
    return false;
  }
  const auto fileSize = ifs.tellg();
  if (fileSize < static_cast<std::streampos>(sizeof(ENIGMA_MAGIC))) {
    ENIGMA_ERROR("Not an Enigma encrypted file " + filename.string());
    return false;
  }
  ifs.seekg(-static_cast<std::streamoff>(sizeof(ENIGMA_MAGIC)), std::ios::end); // seek backwards from the end
  if (!ifs.good()) {
    ENIGMA_ERROR("Could not seek Enigma magic from file " + filename.string());
    return false;
  }
  magic_t magic{};
  ifs.read(reinterpret_cast<char *>(&magic), sizeof(magic));
  if (!ifs.good()) {
    ENIGMA_ERROR("Could not read Enigma magic from file " + filename.string());
    return false;
  }
  magic = LittleEndian::toHost(magic);
  return magic == ENIGMA_MAGIC;
}

bool Meta::isEnigmaCipher(const byte *cipher, const std::size_t cipherSize) {
  if (cipherSize < sizeof(ENIGMA_MAGIC)) {
    return false;
  }
  const byte *pos = cipher + cipherSize;
  // Copy magic ONLY (reading from the end)
  magic_t magic{};
  pos -= sizeof(magic_t);
  std::memcpy(&magic, pos, sizeof(magic_t));
  magic = LittleEndian::toHost(magic);
  return magic == ENIGMA_MAGIC;
  // return EnigmaFooter::fromBytes(cipher, cipherSize).magic == ENIGMA_MAGIC;
}
bool Meta::isEnigmaCipher(const std::vector<byte>& cipher) {
  return Meta::isEnigmaCipher(cipher.data(), cipher.size());
}

void Meta::readCipherChunks(const fs::path& filename, const std::function<bool(Meta::EnigmaCipherChunk&&)>& callback) {
  std::ifstream ifs{filename, std::ios::binary};
  ENIGMA_ASSERT_OR_THROW(ifs.good(), "Could not open file " + filename.string());

  while (!ifs.eof()) {
    Meta::magic_t magic{};
    ifs.read(reinterpret_cast<char *>(&magic), sizeof(magic));
    if (magic == Meta::ENIGMA_CIPHER_CHUNK_MAGIC) {
      Meta::EnigmaCipherChunk cipherChunk{};
      // read cipher size
      Meta::size_type cipherSize{};
      ifs.read(reinterpret_cast<char *>(&cipherSize), sizeof(cipherSize));
      ENIGMA_ASSERT_OR_THROW(ifs.good(), "Could not read cipher size from file " + filename.string());
      cipherSize = LittleEndian::toHost(cipherSize);

      // read cipher
      cipherChunk.cipher.resize(cipherSize);
      ifs.read(reinterpret_cast<char *>(cipherChunk.cipher.data()), cipherSize);
      ENIGMA_ASSERT_OR_THROW(ifs.good(), "Could not read cipher from file " + filename.string());
      // read extra size
      Meta::size_type extraSize{};
      ifs.read(reinterpret_cast<char *>(&extraSize), sizeof(extraSize));
      ENIGMA_ASSERT_OR_THROW(ifs.good(), "Could not read extra size from file " + filename.string());
      extraSize = LittleEndian::toHost(extraSize);

      // read extra
      cipherChunk.extra.resize(extraSize);
      ifs.read(reinterpret_cast<char *>(cipherChunk.extra.data()), extraSize);
      ENIGMA_ASSERT_OR_THROW(ifs.good(), "Could not read extra from file " + filename.string());

      // Serve
      if (!callback(std::move(cipherChunk))) break;
    }
    // Note: no worries about this reading EnigmaFooter, it only reads from magic scope.
  }
  ifs.close();
}
NS_ENIGMA_END