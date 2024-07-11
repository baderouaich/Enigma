#pragma once
#ifndef ENIGMA_TESTS_ALL
#define ENIGMA_TESTS_ALL

// Encryption Algorithms Tests
#include "Crypto/AESTests.hpp"
#include "Crypto/ChaCha20Poly1305Tests.hpp"
#include "Crypto/TripleDESTests.hpp"
#include "Crypto/TwofishTests.hpp"
#include "Crypto/IDEATests.hpp"
#include "Crypto/BlowfishTests.hpp"
//#include "Crypto/RSATests.hpp"

// Utility Tests
#include "Utils/Base64Tests.hpp"
#include "Utils/FileUtilsTests.hpp"
#include "Utils/HashTests.hpp"

// Database Tests
//#include "DB/CRUD.hpp"

// Endianness
#include "Endianness/EndiannessTests.hpp"


#endif // !ENIGMA_TESTS_ALL
