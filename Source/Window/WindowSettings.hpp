#pragma once
#ifndef ENIGMA_WINDOW_SETTINGS_H
#define ENIGMA_WINDOW_SETTINGS_H

#include <Core/Core.hpp>
#include <Config/Config.hpp>
#include <GLFW/glfw3.h>		//GLFW_DONT_CARE

NS_ENIGMA_BEGIN

struct ENIGMA_API WindowSettings
{
	/*
	*	Title of the window (Displayed in the title bar)
	*/
	String title = "Enigma";

	/*
	*	Width in pixels of the window
	*/
	i32 width = 640;

	/*
	*	Height in pixels of the window
	*/
	i32 height = 480;

	/*
	*	Minimum width of the window.
	*/
	i32 minimum_width = GLFW_DONT_CARE;

	/*
	*	Minimum height of the window.
	*/
	i32 minimum_height = GLFW_DONT_CARE;

	/*
	*	Maximum width of the window.
	*/
	i32 maximum_width = GLFW_DONT_CARE;

	/*
	*	Maximum height of the window.
	*/
	i32 maximum_height = GLFW_DONT_CARE;

	/*
	*	Window Maximum Frames Per Second.
	*/
	//ui32 maximum_fps = 60;

	/*
	*	 Specifies the desired refresh rate for full screen windows. If set to -1, the highest
	*	 available refresh rate will be used. This hint is ignored for windowed mode windows.
	*/
	i32 refresh_rate = GLFW_DONT_CARE;

	/*
	*	 Defines the number of samples to use (For anti-aliasing)
	*/
	ui32 samples = 4;


	/*
	*	Vertical sync swap interval
	* 	
	*	Interval 0: unlimited FPS
	*	Interval 1: 60 FPS
	*	Interval 2: 30 FPS
	*/
	i32 swap_interval = 2;


	/*
	*	Specifies if the window is by default in fullscreen or windowed mode
	*/
	bool is_fullscreen = false;

	/*
	*	 Specifies whether the windowed mode window will have window decorations such as a border, a close widget, etc.
	*	 An undecorated window may still allow the user to generate close events on some platforms. This hint is ignored
	*	 for full screen windows.
	*/
	bool is_decorated = true;

	/*
	*	Specifies whether the windowed mode window will be resizable by the user. The window will still be resizable using
	*	the "SetSize(i32, i32)" method of the "Window" class. This hint is ignored for full screen windows
	*/
	bool is_resizable = true;

	/*
	*	 Specifies whether the windowed mode window will be given input focus when created. This hint is ignored for
	*	 full screen and initially hidden windows.
	*/
	bool is_focused = true;

	/*
	*	Specifies whether the windowed mode window will be maximized when created. This hint is ignored for full screen windows.
	*/
	bool is_maximized = false;

	/*
	*	 Specifies whether the windowed mode window will be floating above other regular windows, also called topmost or always-on-top.
	*	 This is intended primarily for debugging purposes and cannot be used to implement proper full screen windows. This hint is
	*	 ignored for full screen windows.
	*/
	bool is_floating = false;

	/*
	*	Specifies whether the windowed mode window will be initially visible. This hint is ignored for full screen windows.
	*/
	bool is_visible = true;

	/*
	*	 Specifies whether the full screen window will automatically iconify and restore
	*	 the previous video mode on input focus loss. This hint is ignored for windowed mode windows
	*/
	bool is_auto_iconify = true;

	/*
	*	Show FPS count next to window title in format: title - FPS: x
	*/
	bool is_show_fps = false;

	/*
	*	Show realtime RAM usage in percentage
	*/
	bool is_show_ram_usage = false;

	/*
	*	Show realtime CPU usage in percentage
	*/
	bool is_show_cpu_usage = false;


public:
	static WindowSettings FromConfig(const Config& config)
	{
		WindowSettings window_settings{};
		window_settings.title = config.Get<decltype(WindowSettings::title)>("window", "title", "Enigma");
		window_settings.width = config.Get<decltype(WindowSettings::width)>("window", "width", 800);
		window_settings.height = config.Get<decltype(WindowSettings::height)>("window", "height", 600);
		window_settings.minimum_width = config.Get<decltype(WindowSettings::minimum_width)>("window", "minimum_width", -1);
		window_settings.minimum_height = config.Get<decltype(WindowSettings::minimum_height)>("window", "minimum_height", -1);
		window_settings.maximum_width = config.Get<decltype(WindowSettings::maximum_width)>("window", "maximum_width", -1);
		window_settings.maximum_height = config.Get<decltype(WindowSettings::maximum_height)>("window", "maximum_height", -1);
		window_settings.refresh_rate = config.Get<decltype(WindowSettings::refresh_rate)>("window", "refresh_rate", -1);
		window_settings.samples = config.Get<decltype(WindowSettings::samples)>("window", "samples", 4);
		window_settings.swap_interval = config.Get<decltype(WindowSettings::swap_interval)>("window", "swap_interval", 2);
		window_settings.is_resizable = config.Get<decltype(WindowSettings::is_resizable)>("window", "resizable", true);
		window_settings.is_fullscreen = config.Get<decltype(WindowSettings::is_fullscreen)>("window", "fullscreen", false);
		window_settings.is_decorated = config.Get<decltype(WindowSettings::is_decorated)>("window", "decorated", true);
		window_settings.is_focused = config.Get<decltype(WindowSettings::is_focused)>("window", "focused", true);
		window_settings.is_maximized = config.Get<decltype(WindowSettings::is_maximized)>("window", "maximized", false);
		window_settings.is_floating = config.Get<decltype(WindowSettings::is_floating)>("window", "floating", false);
		window_settings.is_visible = config.Get<decltype(WindowSettings::is_visible)>("window", "visible", true);
		window_settings.is_auto_iconify = config.Get<decltype(WindowSettings::is_auto_iconify)>("window", "auto_iconify", true);
		window_settings.is_show_fps = config.Get<decltype(WindowSettings::is_show_fps)>("window", "show_fps", true);
		window_settings.is_show_ram_usage = config.Get<decltype(WindowSettings::is_show_ram_usage)>("window", "show_ram_usage", true);
		window_settings.is_show_cpu_usage = config.Get<decltype(WindowSettings::is_show_cpu_usage)>("window", "show_cpu_usage", true);
		return window_settings;
	}


	String toString() noexcept
	{
		std::ostringstream oss{};
		oss << "[ title:" << title << ", width:" << width << ", height:" << height
			<< ", minimum_width:" << minimum_width << ", minimum_height:" << minimum_height
			<< ", maximum_width:" << maximum_width << ", maximum_height:" << maximum_height
			<< ", refresh_rate:" << refresh_rate << ", swap_interval:" << swap_interval
			<< ", is_resizable:" << is_resizable << ", is_decorated:" << is_decorated
			<< ", is_fullscreen:" << is_fullscreen << ", samples:" << samples
			<< ", is_focused:" << is_focused << ", is_maximized:" << is_maximized
			<< ", is_floating:" << is_floating << ", is_visible:" << is_visible
			<< ", is_auto_iconify:" << is_auto_iconify << ", is_show_fps:" << is_show_fps
			<< " ]";
		return oss.str();
	}
};

NS_ENIGMA_END

#endif // !ENIGMA_WINDOW_SETTINGS_H
