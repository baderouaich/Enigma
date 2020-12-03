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

		//Set Window Icon & Cursor
		const auto& window = __super::GetWindow();
		window->SetCursor(Enigma::CursorMode::Arrow);
		window->SetIcon(Enigma::Constants::Resources::Textures::ENIGMA_LOGO_PNG_PATH);
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
	//Construct Window
	Enigma::WindowSettings window_settings;
	{
		window_settings.title = "Enigma";
		window_settings.width = 640;
		window_settings.height = 480;
		//window_settings.minimum_width = window_settings.width - 100;
		//window_settings.minimum_height = window_settings.height - 100;
		//window_settings.maximum_width = 800;
		//window_settings.maximum_height = 600;
		//window_settings.is_resizable = true;
		window_settings.is_resizable = false;
#if ENIGMA_DEBUG
		window_settings.is_vsync = false;
		window_settings.is_show_fps = true;
#else
		window_settings.is_vsync = true;
		window_settings.is_show_fps = false;
#endif
	}
	return Enigma::MakeUnique<EnigmaApplication>(std::move(window_settings));
}
