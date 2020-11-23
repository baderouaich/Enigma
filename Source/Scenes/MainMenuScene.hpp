#pragma once
#include "Scene.hpp"
using namespace Enigma;


class MainMenuScene : public Enigma::Scene
{
public:	/* Constructors / Destructor */
	MainMenuScene();
	virtual ~MainMenuScene() = default;

private: /* Overrides */
	void OnCreate() override;
	[[maybe_unused]] void OnUpdate(const f32& dt) override;
	[[maybe_unused]] void OnDraw() override;
	void OnImGuiDraw() override;
	void OnEvent(Event& event) override;
	void OnDestroy() override;

private:
	void LoadImGuiFonts();

private:
	std::unordered_map<String, ImFont*> m_fonts;
};

