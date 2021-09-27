#pragma once
#ifndef ENIGMA_DATABASE_H
#define ENIGMA_DATABASE_H

#include <Core/Core.hpp>
#include <Logger/Logger.hpp>
#include "Models/Encryption.hpp"
#include <Utility/SizeUtils.hpp>
#include <Utility/StringUtils.hpp>
#include <Utility/FileUtils.hpp>

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
class ENIGMA_API Database final
{
	ENIGMA_STATIC_CLASS(Database);

 public:
	/** Encryption Table order by column */
	enum class OrderBy : byte
	{
		ID,
		Title,
		DateTime,
		Size,

		ENIGMA_ENUM_DECLARE_BEGIN_END(ID)
	};
	/** To help stringify enum OrderBy */
	friend const char* operator *(OrderBy order_by) 
	{
#define CASE_STR(e) case OrderBy::e: return #e
		switch (order_by)
		{
			CASE_STR(ID);
			CASE_STR(Title);
			CASE_STR(DateTime);
			CASE_STR(Size);
			default: return "<unknown OrderBy>";
		}
#undef CASE_STR
	}
	friend std::ostream& operator<<(std::ostream& os, const OrderBy order_by) noexcept // for constructing sql
	{
		switch (order_by)
		{
			case OrderBy::ID: os << " e.ide"; break;
			case OrderBy::Title: os << " e.title"; break;
			case OrderBy::DateTime: os << " e.date_time"; break;
			case OrderBy::Size: os << " e.size"; break;
		}
		return os;
	}
	/** Encryption Table order column */
	enum class Order : byte
	{
		Ascending,
		Descending,

		ENIGMA_ENUM_DECLARE_BEGIN_END(Ascending)
	};
	/** To help stringify enum Order */
	friend const char* operator *(Order order)
	{
#define CASE_STR(e) case Order::e: return #e
		switch (order)
		{
			CASE_STR(Ascending);
			CASE_STR(Descending);
			default: return "<unknown Order>";
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
	/** Add Encryption to Encryptions table, returns true on success */
	static bool AddEncryption(const std::unique_ptr<Encryption>& e);

	/** Returns cipher from database by encryption id */
	static std::unique_ptr<Cipher> GetCipherByEncryptionID(const i64 ide);

	/** Delete Encryption record by id, returns true if successfully deleted */
	static bool DeleteEncryption(const i64 ide);

	/** Delete all saved encryptions from the database (with their cipher) */
	static bool DeleteAllEncryptions();

	/** Returns how many encryption records are saved */
	static i64 GetEncryptionsCount();

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

	/** Get an Encyrption by id with desired columns for optimization */
	template<const bool title, const bool cipher, const bool date_time, const bool size, const bool is_file>
	inline static std::unique_ptr<Encryption> GetEncryptionByID(const i64 ide)
	{
		ENIGMA_TRACE_CURRENT_FUNCTION();

		ENIGMA_ASSERT_OR_RETURN(m_database, "Database was not initialized", nullptr);
		try
		{
			// Select e.id, e.title, c.data, e.date_time, e.is_file from Encryptions e JOIN Ciphers c ON e.id = c.id_enc
			
			// Construct SQL
			std::ostringstream sql{};
			{
				sql << "SELECT e.ide";
				if constexpr (title) sql << ", e.title";
				if constexpr (cipher) sql << ", c.idc, c.data, c.ide";
				if constexpr (date_time) sql << ", e.date_time";
				if constexpr (size) sql << ", e.size";
				if constexpr (is_file) sql << ", e.is_file";
				sql << " FROM Encryption e";
				if constexpr (cipher) sql << " JOIN Cipher c ON e.ide = c.ide";
				sql << " WHERE e.ide = " << ide;
#if defined(ENIGMA_DEBUG)
				ENIGMA_LOG("SQL: {0}", sql.str());
#endif
			}

			const auto query = std::make_unique<SQLite::Statement>(*m_database, sql.str());
			auto e = std::make_unique<Encryption>();

			if (query->executeStep())
			{
				i32 i{ 0 }; // for getColumn, use index starts by 0
				e->ide = query->getColumn(i++).getInt64();
				if constexpr (title) e->title = query->getColumn(i++).getString();
				if constexpr (cipher)
				{
					e->cipher.idc  = query->getColumn(i++).getInt64();
					//e->cipher.data = reinterpret_cast<const char*>(query->getColumn(i++).getBlob());
					e->cipher.data = std::move(query->getColumn(i++).getString());
					e->cipher.ide  = query->getColumn(i++).getInt64();
				}
				if constexpr (date_time) e->date_time = query->getColumn(i++).getString();
				if constexpr (size) e->size = static_cast<decltype(Encryption::size)>(query->getColumn(i++).getInt64());
				if constexpr (is_file) e->is_file = static_cast<decltype(Encryption::is_file)>(query->getColumn(i++).getInt());
			}

			return e;
		}
		catch (const SQLite::Exception& e)
		{
			ENIGMA_ERROR("{0}", e.what());
			return nullptr;
		}
	}

	/** Get all Encryptions with desired columns for optimization */
	template<const bool title, const bool cipher, const bool date_time, const bool size, const bool is_file>
	inline static std::vector<std::unique_ptr<Encryption>> GetAllEncryptions(OrderBy order_by = OrderBy::ID, Order order = Order::Descending)
	{			
		ENIGMA_TRACE_CURRENT_FUNCTION();

		ENIGMA_ASSERT_OR_RETURN(m_database, "Database was not initialized", {});

		std::vector<std::unique_ptr<Encryption>> encryptions{};
		try
		{
			// Construct SQL
			std::ostringstream sql{};
			{
				sql << "SELECT e.ide";
				if constexpr (title) sql << ", e.title";
				if constexpr (cipher) sql << ", c.idc, c.data, c.ide";
				if constexpr (date_time) sql << ", e.date_time";
				if constexpr (size) sql << ", e.size";
				if constexpr (is_file) sql << ", e.is_file";
				sql << " FROM Encryption e";
				if constexpr (cipher) sql << " JOIN Cipher c ON e.ide = c.idc";
				sql << " ORDER BY" << order_by << order;
			}
			
#if defined(ENIGMA_DEBUG)
			ENIGMA_LOG("SQL: {0}", sql.str());
#endif

			const auto query = std::make_unique<SQLite::Statement>(*m_database, sql.str());

			// Loop to execute the query step by step, to get rows of result
			while (query->executeStep())
			{
				auto e = std::make_unique<Encryption>();

				i32 i{ 0 }; // for getColumn, use index starts by 0
				e->ide = query->getColumn(i++).getInt64();
				if constexpr (title) e->title = query->getColumn(i++).getString();
				if constexpr (cipher)
				{
					e->cipher.idc  = query->getColumn(i++).getInt64();
					//e->cipher.data = reinterpret_cast<const char*>(query->getColumn(i++).getBlob());
					e->cipher.data = std::move(query->getColumn(i++).getString());
					e->cipher.ide  = query->getColumn(i++).getInt64();
				}
				if constexpr (date_time) e->date_time = query->getColumn(i++).getString();
				if constexpr (size) e->size = static_cast<decltype(Encryption::size)>(query->getColumn(i++).getInt64());
				if constexpr (is_file) e->is_file = static_cast<decltype(Encryption::is_file)>(query->getColumn(i++).getInt());

				encryptions.emplace_back(std::move(e));
			}
		}
		catch (const SQLite::Exception& e)
		{
			ENIGMA_ERROR("{0}", e.what());
		}
		return encryptions;
	}


	/** Search Encryptions by title using keyword LIKE %QUERY% */
	template<const bool title, const bool cipher, const bool date_time, const bool size, const bool is_file> // select which columns to return (for optimization)
	inline static std::vector<std::unique_ptr<Encryption>> SearchEncryptionsByTitle(const String& qtitle, OrderBy order_by = OrderBy::ID, Order order = Order::Descending)
	{
		ENIGMA_TRACE_CURRENT_FUNCTION();

		ENIGMA_ASSERT_OR_RETURN(m_database, "Database was not initialized", {});

		std::vector<std::unique_ptr<Encryption>> encryptions{};
		try
		{
			// Construct SQL
			std::ostringstream sql{};
			{
				sql << "SELECT e.ide";
				if constexpr (title) sql << ", e.title";
				if constexpr (cipher) sql << ", c.idc, c.data, c.ide";
				if constexpr (date_time) sql << ", e.date_time";
				if constexpr (size) sql << ", e.size";
				if constexpr (is_file) sql << ", e.is_file";
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
			while (query->executeStep())
			{
				auto e = std::make_unique<Encryption>();

				i32 i{ 0 }; // for getColumn, use index starts by 0
				e->ide = query->getColumn(i++).getInt64();
				if constexpr (title) e->title = query->getColumn(i++).getString();
				if constexpr (cipher)
				{
					e->cipher.idc = query->getColumn(i++).getInt64();
					//e->cipher.data = reinterpret_cast<const char*>(query->getColumn(i++).getBlob());
					e->cipher.data = std::move(query->getColumn(i++).getString());
					e->cipher.ide = query->getColumn(i++).getInt64();
				}
				if constexpr (date_time) e->date_time = query->getColumn(i++).getString();
				if constexpr (size) e->size = static_cast<decltype(Encryption::size)>(query->getColumn(i++).getInt64());
				if constexpr (is_file) e->is_file = static_cast<decltype(Encryption::is_file)>(query->getColumn(i++).getInt());

				encryptions.emplace_back(std::move(e));
			}
		}
		catch (const SQLite::Exception& e)
		{
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
	static void Vacuum() noexcept 
	{
		/*ENIGMA_INFO("Vacuuming SQLite3 database to optimize disk space...");
		(void)m_database->exec("VACUUM");*/

		// Only vacuum if changes to the database were made.
		const i32 total_changes = m_database->getTotalChanges();
		if (total_changes > 0)
		{
			ENIGMA_INFO("{0} database changes were made, Vacuuming database to optimize disk space...", total_changes);
			(void)m_database->exec("VACUUM");
		}
		//else
		//	ENIGMA_INFO("No database changes were made, skipping vacuum disk optimization.");
	}

	

private:
	inline static std::unique_ptr<SQLite::Database> m_database{ nullptr }; /**< Database connection configuered on Initialize() */
};

/*
Notes:
for bind, use index starts by 1
for getColumn, use index starts by 0
*/

NS_ENIGMA_END
#endif // !ENIGMA_DATABASE_H

