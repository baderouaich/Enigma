#pragma once
#include "Scene.hpp"

#include <Database/Database.hpp>

NS_ENIGMA_BEGIN

class MyEncryptionsScene : public Enigma::Scene
{
public:	/* Constructors / Destructor */
	explicit MyEncryptionsScene(const std::unordered_map<std::string_view, ImFont*>& fonts);
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

private: /* Initializers */
	// Get all Encryptions from database
	void GetAllEncryptions();

private:
	const std::unordered_map<std::string_view, ImFont*>& m_fonts; // from MainMenuScene where fonts are loaded

private:
	std::vector<std::unique_ptr<Encryption>> m_encryptions;

	Database::OrderBy m_order_by = Database::OrderBy::ID;
	Database::Order m_order = Database::Order::Descending;

};

NS_ENIGMA_END
