#include <pch.hpp>
#include "Enigma.hpp"
#include "Scenes/MainMenuScene.hpp"

class EnigmaApplication : public Enigma::Application
{
public:
	explicit EnigmaApplication(const Enigma::WindowSettings& window_settings)
		:
		Enigma::Application(window_settings)
	{
		ENIGMA_TRACE(ENIGMA_CURRENT_FUNCTION);

		//Set Window Icon & Cursor
		const auto& window = GetWindow();
		window->SetCursor(Enigma::CursorMode::Arrow);
		window->SetIcon(Enigma::Constants::Resources::Textures::ENIGMA_LOGO_PNG_PATH);
		//window->SetCursor("Resources/Textures/Cursor.png", 0, 0);

		//Push Main Menu scene
		this->PushScene(std::make_unique<MainMenuScene>());
	}


	virtual ~EnigmaApplication() = default;
};


/*
*	Entry Point
*/
extern std::unique_ptr<Enigma::Application> Enigma::CreateApplication()
{
	//Construct Window
	Enigma::WindowSettings window_settings;
	{
		window_settings.title = "Enigma";
		window_settings.width = 800;
		window_settings.height = 600;
		//window_settings.minimum_width = window_settings.width - 100;
		//window_settings.minimum_height = window_settings.height - 100;
		//window_settings.maximum_width = 800;
		//window_settings.maximum_height = 600;
		//window_settings.is_resizable = true;
		window_settings.is_vsync = false;
		window_settings.is_resizable = true;
		window_settings.is_show_fps = true;
	}
	return std::make_unique<EnigmaApplication>(std::move(window_settings));
}
