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

		WindowSettings window_settings;
		window_settings.title = config.Get<String>("window", "title", "Enigma");
		window_settings.width = config.Get<decltype(WindowSettings::width)>("window", "width", 800);
		window_settings.height = config.Get<decltype(WindowSettings::height)>("window", "height", 600);
		window_settings.minimum_width = config.Get<decltype(WindowSettings::minimum_width)>("window", "minimum_width", -1);
		window_settings.minimum_height = config.Get<decltype(WindowSettings::minimum_height)>("window", "minimum_height", -1);
		window_settings.maximum_width = config.Get<decltype(WindowSettings::maximum_width)>("window", "maximum_width", -1);
		window_settings.maximum_height = config.Get<decltype(WindowSettings::maximum_height)>("window", "maximum_height", -1);
		window_settings.refresh_rate = config.Get<decltype(WindowSettings::refresh_rate)>("window", "refresh_rate", -1);
		window_settings.samples = config.Get<decltype(WindowSettings::samples)>("window", "samples", 4);
		window_settings.is_resizable = config.Get<bool>("window", "resizable", true);
		window_settings.is_vsync = config.Get<bool>("window", "vsync", true);
		window_settings.is_fullscreen = config.Get<bool>("window", "fullscreen", false);
		window_settings.is_decorated = config.Get<bool>("window", "decorated", true);
		window_settings.is_focused = config.Get<bool>("window", "focused", true);
		window_settings.is_maximized = config.Get<bool>("window", "maximized", false);
		window_settings.is_floating = config.Get<bool>("window", "floating", false);
		window_settings.is_visible = config.Get<bool>("window", "visible", true);
		window_settings.is_auto_iconify = config.Get<bool>("window", "auto_iconify", true);
		window_settings.is_show_fps = config.Get<bool>("window", "show_fps", true);
		window_settings.is_show_ram_usage = config.Get<bool>("window", "show_ram_usage", true);
		window_settings.is_show_cpu_usage = config.Get<bool>("window", "show_cpu_usage", true);

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
