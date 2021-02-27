#pragma once
#ifndef ENIGMA_DATABASE_H
#define ENIGMA_DATABASE_H

#include <Core/Core.hpp>

#include <SQLiteCpp/SQLiteCpp.h>

NS_ENIGMA_BEGIN
/*
* Enigma SQLite database to store and retrieve encryption records
* User can export and import Database file  to clouds or drives
* Database file will store Encrypted Text and Encrypted Files
* Database file may be large in size since it will can hold large amounts of data
*/

// Constants::Database::ENIGMA_DATABASE_FILE_PATH = "./Resources/Database/Enigma.db";
class ENIGMA_API Database final
{
public:
	static void Initialize()
	{

	}

	/*Database()
	{

	}
	~Database()
	{

	}*/

private:
	inline static SQLite::Database* m_database{ nullptr };

	static constexpr const char* CREATE_TABLES_QUERY =
R"(

)";

};
NS_ENIGMA_END

#endif // !ENIGMA_DATABASE_H
