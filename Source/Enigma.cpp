#include <pch.hpp>
#include "Enigma.hpp"
#include "Scenes/MainMenuScene.hpp"

NS_ENIGMA_BEGIN

class EnigmaApplication : public Application
{
public:
	explicit EnigmaApplication(const WindowSettings& window_settings)
		:
		Application(window_settings)
	{
		ENIGMA_TRACE(ENIGMA_CURRENT_FUNCTION);

		// Set Window runtime icon
		const auto& window = GetWindow();
		//window->SetCursor(CursorMode::Arrow);
		window->SetIcon(Constants::Resources::Textures::ENIGMA_LOGO_PNG_PATH);

		// Push Main Menu scene as an entry point
		this->PushScene(std::make_unique<MainMenuScene>());
	}


	virtual ~EnigmaApplication() = default;
};


/*
*	Entry Point
*/
extern std::unique_ptr<Application> CreateApplication()
{
	//Construct Window by Config file
	try
	{
		ENIGMA_TRACE("Loading window config from {0}...", Constants::Config::ENIGMA_CONFIG_PATH);
		
		Config config(Constants::Config::ENIGMA_CONFIG_PATH);
		WindowSettings window_settings = WindowSettings::FromConfig(config);
		
		ENIGMA_LOG("Window Settings: {0}", window_settings.toString());

		return std::make_unique<EnigmaApplication>(std::move(window_settings));
	}
	catch (const std::exception& e)
	{
		ENIGMA_CRITICAL(e.what());
		std::exit(EXIT_FAILURE);
	}

#if 0
	WindowSettings window_settings;
	window_settings.title = "Enigma";
	window_settings.width = 800;
	window_settings.height = 600;
	window_settings.is_resizable = true;
	window_settings.is_vsync = true;
	window_settings.is_show_fps = true;
	window_settings.is_show_ram_usage = true;
	window_settings.is_show_cpu_usage = true;
	return std::make_unique<EnigmaApplication>(std::move(window_settings));
#endif

}


NS_ENIGMA_END
