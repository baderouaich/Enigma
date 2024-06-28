#include <pch.hpp>
#include "Database.hpp"
#include <Utility/FileUtils.hpp>
#include <Utility/ResourceUtils.hpp>

NS_ENIGMA_BEGIN

void Database::Initialize()
{
	ENIGMA_TRACE_CURRENT_FUNCTION();

	// Check first if we are next to ./res/ folder for CLI to work
	{
    const fs::path resDir = ::Enigma::ResourceUtils::GetResourcesDir();
		if (!fs::is_directory(resDir))
		{
			ENIGMA_CRITICAL("Couldn't find {0} folder next to Enigma executable, if you running CLI please make sure you put {1} Directory next to Enigma executable.", fs::path(resDir).string(), fs::path(resDir).string());
			std::exit(EXIT_FAILURE);
		}
#if defined(ENIGMA_DEBUG)
			ENIGMA_INFO("Resources Dir Path: {0}", fs::path(resDir).string());
#endif
	}

#if defined(ENIGMA_DEBUG)
	ENIGMA_INFO("SQLite3 version {0}", SQLite::VERSION);
#endif

	try
	{
		//Create db dir if not exists
    const fs::path dbDir = ::Enigma::ResourceUtils::GetResourcesDir() / "database";
		if (!fs::is_directory(dbDir))
		{
			ENIGMA_INFO("Creating Database Directory {0} ...", dbDir.string());
			if (!fs::create_directory(dbDir))
			{
				ENIGMA_CRITICAL("Failed to create database directory: {0}", dbDir.string());
				std::exit(EXIT_FAILURE);
			}
		}

		// Create or open db file 
		const fs::path database_file_path = Constants::Database::DATABASE_FILE_PATH;
		m_database = std::make_unique<SQLite::Database>(
			database_file_path.string(),
			SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE // create if not exists
			);
#if defined(ENIGMA_DEBUG)
		ENIGMA_INFO("Database File Path: {0}", database_file_path.string());
#endif

		// Create Tables If Not Exists
		for (const auto& create_table_sql : Constants::Database::CREATE_TABLES_SQL)
		{
			//ENIGMA_LOG("SQL: {0}", create_table_sql);
			const auto query = std::make_unique<SQLite::Statement>(
				*m_database,
				create_table_sql.data()
				);
			const std::int32_t status = query->exec(); // status 0 == SQLite::OK
			ENIGMA_ASSERT_OR_THROW(status == SQLite::OK,
				"Database failed to create tables with error code " +
				std::to_string(query->getErrorCode()) + 
				" and message: " + std::string(query->getErrorMsg()));
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
	ENIGMA_TRACE_CURRENT_FUNCTION();

	if (m_database) 
	{
		Vacuum();
	}
}

// Add Encryption to Encryptions table, returns true on success
std::int64_t Database::AddEncryption(const std::unique_ptr<Encryption>& e)
{
	ENIGMA_TRACE_CURRENT_FUNCTION();

	ENIGMA_ASSERT_OR_RETURN(m_database, "Database was not initialized", -1);
	try
	{
		auto transaction = std::make_unique<SQLite::Transaction>(*m_database);
		// Insert encryption
		{
			constexpr const char* sql = "INSERT INTO Encryption(title, date_time, size, is_file, file_ext) VALUES(?, DATETIME(), ?, ?, ?)";
#if defined(ENIGMA_DEBUG)
			ENIGMA_LOG("SQL: {0}", sql);
#endif
			const auto query = std::make_unique<SQLite::Statement>(*m_database, sql);
			query->bindNoCopy(1, e->title);
			query->bind(2, e->size);
			query->bind(3, static_cast<std::int32_t>(e->is_file));
			if (e->is_file)
				query->bind(4, e->file_ext);
			const std::int32_t r = query->exec(); // returns num rows effected
			ENIGMA_ASSERT_OR_THROW(r > 0, "Failed to insert encryption record");
		}
		
		// Get inserted encryption id
		const std::int64_t last_inserted_encryption_id = m_database->getLastInsertRowid();
		
		// Insert cipher
		{
			constexpr const char* sql = "INSERT INTO Cipher(data, ide) VALUES(?, ?)";
#if defined(ENIGMA_DEBUG)
			ENIGMA_LOG("SQL: {0}", sql);
#endif
			const auto query = std::make_unique<SQLite::Statement>(*m_database, sql);
			query->bindNoCopy(1, e->cipher.data.data(), static_cast<std::int32_t>(e->cipher.data.size())); // bind blob
			//query->bindNoCopy(1, e->cipher.data); // bind blob (same as bindText..)
			query->bind(2, last_inserted_encryption_id);
			const std::int32_t r = query->exec(); // returns # of rows effected
			ENIGMA_ASSERT_OR_THROW(r > 0, "Failed to insert cipher record");
		}
		
		// OK (if transaction was not committed, it will rollback when it goes out of scope in ~Transaction)
		transaction->commit();

		return last_inserted_encryption_id;
	}
	catch (const SQLite::Exception& e)
	{
		ENIGMA_ERROR(e.what());
		return std::int64_t(-1);
	}

}

std::unique_ptr<Cipher> Database::GetCipherByEncryptionID(const std::int64_t ide)
{
	ENIGMA_TRACE_CURRENT_FUNCTION();
	ENIGMA_ASSERT_OR_RETURN(m_database, "Database was not initialized", nullptr);

	try
	{
		// Make sql
		std::ostringstream sql{};
		sql << "SELECT * FROM Cipher WHERE ide = " << ide;
#if defined(ENIGMA_DEBUG)
			ENIGMA_LOG("SQL: {0}", sql.str());
#endif

		// Execute sql
		const auto query = std::make_unique<SQLite::Statement>(*m_database, sql.str());
		ENIGMA_ASSERT_OR_THROW(query->executeStep(), "Failed to execute step");

		// Make and return Cipher
		auto cipher = std::make_unique<Cipher>();
		cipher->idc = query->getColumn(0).getInt64(); // getColumn, index starts by 0
		cipher->data = std::move(query->getColumn(1).getString());
		cipher->ide = query->getColumn(2);

		return cipher;
	}
	catch (const SQLite::Exception& e)
	{
		ENIGMA_ERROR("{0}", e.what());
		return nullptr;
	}

}

// Delete Encryption record by id, returns true if successfully deleted
bool Database::DeleteEncryption(const std::int64_t ide)
{
	ENIGMA_TRACE_CURRENT_FUNCTION();

	ENIGMA_ASSERT_OR_RETURN(m_database, "Database was not initialized", false);
	try
	{
		auto transaction = std::make_unique<SQLite::Transaction>(*m_database);

		// Delete cipher (Optional, since ON DELETE CASCADE is enabled in Cipher.ide, cipher record will be deleted automatically)
		{
			constexpr const char* sql = "DELETE FROM Cipher WHERE ide = ?";
#if defined(ENIGMA_DEBUG)
			ENIGMA_LOG("SQL: {0}", sql);
#endif
			auto query = std::make_unique<SQLite::Statement>(*m_database, sql);
			query->bind(1, ide);
			std::int32_t r = query->exec(); // returns # of rows effected
			ENIGMA_ASSERT_OR_THROW(r > 0, "Failed to delete cipher record");
		}

		// Delete encryption
		{
			constexpr const char* sql = "DELETE FROM Encryption WHERE ide = ?";
#if defined(ENIGMA_DEBUG)
			ENIGMA_LOG("SQL: {0}", sql);
#endif
			auto query = std::make_unique<SQLite::Statement>(*m_database, sql);
			query->bind(1, ide);
			std::int32_t r = query->exec(); // returns # of rows effected
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

bool Database::DeleteAllEncryptions()
{
	ENIGMA_TRACE_CURRENT_FUNCTION();
	ENIGMA_ASSERT_OR_RETURN(m_database, "Database was not initialized", false);
	try
	{
		auto transaction = std::make_unique<SQLite::Transaction>(*m_database);

		// Delete everything from Cipher
		{
			constexpr const char* sql = "DELETE FROM Cipher";
#if defined(ENIGMA_DEBUG)
			ENIGMA_LOG("SQL: {0}", sql);
#endif
			auto query = std::make_unique<SQLite::Statement>(*m_database, sql);
			[[maybe_unused]] auto _ = query->exec();
		}

		// Delete everything from Encryption
		{
			constexpr const char* sql = "DELETE FROM Encryption";
#if defined(ENIGMA_DEBUG)
			ENIGMA_LOG("SQL: {0}", sql);
#endif
			auto query = std::make_unique<SQLite::Statement>(*m_database, sql);
			[[maybe_unused]] auto _ = query->exec();
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

std::int64_t Database::GetEncryptionsCount()
{
	ENIGMA_TRACE_CURRENT_FUNCTION();
	ENIGMA_ASSERT_OR_RETURN(m_database, "Database was not initialized", 0);

	try
	{
		// Make sql
		constexpr const char* sql = "SELECT COUNT(*) FROM Encryption";
#if defined(ENIGMA_DEBUG)
		ENIGMA_LOG("SQL: {0}", sql);
#endif
		// Execute sql
		const auto query = std::make_unique<SQLite::Statement>(*m_database, sql);
		ENIGMA_ASSERT_OR_THROW(query->executeStep(), "Failed to execute step");

		// get and return count
		return query->getColumn(0).getInt64();
	}
	catch (const SQLite::Exception& e)
	{
		ENIGMA_ERROR("{0}", e.what());
		return 0;
	}
}

void Database::Vacuum() noexcept
{
	/*ENIGMA_INFO("Vacuuming SQLite3 database to optimize disk space...");
	(void)m_database->exec("VACUUM");*/

	// Only vacuum if changes to the database were made.
	const std::int32_t total_changes = m_database->getTotalChanges();
	if (total_changes > 0)
	{
		ENIGMA_INFO("{0} database changes were made, Vacuuming database to optimize disk space...", total_changes);
		(void)m_database->exec("VACUUM");
	}
	//else
	//	ENIGMA_INFO("No database changes were made, skipping vacuum disk optimization.");
}

NS_ENIGMA_END
