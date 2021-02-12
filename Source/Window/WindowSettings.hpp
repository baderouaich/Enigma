#pragma once
#ifndef ENIGMA_WINDOW_SETTINGS_H
#define ENIGMA_WINDOW_SETTINGS_H

#include <Core/Core.hpp>
#include <GLFW/glfw3.h>		//GLFW_DONT_CARE

NS_ENIGMA_BEGIN

struct ENIGMA_API WindowSettings
{
	/*
	*	Title of the window (Displayed in the title bar)
	*/
	String title = "Enigma Application";

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
	*	Enable/Disable vertical sync
	*/
	bool is_vsync = false;

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

	/*
	*	 Specifies the desired refresh rate for full screen windows. If set to -1, the highest
	*	 available refresh rate will be used. This hint is ignored for windowed mode windows.
	*/
	i32 refresh_rate = GLFW_DONT_CARE;

	/*
	*	 Defines the number of samples to use (For anti-aliasing)
	*/
	ui32 samples = 4;
};

NS_ENIGMA_END

#endif // !ENIGMA_WINDOW_SETTINGS_H
