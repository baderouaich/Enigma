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
	[[maybe_unused]] void OnUpdate(const f32 & dt) override;
	void OnDraw() override;
	void OnImGuiDraw() override;
	void OnEvent(Enigma::Event & event) override;
	void OnDestroy() override;

private: /* Callbacks */
	void OnBackButtonPressed();
	void OnViewEncryptionButtonPressed(const size_t id);
	void OnDeleteEncryptionButtonPressed(const size_t id);

private: /* Initializers */
	// Get all Encryptions from database
	void GetAllEncryptions();

private:
	const std::unordered_map<std::string_view, ImFont*>& m_fonts; // from MainMenuScene where fonts are loaded

private:
	std::vector<std::unique_ptr<Encryption>> m_encryptions;
	
	enum class OrderBy : byte
	{
		ID,
		Title,
		DateTime
	} m_order_by = OrderBy::ID;

	enum class Order : byte
	{
		Ascending,
		Descending,
	} m_order = Order::Descending;
};

NS_ENIGMA_END
