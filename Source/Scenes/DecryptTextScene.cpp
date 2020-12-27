#include "pch.hpp"
#include "DecryptTextScene.hpp"

DecryptTextScene::DecryptTextScene(const std::unordered_map<std::string_view, ImFont*>& fonts)
	:
	Enigma::Scene(),
	m_fonts(fonts)
{
}

void DecryptTextScene::OnCreate()
{
	ENIGMA_TRACE(ENIGMA_CURRENT_FUNCTION);

	// Set background clear color
	glAssert(glClearColor(
		Constants::Colors::BACKGROUND_COLOR.x,
		Constants::Colors::BACKGROUND_COLOR.y,
		Constants::Colors::BACKGROUND_COLOR.z,
		Constants::Colors::BACKGROUND_COLOR.w
	));
}

void DecryptTextScene::OnUpdate(const f32& dt)
{}

void DecryptTextScene::OnDraw()
{
	// Clear GL buffers
	glAssert(glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}

void DecryptTextScene::OnImGuiDraw()
{

}


void DecryptTextScene::OnEvent(Event& event)
{
}

void DecryptTextScene::OnDestroy()
{
	ENIGMA_TRACE(ENIGMA_CURRENT_FUNCTION);
}
