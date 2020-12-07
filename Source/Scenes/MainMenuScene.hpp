#pragma once
#include "Scene.hpp"
USING_NS_ENIGMA

class MainMenuScene : public Enigma::Scene
{
public:	/* Constructors / Destructor */
	MainMenuScene() noexcept;
	virtual ~MainMenuScene() = default;

private: /* Overrides */
	void OnCreate() override;
	[[maybe_unused]] void OnUpdate(const f32& dt) override;
	void OnDraw() override;
	void OnImGuiDraw() override;
	void OnEvent(Enigma::Event& event) override;
	void OnDestroy() override;

private: /* Initializers */
	void LoadImGuiFonts();

private: /* Dialogs */
	void ShowAboutDialog();

private:
	std::unordered_map<Enigma::String, ImFont*> m_fonts;
};

