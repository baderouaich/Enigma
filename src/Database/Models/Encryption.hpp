#pragma once

#include "CipherChunk.hpp"
#include <Core/Core.hpp>

#include <sstream> //	std::ostringstream

NS_ENIGMA_BEGIN

/**
*	Encryption Model | Table: Encryptions
*/
struct Encryption {
    static constexpr const std::string_view TABLE_NAME = "Encryptions";


    std::int64_t ide{};      /**< auto incremental id in range [0, SIZE_MAX] */
    std::string title{};     /**< encryption title, e.g: "this is my Email password" */
    std::string date_time{}; /**< date & time saved by sqlite3 */
    std::int64_t size{};     /**< size of cipher in bytes */
    bool is_file{};          /**< whether its a text or file encryption */
    std::string file_ext{};  /**< file extension to remember file type on decryption */

    ~Encryption() noexcept = default;

  public:
    constexpr bool operator==(const Encryption& e) const noexcept { return ide == e.ide; }
    friend std::ostream& operator<<(std::ostream& os, const Encryption& e) noexcept {
      return os
             << "Encryptions(ide: " << e.ide << ','
             << "title: " << e.title << ','
             << "date_time: " << e.date_time << ','
             << "is_file: " << std::boolalpha << e.is_file << ','
             << "file_ext:" << e.file_ext << ')';
    }
    std::string toString() noexcept {
      std::ostringstream oss{};
      oss << *this;
      return oss.str();
    }
};

NS_ENIGMA_END
