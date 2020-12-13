#pragma once
#include "Scene.hpp"
using namespace Enigma;

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
	
private: /* Callbacks */
	// Buttons
	void OnEncryptFileButtonPressed();
	void OnDecryptFileButtonPressed();
	void OnEncryptTextButtonPressed();
	void OnDecryptTextButtonPressed();
	// Menu
	void OnAboutMenuButtonPressed();

private: /* Initializers */
	void LoadImGuiFonts();

private:
	std::unordered_map<std::string_view, ImFont*> m_fonts;
};
