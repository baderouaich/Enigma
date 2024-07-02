#pragma once
#include <Core/Core.hpp>
#include <Logger/Logger.hpp>
#include <Utility/SizeUtils.hpp>
#include <fstream>
#include <iostream>
#include <sha.h>
//#include <sha3.h>

/**
 * Notes:
 * - If you add a new member to EnigmaFooter or EnigmaCipherChunk, make sure you handle it in:
 *   sizeInBytes(), toBytes(), fromBytes(), fromFile()
 *
 */
namespace Enigma {
  using magic_t = std::uint64_t;
  using algo_t = byte;
  using size_type = std::size_t;
  static constexpr const magic_t ENIGMA_MAGIC = 0x456e69676d61;
  static constexpr const magic_t ENIGMA_CIPHER_CHUNK_MAGIC = 0x43456e69676d6143;
  static constexpr const size_type ENIGMA_BUFFER_DEFAULT_SIZE = ENIGMA_MB_TO_BYTES(1); // 1MB chunk size

#pragma pack(push, 1)
  /// Encrypted buffer footer
  struct EnigmaFooter {
      /// Enigma magic number
      magic_t magic = ENIGMA_MAGIC;
      /// Enigma version used in encryption
      std::uint64_t version = ENIGMA_VERSION_MAJOR * 100'000 + ENIGMA_VERSION_MINOR * 1000 + ENIGMA_VERSION_PATCH;
      /// Algorithm enum id used for encryption
      algo_t algo{};
      /// SHA2-256 Hash of the original file or text, used to ensure decryption is successful.
      std::array<byte, CryptoPP::SHA256::DIGESTSIZE> hash{};
      /// IV used encryption
      std::vector<byte> iv{};
      /// Extra data (for example mac for ChaCha20Poly1305)
      std::vector<byte> extra{};


      [[nodiscard]] constexpr size_type sizeInBytes() const noexcept;

      /// Convert instance of this struct to an array of bytes.
      [[nodiscard]] std::vector<byte> toBytes() const;

      [[nodiscard]] static EnigmaFooter fromBytes(const std::vector<byte>& bytes);

      [[nodiscard]] static EnigmaFooter fromFile(const fs::path& filename);
  };

  struct EnigmaCipherChunk {
      magic_t magic{ENIGMA_CIPHER_CHUNK_MAGIC};
      std::vector<byte> cipher;
      std::vector<byte> extra;

      [[nodiscard]] constexpr size_type sizeInBytes() const noexcept;

      [[nodiscard]] std::vector<byte> toBytes() const;
  };
#pragma pack(pop)

  // Sanity checks
  // Ensure that size of magic_t is same on all platforms,
  // so if you encrypt in Windows, you can decrypt in Linux..
  static_assert(sizeof(magic_t) == 8, "Magic type must be of 8 bytes");
  static_assert(sizeof(algo_t) == 1, "Algo type must be of 1 byte");
  static_assert(sizeof(size_type) == 8, "Size type must be of 8 bytes");

  /// Returns true if the file is encrypted with enigma
  static bool isEnigmaFile(const fs::path& filename);

  /// Returns true if the file is encrypted with enigma
  static bool isEnigmaCipher(const std::vector<byte>& cipher);

  /// Read cipher chunks from an Enigma encrypted file
  static void readCipherChunks(const fs::path& filename, const std::function<bool(EnigmaCipherChunk&&)>& callback);
}