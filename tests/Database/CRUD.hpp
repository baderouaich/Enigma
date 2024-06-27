#pragma once
#include <Core/Core.hpp>
#include <catch2/catch_all.hpp>
#include <Database/Database.hpp>
#include <Tests/TestsData.hpp>

using namespace Enigma;
using namespace Catch;
using namespace Catch::Matchers;


TEST_CASE("SQLite3 Database insert encryption", "Create")
{
	std::cout << "\n======[ " << Catch::getResultCapture().getCurrentTestName() << " ]======\n";

	auto e1 = std::make_unique<Encryption>();
	e1->title = "My file securely encrypted";
	e1->is_file = true;
	e1->file_ext = ".txt";
	e1->cipher = {0, "00000", 0};
	e1->size = 100;

	i64 created_id = Database::AddEncryption(e1);
	e1->ide = created_id;
	REQUIRE(created_id >= 0);

	auto e2 = Database::GetEncryptionByID<false, false, false, false, false, false>(created_id);
	REQUIRE(*e1 == *e1);

	std::cout << "e1: " << *e1 <<std::endl;
	std::cout << "e2: " << *e2 <<std::endl;
}
