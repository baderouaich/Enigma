#include "pch.hpp"
#include "Enigma.hpp"

#include "Scenes/MainMenuScene.hpp"
#include "glad/glad.h"


class EnigmaApplication : public Enigma::Application
{
public:
	explicit EnigmaApplication(const Enigma::WindowSettings& window_settings)
		:
		Enigma::Application(window_settings)
	{
		LOG(ENIGMA_CURRENT_FUNCTION);
		
		//Window
		const auto& window = __super::GetWindow();
		window->SetCursor(Enigma::CursorMode::Arrow);
		window->SetIcon("Resources/Branding/EnigmaLogo_200x200.png");
		//window->SetCursor("Resources/Textures/Cursor.png", 0, 0);


		//Push Main Menu scene
		__super::PushScene(new MainMenuScene());
	}


	virtual ~EnigmaApplication() = default;
};


/*
*	Entry Point
*/
extern Enigma::UniquePtr<Enigma::Application> Enigma::CreateApplication()
{
	LOG(ENIGMA_CURRENT_FUNCTION);

	//Construct Window
	Enigma::WindowSettings window_settings;
	window_settings.title = "Enigma";
	window_settings.width = 640;
	window_settings.height = 480;
	//window_settings.minimum_width = 1280 - 100;
	//window_settings.minimum_height = 720 - 100;
	//window_settings.maximum_width = 1280 + 100;
	//window_settings.maximum_height = 720 + 100;
	window_settings.is_vsync = !ENIGMA_DEBUG;
	window_settings.is_show_fps = ENIGMA_DEBUG;
	window_settings.is_resizable = false;

	return Enigma::MakeUnique<EnigmaApplication>(std::move(window_settings));
}