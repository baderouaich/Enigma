#pragma once
#include "Scene.hpp"

#include <Database/Database.hpp>

NS_ENIGMA_BEGIN

class MyEncryptionsScene : public Enigma::Scene
{
public:	/* Constructors / Destructor */
	explicit MyEncryptionsScene();
	virtual ~MyEncryptionsScene() noexcept = default;

private: /* Overrides */
	void OnCreate() override;
	[[maybe_unused]] void OnUpdate(const f32&) override;
	void OnDraw() override;
	void OnImGuiDraw() override;
	[[maybe_unused]] void OnEvent(Event&) override;
	void OnDestroy() override;

private: /* Callbacks */
	void OnBackButtonPressed();
	void OnViewEncryptionButtonPressed(const i64 ide);
	// returns true if item deleted successfully to notify draw loop that vector range changed
	bool OnDeleteEncryptionButtonPressed(const i64 ide);
	void OnSearchEncryptionsByTitle();
	void OnDeleteAllEncryptions();
	//void OnExportAllEncryptionsJSON();

private: /* Initializers */
	// Get all Encryptions from database
	void GetAllEncryptions();
	/** Reformats sqlite3 date format from "2021-12-14 20:40:24" to "Dec 14 2021 20:40:24 PM" */
	void ReformatEncryptionsDateTime();

private:
	std::vector<std::unique_ptr<Encryption>> m_encryptions{}; // encryption records from database
	Database::OrderBy m_order_by = Database::OrderBy::ID; // order by column
	Database::Order m_order = Database::Order::Descending; // order desc/asc
	
	
	String m_query{}; // title search query
	bool m_isSearching{ false };
};

NS_ENIGMA_END
