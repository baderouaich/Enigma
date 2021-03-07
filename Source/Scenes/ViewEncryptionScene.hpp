#pragma once
#include "Scene.hpp"

#include <Database/Database.hpp>

NS_ENIGMA_BEGIN

class ViewEncryptionScene : public Enigma::Scene
{
public:	/* Constructors / Destructor */
	explicit ViewEncryptionScene(const size_t encryption_id, const std::unordered_map<std::string_view, ImFont*>& fonts);
	virtual ~ViewEncryptionScene() noexcept = default;

private: /* Overrides */
	void OnCreate() override;
	[[maybe_unused]] void OnUpdate(const f32&) override;
	void OnDraw() override;
	void OnImGuiDraw() override;
	[[maybe_unused]] void OnEvent(Enigma::Event&) override;
	void OnDestroy() override;

private: /* Callbacks */
	void OnBackButtonPressed();

private: /* Initializers */


private:
	const std::unordered_map<std::string_view, ImFont*>& m_fonts; // from MainMenuScene where fonts are loaded

private:
	std::unique_ptr<Encryption> m_encryption{};
};

NS_ENIGMA_END
