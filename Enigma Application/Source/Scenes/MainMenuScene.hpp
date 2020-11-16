#pragma once
#include <Enigma.hpp>
using namespace Enigma;


#define VEC_RGBA(r, g, b, a) ImVec4(r/255.0f, g/255.0f, b/255.0f, a/255.0f)
static const ImVec4 BACKGROUND_COLOR = VEC_RGBA(46, 33, 59, 255);

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

