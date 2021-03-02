#pragma once
#ifndef ENIGMA_DATABASE_H
#define ENIGMA_DATABASE_H

#include <Core/Core.hpp>
#include <Logger/Logger.hpp>
#include "Models/Encryption.hpp"

#include <SQLiteCpp/SQLiteCpp.h>

#include <memory>
#include <vector>

NS_ENIGMA_BEGIN
/*
* Enigma SQLite database to store and retrieve encryption records
* User can export and import Database file to clouds or drives
* Database file will store Encrypted Text and Encrypted Files ciphers and other info
*/
class ENIGMA_API Database final
{
public:
	static void Initialize();

public: // Encryption Operations

	// Add Encryption to Encryptions table
	static bool AddEncryption(const std::unique_ptr<Encryption>& e);

	// Get all Encryptions with desired columns for optimization
	template<const bool title, const bool cipher, const bool date_time, const bool is_file>
	inline static std::vector<std::unique_ptr<Encryption>> GetAllEncryptions(const String& order_by)
	{
		std::vector<std::unique_ptr<Encryption>> encryptions{};
		try
		{
			// Construct SQL
			std::ostringstream sql{};
			sql << "SELECT id";
			if constexpr (title) sql << ", title";
			if constexpr (cipher) sql << ", cipher";
			if constexpr (date_time) sql << ", date_time";
			if constexpr (is_file) sql << ", is_file";
			sql << " FROM Encryptions ORDER BY " << order_by;
			ENIGMA_LOG("SQL: {0}", sql.str());

			const auto query = std::make_unique<SQLite::Statement>(*m_database, sql.str());

			// Loop to execute the query step by step, to get rows of result
			while (query->executeStep())
			{
				auto e = std::make_unique<Encryption>();
				
				e->id = static_cast<decltype(Encryption::id)>(query->getColumn(0).getInt64()); // either use index or column name
				if constexpr (title) e->title = query->getColumn("title").getString();
				if constexpr (cipher) e->cipher = query->getColumn("cipher").getString();
				if constexpr (date_time) e->date_time = query->getColumn("date_time").getString();
				if constexpr (is_file) e->is_file =static_cast<decltype(Encryption::is_file)>(query->getColumn("is_file").getInt());

				encryptions.emplace_back(std::move(e));
			}
		}
		catch (const SQLite::Exception& e)
		{
			ENIGMA_ERROR(e.what());
		}
		return encryptions;
	}


	// Get an Encyrption by id with desired columns for optimization
	template<const bool title, const bool cipher, const bool date_time, const bool is_file>
	inline static std::unique_ptr<Encryption> GetEncryptionByID(const size_t id)
	{
		try
		{
			// Construct SQL
			std::ostringstream sql{};
			sql << "SELECT id";
			if constexpr (title) sql << ", title";
			if constexpr (cipher) sql << ", cipher";
			if constexpr (date_time) sql << ", date_time";
			if constexpr (is_file) sql << ", is_file";
			sql << " FROM Encryptions where id = " << id;
			ENIGMA_LOG("SQL: {0}", sql.str());

			const auto query = std::make_unique<SQLite::Statement>(*m_database, sql.str());
			auto e = std::make_unique<Encryption>();

			if (query->executeStep())
			{
				e->id = static_cast<decltype(Encryption::id)>(query->getColumn(0).getInt64());
				if constexpr (title) e->title = query->getColumn("title").getString();
				if constexpr (cipher) e->cipher = query->getColumn("cipher").getString();
				if constexpr (date_time) e->date_time = query->getColumn("date_time").getString();
				if constexpr (is_file) e->is_file = static_cast<decltype(Encryption::is_file)>(query->getColumn("is_file").getInt());
			}

			return e;
		}
		catch (const SQLite::Exception& e)
		{
			ENIGMA_ERROR(e.what());
			return nullptr;
		}
	}

private:
	inline static std::unique_ptr<SQLite::Database> m_database{ nullptr };

};

NS_ENIGMA_END
/*
Notes: 
for bind, use index starts by 1
for getColumn, use index starts by 0
*/
#endif // !ENIGMA_DATABASE_H

