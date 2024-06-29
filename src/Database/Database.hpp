#pragma once
#ifndef ENIGMA_DATABASE_H
#define ENIGMA_DATABASE_H

#include "Models/Encryption.hpp"
#include <Core/Core.hpp>
#include <Logger/Logger.hpp>
#include <Utility/FileUtils.hpp>
#include <Utility/SizeUtils.hpp>
#include <Utility/StringUtils.hpp>

#include <SQLiteCpp/SQLiteCpp.h>
#include <Utility/GZip.hpp>

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
          os << " e.ide";
          break;
        case OrderBy::Title:
          os << " e.title";
          break;
        case OrderBy::DateTime:
          os << " e.date_time";
          break;
        case OrderBy::Size:
          os << " e.size";
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
      return os << (order == Order::Ascending ? " ASC" : " DESC");
    }

  public:
    /** Initializes SQLite database connection */
    static void Initialize();
    /** Shuts down SQLite connection */
    static void Shutdown();

  public: // Encryption Operations
    /** Add Encryption record to database table
	*	@return inserted encryption id std::int64_t>=0 on success, -1 on failure
	*/
    static std::int64_t AddEncryption(const std::unique_ptr<Encryption>& e);

    /** Returns cipher from database by encryption id */
    static std::unique_ptr<Cipher> GetCipherByEncryptionID(const std::int64_t ide);

    /** Delete Encryption record by id, returns true if successfully deleted */
    static bool DeleteEncryption(const std::int64_t ide);

    /** Delete all saved encryptions from the database (with their cipher) */
    static bool DeleteAllEncryptions();

    /** Returns how many encryption records are saved */
    static std::int64_t GetEncryptionsCount();

    /** Get an Encryption by id with desired columns for optimization */
    template<const bool title, const bool cipher, const bool date_time, const bool size, const bool is_file, const bool file_ext>
    inline static std::unique_ptr<Encryption> GetEncryptionByID(const std::int64_t ide) {
      ENIGMA_TRACE_CURRENT_FUNCTION();

      ENIGMA_ASSERT_OR_RETURN(m_database, "Database was not initialized", nullptr);
      try {
        // Select e.id, e.title, c.data, e.date_time, e.is_file, e.file_ext from Encryptions e JOIN Ciphers c ON e.id = c.id_enc

        // Construct SQL
        std::ostringstream sql{};
        {
          sql << "SELECT e.ide";
          if constexpr (title) sql << ", e.title";
          if constexpr (cipher) sql << ", c.idc, c.data, c.ide";
          if constexpr (date_time) sql << ", e.date_time";
          if constexpr (size) sql << ", e.size";
          if constexpr (is_file) sql << ", e.is_file";
          if constexpr (file_ext) sql << ", e.file_ext";
          sql << " FROM Encryption e";
          if constexpr (cipher) sql << " JOIN Cipher c ON e.ide = c.ide";
          sql << " WHERE e.ide = " << ide;
#if defined(ENIGMA_DEBUG)
          ENIGMA_LOG("SQL: {0}", sql.str());
#endif
        }

        const auto query = std::make_unique<SQLite::Statement>(*m_database, sql.str());
        auto e = std::make_unique<Encryption>();

        if (query->executeStep()) {
          std::int32_t i{0}; // for getColumn, use index starts by 0
          e->ide = query->getColumn(i++).getInt64();
          if constexpr (title) e->title = query->getColumn(i++).getString();
          if constexpr (cipher) {
            e->cipher.idc = query->getColumn(i++).getInt64();
            //e->cipher.data = reinterpret_cast<const char*>(query->getColumn(i++).getBlob());
            e->cipher.data = std::move(query->getColumn(i++).getString());
            e->cipher.ide = query->getColumn(i++).getInt64();
          }
          if constexpr (date_time) e->date_time = query->getColumn(i++).getString();
          if constexpr (size) e->size = static_cast<decltype(Encryption::size)>(query->getColumn(i++).getInt64());
          if constexpr (is_file) e->is_file = static_cast<decltype(Encryption::is_file)>(query->getColumn(i++).getInt());
          if constexpr (file_ext) e->file_ext = query->getColumn(i++).getString();
        }

        return e;
      } catch (const SQLite::Exception& e) {
        ENIGMA_ERROR("{0}", e.what());
        return nullptr;
      }
    }

    /** Get all Encryptions with desired columns for optimization */
    template<const bool title, const bool cipher, const bool date_time, const bool size, const bool is_file, const bool file_ext>
    inline static std::vector<std::unique_ptr<Encryption>> GetAllEncryptions(OrderBy order_by = OrderBy::ID, Order order = Order::Descending) {
      ENIGMA_TRACE_CURRENT_FUNCTION();

      ENIGMA_ASSERT_OR_RETURN(m_database, "Database was not initialized", {});

      std::vector<std::unique_ptr<Encryption>> encryptions{};
      try {
        // Construct SQL
        std::ostringstream sql{};
        {
          sql << "SELECT e.ide";
          if constexpr (title) sql << ", e.title";
          if constexpr (cipher) sql << ", c.idc, c.data, c.ide";
          if constexpr (date_time) sql << ", e.date_time";
          if constexpr (size) sql << ", e.size";
          if constexpr (is_file) sql << ", e.is_file";
          if constexpr (file_ext) sql << ", e.file_ext";
          sql << " FROM Encryption e";
          if constexpr (cipher) sql << " JOIN Cipher c ON e.ide = c.idc";
          sql << " ORDER BY" << order_by << order;
        }

#if defined(ENIGMA_DEBUG)
        ENIGMA_LOG("SQL: {0}", sql.str());
#endif

        const auto query = std::make_unique<SQLite::Statement>(*m_database, sql.str());

        // Loop to execute the query step by step, to get rows of result
        while (query->executeStep()) {
          auto e = std::make_unique<Encryption>();

          std::int32_t i{0}; // for getColumn, use index starts by 0
          e->ide = query->getColumn(i++).getInt64();
          if constexpr (title) e->title = query->getColumn(i++).getString();
          if constexpr (cipher) {
            e->cipher.idc = query->getColumn(i++).getInt64();
            //e->cipher.data = reinterpret_cast<const char*>(query->getColumn(i++).getBlob());
            e->cipher.data = std::move(query->getColumn(i++).getString());
            e->cipher.ide = query->getColumn(i++).getInt64();
          }
          if constexpr (date_time) e->date_time = query->getColumn(i++).getString();
          if constexpr (size) e->size = static_cast<decltype(Encryption::size)>(query->getColumn(i++).getInt64());
          if constexpr (is_file) e->is_file = static_cast<decltype(Encryption::is_file)>(query->getColumn(i++).getInt());
          if constexpr (file_ext) e->file_ext = query->getColumn(i++).getString();

          encryptions.emplace_back(std::move(e));
        }
      } catch (const SQLite::Exception& e) {
        ENIGMA_ERROR("{0}", e.what());
      }
      return encryptions;
    }

    /** Search Encryptions by title using keyword LIKE %QUERY% */
    template<const bool title, const bool cipher, const bool date_time, const bool size, const bool is_file, const bool file_ext> // select which columns to return (for optimization)
    inline static std::vector<std::unique_ptr<Encryption>> SearchEncryptionsByTitle(const std::string& qtitle, OrderBy order_by = OrderBy::ID, Order order = Order::Descending) {
      ENIGMA_TRACE_CURRENT_FUNCTION();

      ENIGMA_ASSERT_OR_RETURN(m_database, "Database was not initialized", {});

      std::vector<std::unique_ptr<Encryption>> encryptions{};
      try {
        // Construct SQL
        std::ostringstream sql{};
        {
          sql << "SELECT e.ide";
          if constexpr (title) sql << ", e.title";
          if constexpr (cipher) sql << ", c.idc, c.data, c.ide";
          if constexpr (date_time) sql << ", e.date_time";
          if constexpr (size) sql << ", e.size";
          if constexpr (is_file) sql << ", e.is_file";
          if constexpr (file_ext) sql << ", e.file_ext";
          sql << " FROM Encryption e";
          if constexpr (cipher) sql << " JOIN Cipher c ON e.ide = c.idc";
          sql << " WHERE LOWER(e.title) LIKE '%" << StringUtils::LowerCopy(qtitle) << "%'";
          sql << " ORDER BY" << order_by << order;
        }

#if defined(ENIGMA_DEBUG)
        ENIGMA_LOG("SQL: {0}", sql.str());
#endif
        const auto query = std::make_unique<SQLite::Statement>(*m_database, sql.str());

        // Loop to execute the query step by step, to get rows of result
        while (query->executeStep()) {
          auto e = std::make_unique<Encryption>();

          std::int32_t i{0}; // for getColumn, use index starts by 0
          e->ide = query->getColumn(i++).getInt64();
          if constexpr (title) e->title = query->getColumn(i++).getString();
          if constexpr (cipher) {
            e->cipher.idc = query->getColumn(i++).getInt64();
            //e->cipher.data = reinterpret_cast<const char*>(query->getColumn(i++).getBlob());
            e->cipher.data = std::move(query->getColumn(i++).getString());
            e->cipher.ide = query->getColumn(i++).getInt64();
          }
          if constexpr (date_time) e->date_time = query->getColumn(i++).getString();
          if constexpr (size) e->size = static_cast<decltype(Encryption::size)>(query->getColumn(i++).getInt64());
          if constexpr (is_file) e->is_file = static_cast<decltype(Encryption::is_file)>(query->getColumn(i++).getInt());
          if constexpr (file_ext) e->file_ext = query->getColumn(i++).getString();

          encryptions.emplace_back(std::move(e));
        }
      } catch (const SQLite::Exception& e) {
        ENIGMA_ERROR("{0}", e.what());
      }
      return encryptions;
    }

  public: // Accessors
    /** Returns SQLite database connection instance */
    static const std::unique_ptr<SQLite::Database>& GetInstance() noexcept { return m_database; }

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
    inline static std::unique_ptr<SQLite::Database> m_database{nullptr}; /**< Database connection configured on Initialize() */
};

/**
*	Notes:
*	for bind, use index starts by 1
*	for getColumn, use index starts by 0
*/

NS_ENIGMA_END
#endif // !ENIGMA_DATABASE_H


#if 0
// TODO
// Export all encryptions data to a json file
static bool ExportAllEncryptionsJSON(const fs::path& filename)
{
	ENIGMA_TRACE_CURRENT_FUNCTION();
	ENIGMA_ASSERT_OR_RETURN(m_database, "Database was not initialized", false);
	try
	{
		ENIGMA_INFO("Getting all encryptions from database to export to json file {}...", filename.string());
		std::vector<std::unique_ptr<Encryption>> encryptions = Database::GetAllEncryptions<true, true, true, true, true>();
		ENIGMA_INFO("Got {0} Encryption records.", encryptions.size());

		// Make json 
		using namespace nlohmann;
		json json_encryptions = json::array();
		for (const auto& encryption_ptr : encryptions)
		{
			// expand Encryption 
			const auto& [ide, title, cipher, date_time, size, is_file] = *encryption_ptr;

			// make encryption object
			json enc_obj{};
			enc_obj["ide"] = ide;
			enc_obj["title"] = title;

			json cipher_obj{};
			cipher_obj["idc"] = cipher.idc;
			cipher_obj["ide"] = cipher.ide;
			cipher_obj["data"] = cipher.data;
			enc_obj["cipher"] = cipher_obj;

			enc_obj["date_time"] = date_time;
			enc_obj["size"] = size;
			enc_obj["is_file"] = is_file;

			// add obj to array
			json_encryptions.push_back(std::move(enc_obj));
		}

		// we can freeup the memory now.
		encryptions.clear();

		// save made json array to file
		return FileUtils::Write(filename, json_encryptions.dump(-1, 32, false, json::error_handler_t::replace));
	}
	catch (const SQLite::Exception& e)
	{
		ENIGMA_ERROR(e.what());
		return false;
	}
}
#endif
