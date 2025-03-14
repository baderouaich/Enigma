#include <pch.hpp>
#include "Database.hpp"
#include <Utility/FileUtils.hpp>
#include <ResourceManager/ResourceManager.hpp>

NS_ENIGMA_BEGIN

void Database::initialize() {
  ENIGMA_TRACE_CURRENT_FUNCTION();
#if defined(ENIGMA_DEBUG)
  ENIGMA_INFO("SQLite3 version {}", SQLite::VERSION);
#endif

  try {
    // Create or open db file
    m_database = std::make_unique<SQLite::Database>(
      //database_file_path.string(),
      ResourceManager::getDatabaseFilepath(),
      SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE // create if not exists
    );
#if defined(ENIGMA_DEBUG)
    ENIGMA_INFO("Database File Path: {0}", m_database->getFilename());
#endif

    // Create Tables If Not Exists
    for (const auto& create_table_sql: Constants::Database::CREATE_TABLES_SQL) {
      //ENIGMA_LOG("SQL: {0}", create_table_sql);
      const auto query = std::make_unique<SQLite::Statement>(*m_database, create_table_sql.data());
      const std::int32_t status = query->exec(); // status 0 == SQLite::OK
      ENIGMA_ASSERT_OR_THROW(status == SQLite::OK,
                             "Database failed to create tables with error code " +
                               std::to_string(query->getErrorCode()) +
                               " and message: " + std::string(query->getErrorMsg()));
    }

    // Create indexes
    for (const auto& create_index_sql: Constants::Database::CREATE_INDEXES_SQL) {
      const auto query = std::make_unique<SQLite::Statement>(*m_database, create_index_sql.data());
      const std::int32_t status = query->exec(); // status 0 == SQLite::OK
      ENIGMA_ASSERT_OR_THROW(status == SQLite::OK,
                             "Database failed to create tables with error code " +
                               std::to_string(query->getErrorCode()) +
                               " and message: " + std::string(query->getErrorMsg()));
    }
  } catch (const SQLite::Exception& e) {
    ENIGMA_CRITICAL(e.what());
    std::exit(EXIT_FAILURE);
  } catch (const std::exception& e) {
    ENIGMA_CRITICAL(e.what());
  }
}

void Database::shutdown() {
  ENIGMA_TRACE_CURRENT_FUNCTION();

  if (m_database) {
    Vacuum();
  }
}

std::int64_t Database::addEncryption(const std::unique_ptr<Encryption>& e) {
  ENIGMA_TRACE_CURRENT_FUNCTION();
  ENIGMA_ASSERT_OR_RETURN(m_database, "Database was not initialized", -1);

  try {
    // Insert encryption
    std::ostringstream sql{};
    sql << "INSERT INTO " << Encryption::TABLE_NAME << "(algo, title, date_time, size, is_file, file_ext) VALUES(?, ?, DATETIME(), ?, ?, ?)";
#if defined(ENIGMA_DEBUG)
    ENIGMA_LOG("SQL: {0}", sql.str());
#endif
    const auto query = std::make_unique<SQLite::Statement>(*m_database, sql.str());
    std::int32_t i{1};
    query->bind(i++, static_cast<std::int32_t>(e->algo));
    query->bindNoCopy(i++, e->title);
    query->bind(i++, e->size);
    query->bind(i++, static_cast<std::int32_t>(e->is_file));
    query->bind(i++, e->is_file ? e->file_ext.c_str() : nullptr);
    const std::int32_t r = query->exec(); // returns num rows effected
    ENIGMA_ASSERT_OR_THROW(r > 0, "Failed to insert encryption record");

    // Get inserted encryption id
    return m_database->getLastInsertRowid();
  } catch (const SQLite::Exception& e) {
    ENIGMA_ERROR(e.what());
  } catch (const std::exception& e) {
    ENIGMA_CRITICAL(e.what());
  }
  return std::int64_t(-1);
}

std::int64_t Database::addCipherChunk(const std::unique_ptr<CipherChunk>& cc) {
  ENIGMA_TRACE_CURRENT_FUNCTION();
  ENIGMA_ASSERT_OR_RETURN(m_database, "Database was not initialized", -1);

  try {
    // Insert encryption
    std::ostringstream sql{};
    sql << "INSERT INTO " << CipherChunk::TABLE_NAME << "(ide, offset, size, bytes) VALUES(?, ?, ?, ?)";
#if defined(ENIGMA_DEBUG)
    ENIGMA_LOG("SQL: {0}", sql.str());
#endif
    const auto query = std::make_unique<SQLite::Statement>(*m_database, sql.str());
    query->bind(1, cc->ide);
    query->bind(2, cc->offset);
    query->bind(3, cc->size);
    query->bindNoCopy(4, cc->bytes.data(), cc->bytes.size());
    const std::int32_t r = query->exec(); // returns num rows effected
    ENIGMA_ASSERT_OR_THROW(r > 0, "Failed to insert encryption record");

    // Get inserted encryption id
    return m_database->getLastInsertRowid();
  } catch (const SQLite::Exception& e) {
    ENIGMA_ERROR(e.what());
  } catch (const std::exception& e) {
    ENIGMA_CRITICAL(e.what());
  }
  return std::int64_t(-1);
}

std::unique_ptr<Encryption> Database::getEncryption(const std::int64_t ide) {
  ENIGMA_TRACE_CURRENT_FUNCTION();
  ENIGMA_ASSERT_OR_RETURN(m_database, "Database was not initialized", nullptr);

  try {
    std::ostringstream sql{};
    sql << "SELECT ide, algo, title, date_time, size, is_file, file_ext FROM " << Encryption::TABLE_NAME << " WHERE ide = ?;";
#if defined(ENIGMA_DEBUG)
    ENIGMA_LOG("SQL: {0}", sql.str());
#endif

    const auto query = std::make_unique<SQLite::Statement>(*m_database, sql.str());
    query->bind(1, ide);
    if (query->executeStep()) {
      auto e = std::make_unique<Encryption>();
      std::int32_t i{0};
      e->ide = query->getColumn(i++).getInt64();
      e->algo = static_cast<Algorithm::Type>(query->getColumn(i++).getUInt());
      e->title = query->getColumn(i++).getString();
      e->date_time = query->getColumn(i++).getString();
      e->size = query->getColumn(i++).getInt64();
      e->is_file = !!query->getColumn(i++).getUInt();
      e->file_ext = query->getColumn(i++).getString();
      return e;
    }
  } catch (const SQLite::Exception& e) {
    ENIGMA_ERROR("{0}", e.what());
    ;
  }
  return nullptr;
}

std::unique_ptr<CipherChunk> Database::getCipherChunk(const std::int64_t ide) {
  ENIGMA_TRACE_CURRENT_FUNCTION();
  ENIGMA_ASSERT_OR_RETURN(m_database, "Database was not initialized", nullptr);

  try {
    std::ostringstream sql{};
    sql << "SELECT idc, ide, offset, size, bytes FROM " << CipherChunk::TABLE_NAME << " WHERE ide = ?;";
#if defined(ENIGMA_DEBUG)
    ENIGMA_LOG("SQL: {0}", sql.str());
#endif

    const auto query = std::make_unique<SQLite::Statement>(*m_database, sql.str());
    query->bind(1, ide);
    if (query->executeStep()) {
      auto cc = std::make_unique<CipherChunk>();
      std::int32_t i{0};
      cc->idc = query->getColumn(i++).getInt64();
      cc->ide = query->getColumn(i++).getInt64();
      cc->offset = query->getColumn(i++).getInt64();
      cc->size = query->getColumn(i++).getInt64();
      const void* blob = query->getColumn(i++).getBlob();
      cc->bytes.resize(cc->size, '\000');
      std::memcpy(cc->bytes.data(), blob, cc->size);
      return cc;
    }
  } catch (const SQLite::Exception& e) {
    ENIGMA_ERROR("{0}", e.what());
    ;
  }
  return nullptr;
}

std::vector<std::unique_ptr<Encryption>> Database::getAllEncryptions(const Database::OrderBy order_by, const Database::Order order) {
  ENIGMA_TRACE_CURRENT_FUNCTION();
  ENIGMA_ASSERT_OR_RETURN(m_database, "Database was not initialized", {});

  std::vector<std::unique_ptr<Encryption>> encryptions;
  try {
    std::ostringstream sql{};
    sql << "SELECT ide, algo, title, date_time, size, is_file, file_ext FROM " << Encryption::TABLE_NAME << " ORDER BY " << order_by << ' ' << order;
#if defined(ENIGMA_DEBUG)
    ENIGMA_LOG("SQL: {0}", sql.str());
#endif

    const auto query = std::make_unique<SQLite::Statement>(*m_database, sql.str());

    while (query->executeStep()) {
      auto e = std::make_unique<Encryption>();
      std::int32_t i{0};
      e->ide = query->getColumn(i++).getInt64();
      e->algo = static_cast<Algorithm::Type>(query->getColumn(i++).getUInt());
      e->title = query->getColumn(i++).getString();
      e->date_time = query->getColumn(i++).getString();
      e->size = query->getColumn(i++).getInt64();
      e->is_file = !!query->getColumn(i++).getUInt();
      e->file_ext = query->getColumn(i++).getString();
      encryptions.emplace_back(std::move(e));
    }
  } catch (const SQLite::Exception& e) {
    ENIGMA_ERROR("{0}", e.what());
  }

  return encryptions;
}

bool Database::deleteEncryption(const std::int64_t ide) {
  ENIGMA_TRACE_CURRENT_FUNCTION();
  ENIGMA_ASSERT_OR_RETURN(m_database, "Database was not initialized", false);

  try {
    std::ostringstream sql{};
    sql << "DELETE FROM " << Encryption::TABLE_NAME << " WHERE ide = ?;";
#if defined(ENIGMA_DEBUG)
    ENIGMA_LOG("SQL: {0}", sql.str());
#endif

    const auto query = std::make_unique<SQLite::Statement>(*m_database, sql.str());
    query->bind(1, ide);
    std::int32_t r = query->exec(); // returns # of rows effected
    ENIGMA_ASSERT_OR_THROW(r > 0, "Failed to delete encryption record");
    return true;
  } catch (const SQLite::Exception& e) {
    ENIGMA_ERROR(e.what());
  } catch (const std::exception& e) {
    ENIGMA_CRITICAL(e.what());
  }
  return false;
}

void Database::getCipherChunks(const std::int64_t ide, const std::function<bool(std::unique_ptr<CipherChunk>&&)>& callback) {
  ENIGMA_TRACE_CURRENT_FUNCTION();
  ENIGMA_ASSERT(m_database, "Database was not initialized");

  try {
    std::ostringstream sql{};
    sql << "SELECT idc, ide, offset, size, bytes FROM " << CipherChunk::TABLE_NAME << " WHERE ide = ? ORDER BY offset ASC"; // NOTE: order by offset 0 -> MAX to get chunks respectively in order
#if defined(ENIGMA_DEBUG)
    ENIGMA_LOG("SQL: {0}", sql.str());
#endif

    const auto query = std::make_unique<SQLite::Statement>(*m_database, sql.str());
    query->bind(1, ide);
    while (query->executeStep()) {
      auto cc = std::make_unique<CipherChunk>();
      std::int32_t i{0};
      cc->idc = query->getColumn(i++).getInt64();
      cc->ide = query->getColumn(i++).getInt64();
      cc->offset = query->getColumn(i++).getInt64();
      cc->size = query->getColumn(i++).getInt64();
      const void* blob = query->getColumn(i++).getBlob();
      //ENIGMA_ASSERT_OR_THROW(cc->size == query->getColumn(i).size(), "");
      cc->bytes.resize(cc->size, '\000');
      std::memcpy(cc->bytes.data(), blob, cc->size);

      if (!callback(std::move(cc)))
        break;
    }
  } catch (const SQLite::Exception& e) {
    ENIGMA_ERROR("{0}", e.what());
  }
}
bool Database::deleteAllEncryptions() {
  ENIGMA_TRACE_CURRENT_FUNCTION();
  ENIGMA_ASSERT_OR_RETURN(m_database, "Database was not initialized", false);

  try {
    std::ostringstream sql{};
    sql << "DELETE FROM " << Encryption::TABLE_NAME;
#if defined(ENIGMA_DEBUG)
    ENIGMA_LOG("SQL: {0}", sql.str());
#endif

    const auto query = std::make_unique<SQLite::Statement>(*m_database, sql.str());
    std::int32_t r = query->exec(); // returns # of rows effected
    ENIGMA_ASSERT_OR_THROW(r > 0, "Failed to delete encryption records");
    return true;
  } catch (const SQLite::Exception& e) {
    ENIGMA_ERROR(e.what());
  } catch (const std::exception& e) {
    ENIGMA_CRITICAL(e.what());
  }
  return false;
}

std::vector<std::unique_ptr<Encryption>> Database::searchEncryptionsByTitle(const std::string& qtitle, Database::OrderBy order_by, Database::Order order) {
  ENIGMA_TRACE_CURRENT_FUNCTION();
  ENIGMA_ASSERT_OR_RETURN(m_database, "Database was not initialized", {});

  std::vector<std::unique_ptr<Encryption>> encryptions;
  try {
    std::ostringstream sql{};
    sql << "SELECT ide, algo, title, date_time, size, is_file, file_ext FROM " << Encryption::TABLE_NAME << " WHERE title LIKE '%'||?||'%' ORDER BY " << order_by << ' ' << order;
#if defined(ENIGMA_DEBUG)
    ENIGMA_LOG("SQL: {0}", sql.str());
#endif

    const auto query = std::make_unique<SQLite::Statement>(*m_database, sql.str());
    query->bind(1, qtitle);
    while (query->executeStep()) {
      auto e = std::make_unique<Encryption>();
      std::int32_t i{0};
      e->ide = query->getColumn(i++).getInt64();
      e->algo = static_cast<Algorithm::Type>(query->getColumn(i++).getUInt());
      e->title = query->getColumn(i++).getString();
      e->date_time = query->getColumn(i++).getString();
      e->size = query->getColumn(i++).getInt64();
      e->is_file = !!query->getColumn(i++).getUInt();
      e->file_ext = query->getColumn(i++).getString();
      encryptions.emplace_back(std::move(e));
    }
  } catch (const SQLite::Exception& e) {
    ENIGMA_ERROR("{0}", e.what());
  }
  return encryptions;
}

void Database::Vacuum() noexcept {
  // Only vacuum if changes to the database were made.
  const std::int32_t total_changes = m_database->getTotalChanges();
  if (total_changes > 0) {
    ENIGMA_INFO("{0} database changes were made, Vacuuming database to optimize disk space...", total_changes);
    (void) m_database->exec("VACUUM");
  }
}

void Database::Export(const fs::path& filename) {
  m_database->backup(filename.string().c_str(), SQLite::Database::BackupType::Save);
}
void Database::Import(const fs::path& filename) {
  m_database->backup(filename.string().c_str(), SQLite::Database::BackupType::Load);
}

NS_ENIGMA_END
