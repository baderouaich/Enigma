#pragma once

#include <Core/Core.hpp>
#include <Logger/Logger.hpp>
#include <Utility/FileUtils.hpp>
#include <Utility/SizeUtils.hpp>
#include <Utility/StringUtils.hpp>

#include <SQLiteCpp/SQLiteCpp.h>
//#include <Utility/GZip.hpp>
#include "Models/Encryption.hpp"
#include <memory>
#include <vector>


NS_ENIGMA_BEGIN

/**
* Enigma SQLite database to store and retrieve encryption records
* User can export and import Database file to clouds or drives
* Database file will store Encrypted Text and Encrypted Files ciphers and other info
*/
class Database final {
    ENIGMA_STATIC_CLASS(Database);
  public:
#pragma region enums
    /** Encryption Table order by column */
    enum class OrderBy : byte {
      ID,
      Title,
      DateTime,
      Size,

      ENIGMA_ENUM_DECLARE_BEGIN_END(ID)
    };
    /** To help stringify enum OrderBy */
    friend const char *operator*(OrderBy order_by) {
#define CASE_STR(e) \
  case OrderBy::e:  \
    return #e
      switch (order_by) {
        CASE_STR(ID);
        CASE_STR(Title);
        CASE_STR(DateTime);
        CASE_STR(Size);
        default:
          return "<unknown OrderBy>";
      }
#undef CASE_STR
    }
    friend std::ostream& operator<<(std::ostream& os, const OrderBy order_by) noexcept // for constructing sql
    {
      switch (order_by) {
        case OrderBy::ID:
          os << "ide";
          break;
        case OrderBy::Title:
          os << "title";
          break;
        case OrderBy::DateTime:
          os << "date_time";
          break;
        case OrderBy::Size:
          os << "size";
          break;
        default:
          break;
      }
      return os;
    }
    /** Encryption Table order column */
    enum class Order : byte {
      Ascending,
      Descending,

      ENIGMA_ENUM_DECLARE_BEGIN_END(Ascending)
    };
    /** To help stringify enum Order */
    friend const char *operator*(Order order) {
#define CASE_STR(e) \
  case Order::e:    \
    return #e
      switch (order) {
        CASE_STR(Ascending);
        CASE_STR(Descending);
        default:
          return "<unknown Order>";
      }
#undef CASE_STR
    }

    friend std::ostream& operator<<(std::ostream& os, const Order order) noexcept // for constructing sql
    {
      return os << (order == Order::Ascending ? "ASC" : "DESC");
    }
#pragma endregion enums

  public:
    /** Initializes SQLite database connection */
    static void initialize();
    /** Shuts down SQLite connection */
    static void shutdown();

  public:
    /** Add Encryption record to database table
    *	@return inserted encryption id std::int64_t>=0 on success, -1 on failure
    */
    static std::int64_t addEncryption(const std::unique_ptr<Encryption>& e);

    /** Add CipherChunk record to database table
    * @param ide: Encryption id this CipherChunk belongs to
    *	@return inserted CipherChunk id std::int64_t>=0 on success, -1 on failure
    */
    static std::int64_t addCipherChunk(const std::unique_ptr<CipherChunk>& cc);

    /** Get an Encryption by id */
    static std::unique_ptr<Encryption> getEncryption(const std::int64_t ide);

    /** Get a CipherChunk by Encryption id
     * Used for small text encryptions
     **/
    static std::unique_ptr<CipherChunk> getCipherChunk(const std::int64_t ide);

    /** Get CipherChunks iteratively by Encryption id, ordered by offset from 0 -> LAST
     * Used for large files encryptions
     * */
    static void getCipherChunks(const std::int64_t ide, const std::function<bool(std::unique_ptr<CipherChunk>&& cc)>& callback);

    /** Get a CipherChunk by Encryption id */
    static std::vector<std::unique_ptr<Encryption>> getAllEncryptions(const OrderBy order_by = OrderBy::ID, const Order order = Order::Descending);

    /** Delete Encryption record by id, returns true if successfully deleted
     * Note: all associated CipherChunks with this encryption will be automatically
     * deleted due ON DELETE CASCADE
     */
    static bool deleteEncryption(const std::int64_t ide);

    /** Delete all Encryption records by id, returns true if successfully deleted
     * Note: all associated CipherChunks with all encryptions will be automatically
     * deleted due ON DELETE CASCADE
     */
    static bool deleteAllEncryptions();

    /**
     * Search Encryptions by title using keyword LIKE %QUERY%
     */
    static std::vector<std::unique_ptr<Encryption>> searchEncryptionsByTitle(const std::string& qtitle, OrderBy order_by = OrderBy::ID, Order order = Order::Descending);

  public: // Accessors
    /** Returns SQLite database connection instance */
    static const std::unique_ptr<SQLite::Database>& getStorage() noexcept { return m_database; }

  public: // Modifiers
    /**
	*	Cleans up allocated disk space for deleted data, blob...
	*
	*	When you insert or delete data from the tables, the indexes and tables become fragmented, 
	*	especially for the database that has a high number of inserts, updates, and deletes.
	*	@note https://www.sqlitetutorial.net/sqlite-vacuum/
	*/
    static void Vacuum() noexcept;

    static void Export(const fs::path& filename);
    static void Import(const fs::path& filename);

  private:
    inline static std::unique_ptr<SQLite::Database> m_database{nullptr}; /**< Database connection created on initialize() */
};

/**
*	Notes:
*	for bind, use index starts by 1
*	for getColumn, use index starts by 0
*/

NS_ENIGMA_END
