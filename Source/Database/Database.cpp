#include <pch.hpp>
#include "Database.hpp"

NS_ENIGMA_BEGIN

void Database::Initialize()
{
	ENIGMA_INFO("SQLite3 version {0}", SQLite::VERSION);
	try
	{
		m_database = std::make_unique<SQLite::Database>(
			Constants::Database::DATABASE_FILE_PATH,
			SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE // create if not exists
			);

		// Create Tables If Not Exists
		const auto query = std::make_unique<SQLite::Statement>(
			*m_database,
			Constants::Database::CREATE_TABLES_QUERY
			);
		const i32 status = query->exec(); // status 0 == SQLite::OK
		if (status != SQLite::OK)
		{
			ENIGMA_CRITICAL("Database failed to create tables with error code: {0} | msg: {1}", query->getErrorCode(), query->getErrorMsg());
			std::exit(EXIT_SUCCESS);
		}
	}
	catch (const SQLite::Exception& e)
	{
		ENIGMA_CRITICAL(e.what());
		std::exit(EXIT_FAILURE);
	}

}

bool Database::AddEncryption(const std::unique_ptr<Encryption>& e)
{
	try
	{
		constexpr char* sql = "INSERT INTO Encryptions(title, cipher, date_time, is_file) VALUES(?, ?, DATETIME(), ?)";
		ENIGMA_LOG("SQL: {0}", sql);

		auto query = std::make_unique<SQLite::Statement>(*m_database, sql);
		query->bindNoCopy(1, e->title);
		query->bindNoCopy(2, e->cipher);
		query->bind(3, static_cast<i32>(e->is_file));
		i32 r = query->exec(); // returns # of rows effected
		return r > 0;
	}
	catch (const SQLite::Exception& e)
	{
		ENIGMA_ERROR(e.what());
		return false;
	}

}




NS_ENIGMA_END
