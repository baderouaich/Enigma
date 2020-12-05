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

private:
	void LoadImGuiFonts();

private:
	void ShowAboutDialog();

private:
	std::unordered_map<Enigma::String, ImFont*> m_fonts;
	//std::unordered_map<String, std::unordered_map<unsigned short, ImFont*> m_fonts; // { "font name", {font_size, font} }
};

