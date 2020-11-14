#include "pch.hpp"	// Precompiled Headers
#include <Enigma.hpp> // Pragmatic Game Engine
using namespace Enigma;

#include "Scenes/MainScene.hpp"


class AppDelegate : public Enigma::Application
{
public:
	AppDelegate(const WindowSettings& window_settings)
		:
	Enigma::Application(window_settings)
	{	
		LOG(ENIGMA_CURRENT_FUNCTION);

		//Window
		{
			const auto& window = __super::GetWindow();
			//window->SetIcon("Resources/Textures/EnigmaLogo.png");
			window->SetCursor(Enigma::CursorMode::Arrow);
			//window->SetCursor("Resources/Textures/Cursor.png", 0, 0);
		}
		

		//Push Main scene
		__super::PushScene(new MainScene());
	}

	virtual ~AppDelegate()
	{
		LOG(ENIGMA_CURRENT_FUNCTION);

	}
};


/*
*	Entry Point
*/
extern Enigma::UniquePtr<Enigma::Application> Enigma::CreateApplication()
{ 
	//Construct Window
	Enigma::WindowSettings window_settings;
	window_settings.title = "Enigma Application";
	window_settings.width = 1280;
	window_settings.height = 720;
	window_settings.minimum_width = 1280 - 100;
	window_settings.minimum_height = 720 - 100;
	window_settings.maximum_width = 1280 + 100;
	window_settings.maximum_height = 720 + 100;
	window_settings.is_vsync = false;
	window_settings.is_resizable = true;
	window_settings.gl_major_version = 4;
	window_settings.gl_minor_version = 3;

	return Enigma::MakeUnique<AppDelegate>(std::move(window_settings));
}


