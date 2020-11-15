#pragma once
#include <Enigma.hpp>
using namespace Enigma;



static const ImVec4 BACKGROUND_COLOR(49 / 255.0f, 34 / 255.0f, 56 / 255.0f, 1.0f);

class MainMenuScene : public Enigma::Scene
{
public:	/* Constructors / Destructor */
	MainMenuScene();
	virtual ~MainMenuScene() = default;

private: /* Overrides */
	void OnCreate() override;
	void OnUpdate(const f32& dt) override;
	void OnDraw() override;
	void OnImGuiDraw() override;
	void OnEvent(Event& event) override;
	void OnDestroy() override;

private:


};

