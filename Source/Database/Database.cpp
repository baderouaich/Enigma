#include <pch.hpp>
#include "Database.hpp"

NS_ENIGMA_BEGIN

void Database::Initialize()
{
#ifdef ENIGMA_DEBUG
	ENIGMA_TRACE(ENIGMA_CURRENT_FUNCTION);
#endif
	ENIGMA_INFO("SQLite3 version {0}", SQLite::VERSION);
	try
	{
		//Create dir if not exists
		if (!fs::exists(Constants::Database::DATABASE_FILE_DIR))
		{
			ENIGMA_INFO("Creating Database Directory {0} ...", Constants::Database::DATABASE_FILE_DIR);
			if (!fs::create_directory(Constants::Database::DATABASE_FILE_DIR))
			{
				ENIGMA_CRITICAL("Failed to create database directory: {0}", Constants::Database::DATABASE_FILE_DIR);
				std::exit(EXIT_FAILURE);
			}
		}

		m_database = std::make_unique<SQLite::Database>(
			Constants::Database::DATABASE_FILE_NAME,
			SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE // create if not exists
			);

		// Create Tables If Not Exists
		for (const auto& create_table_sql : Constants::Database::CREATE_TABLES_SQL)
		{
			//ENIGMA_LOG("SQL: {0}", create_table_sql);
			const auto query = std::make_unique<SQLite::Statement>(
				*m_database,
				create_table_sql
				);
			const i32 status = query->exec(); // status 0 == SQLite::OK
			ENIGMA_ASSERT_OR_THROW(status == SQLite::OK,
				"Database failed to create tables with error code " +
				std::to_string(query->getErrorCode()) + 
				" and message: " + String(query->getErrorMsg()));
		}
	
	}
	catch (const SQLite::Exception& e)
	{
		ENIGMA_CRITICAL(e.what());
		std::exit(EXIT_FAILURE);
	}

}

void Database::Shutdown()
{
#ifdef ENIGMA_DEBUG
	ENIGMA_TRACE(ENIGMA_CURRENT_FUNCTION);
#endif
	Vacuum();
}

// Add Encryption to Encryptions table, returns true on success
bool Database::AddEncryption(const std::unique_ptr<Encryption>& e)
{
#ifdef ENIGMA_DEBUG
	ENIGMA_TRACE(ENIGMA_CURRENT_FUNCTION);
#endif
	ENIGMA_ASSERT_OR_RETURN(m_database, "Database was not initialized", false);
	try
	{
		auto transaction = std::make_unique<SQLite::Transaction>(*m_database);
		// Insert encryption
		{
			constexpr char* sql = "INSERT INTO Encryption(title, date_time, size, is_file) VALUES(?, DATETIME(), ?, ?)";
			ENIGMA_LOG("SQL: {0}", sql);
			auto query = std::make_unique<SQLite::Statement>(*m_database, sql);
			query->bindNoCopy(1, e->title);
			ENIGMA_INFO("size from query {0}", e->size);
			query->bind(2, e->size);
			query->bind(3, static_cast<i32>(e->is_file));
			i32 r = query->exec(); // returns # of rows effected
			ENIGMA_ASSERT_OR_THROW(r > 0, "Failed to insert encyption record");
		}
		
		// Get inserted encryption id
		const i64 last_inserted_encryption_id = m_database->getLastInsertRowid();
		
		// Insert cipher
		{
			constexpr char* sql = "INSERT INTO Cipher(data, ide) VALUES(?, ?)";
			ENIGMA_LOG("SQL: {0}", sql);
			auto query = std::make_unique<SQLite::Statement>(*m_database, sql);
			query->bindNoCopy(1, e->cipher.data.data(), static_cast<i32>(e->cipher.data.size())); // bind blob
			//query->bindNoCopy(1, e->cipher.data); // bind blob (same as bindText..)
			query->bind(2, last_inserted_encryption_id);
			i32 r = query->exec(); // returns # of rows effected
			ENIGMA_ASSERT_OR_THROW(r > 0, "Failed to insert cipher record");
		}
		
		// OK (transaction will rollback when it goes out of scope)
		transaction->commit();

		return true;
	}
	catch (const SQLite::Exception& e)
	{
		ENIGMA_ERROR(e.what());
		return false;
	}

}

// Delete Encryption record by id, returns true if successfully deleted
bool Database::DeleteEncryption(const i64 ide)
{
#ifdef ENIGMA_DEBUG
	ENIGMA_TRACE(ENIGMA_CURRENT_FUNCTION);
#endif
	ENIGMA_ASSERT_OR_RETURN(m_database, "Database was not initialized", false);
	try
	{
		auto transaction = std::make_unique<SQLite::Transaction>(*m_database);

		// Delete cipher (Optional, since ON DELETE CASCADE is enabled in Cipher.ide, cipher record will be deleted automatically)
		{
			constexpr char* sql = "DELETE FROM Cipher WHERE ide = ?";
			ENIGMA_LOG("SQL: {0}", sql);
			auto query = std::make_unique<SQLite::Statement>(*m_database, sql);
			query->bind(1, ide);
			i32 r = query->exec(); // returns # of rows effected
			ENIGMA_ASSERT_OR_THROW(r > 0, "Failed to delete cipher record");
		}

		// Delete encryption
		{
			constexpr char* sql = "DELETE FROM Encryption WHERE ide = ?";
			ENIGMA_LOG("SQL: {0}", sql);
			auto query = std::make_unique<SQLite::Statement>(*m_database, sql);
			query->bind(1, ide);
			i32 r = query->exec(); // returns # of rows effected
			ENIGMA_ASSERT_OR_THROW(r > 0, "Failed to delete encyption record");
		}

		// OK
		transaction->commit();

		return true;
	}
	catch (const SQLite::Exception& e)
	{
		ENIGMA_ERROR(e.what());
		return false;
	}
}



NS_ENIGMA_END
