#pragma once
#if ENABLE
#include <Core/Core.hpp>
#include <catch2/catch_all.hpp>
#include <Database/Database.hpp>
#include <iostream>
#include "TestsData.hpp"
using namespace Enigma;
using namespace Catch;
using namespace Catch::Matchers;


std::once_flag onceFlag{};

TEST_CASE("SQLite3 Database insert encryption", "Create")
{
	std::cout << "\n======[ " << Catch::getResultCapture().getCurrentTestName() << " ]======\n";

  std::call_once(onceFlag, [](){
    Database::Initialize();
  });
	auto e1 = std::make_unique<Encryption>();
	e1->title = "My file securely encrypted";
	e1->is_file = true;
	e1->file_ext = ".txt";
	e1->cipher = {0, std::string(100, 'A'), 0};
	e1->size = 100;

	std::int64_t created_id = Database::AddEncryption(e1);
	e1->ide = created_id;
	REQUIRE(created_id >= 0);

	auto e2 = Database::GetEncryptionByID<false, false, false, false, false, false>(created_id);
	REQUIRE(*e1 == *e1);

 REQUIRE(true == Database::DeleteEncryption(e1->ide));

	std::cout << "e1: " << *e1 <<std::endl;
	std::cout << "e2: " << *e2 <<std::endl;
}
#endif