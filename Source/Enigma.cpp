#include "pch.hpp"
#include "Enigma.hpp"

#include "Scenes/MainMenuScene.hpp"

class AppDelegate : public Enigma::Application
{
public:
	AppDelegate(const Enigma::WindowSettings& window_settings)
		:
		Enigma::Application(window_settings)
	{
		LOG(ENIGMA_CURRENT_FUNCTION);

		//Window
		const auto& window = __super::GetWindow();
		window->SetCursor(Enigma::CursorMode::Arrow);
		//window->SetIcon("Resources/Textures/EnigmaLogo.png");
		//window->SetCursor("Resources/Textures/Cursor.png", 0, 0);


		//Push Main Menu scene
		__super::PushScene(new MainMenuScene());
	}


	virtual ~AppDelegate() = default;
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
	window_settings.is_vsync = true;
#if ENIGMA_DEBUG
	window_settings.is_show_fps = true;
#endif
	window_settings.is_resizable = false;
	window_settings.gl_major_version = 4; // Todo: maybe 3.3 is mostly supported? 
	window_settings.gl_minor_version = 3;

	return Enigma::MakeUnique<AppDelegate>(std::move(window_settings));
}