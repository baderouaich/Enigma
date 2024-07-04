#pragma once
#include <Core/Core.hpp>

NS_ENIGMA_BEGIN

/**
*	CipherChunk Model | table: CipherChunks
*	When we encrypt a buffer (file or text), we get back cipher (aka: encrypted content)
*	Cipher can be large when encrypting large files so we save it in the database in chunks
*/
struct CipherChunk {
    static constexpr const std::string_view TABLE_NAME = "CipherChunks";

    std::int64_t idc{};                  /**< auto incremental id */
    std::int64_t ide{};                  /**< Which encryption this cipher chunk belongs to? */
    std::int64_t offset{};               /**< Starting offset of this chunk */
    std::int64_t size{};                 /**< Cipher chunk size */
    std::vector<::Enigma::byte> bytes{}; /**< BLOB cipher chunk binary */

    ~CipherChunk() = default;

  public:
    constexpr bool operator==(const CipherChunk& c) const noexcept { return idc == c.idc; }
    friend std::ostream& operator<<(std::ostream& os, const CipherChunk& c) noexcept {
      return os
             << "CipherChunk(idc: " << c.idc << ','
             << "ide: " << c.ide << ','
             << "offset: " << c.offset << ','
             << "size: " << c.size << ','
             << "data: " << std::string(c.bytes.begin(), c.bytes.end()) << ')';
    }
    std::string toString() const noexcept {
      std::ostringstream oss{};
      oss << *this;
      return oss.str();
    }
};

NS_ENIGMA_END
