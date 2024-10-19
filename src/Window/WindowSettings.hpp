#pragma once
#ifndef ENIGMA_WINDOW_SETTINGS_H
#define ENIGMA_WINDOW_SETTINGS_H

#include <Config/Config.hpp>
#include <Core/Core.hpp>
#include <GLFW/glfw3.h> //GLFW_DONT_CARE

NS_ENIGMA_BEGIN

struct WindowSettings {
    /*
    *	Title of the window (Displayed in the title bar)
    */
    std::string title = "Enigma";

    /*
    *	Width in pixels of the window
    */
    std::int32_t width = 960;

    /*
    *	Height in pixels of the window
    */
    std::int32_t height = 720;

    /*
    *	Minimum width of the window.
    */
    std::int32_t minimum_width = GLFW_DONT_CARE;

    /*
    *	Minimum height of the window.
    */
    std::int32_t minimum_height = GLFW_DONT_CARE;

    /*
	  *	Maximum width of the window.
    */
    std::int32_t maximum_width = GLFW_DONT_CARE;

    /*
    *	Maximum height of the window.
    */
    std::int32_t maximum_height = GLFW_DONT_CARE;

    /*
    *	Window Maximum Frames Per Second.
    */
    //std::uint32_t maximum_fps = 60;

    /*
    *	 Specifies the desired refresh rate for full screen windows. If set to -1, the highest
    *	 available refresh rate will be used. This hint is ignored for windowed mode windows.
    */
    std::int32_t refresh_rate = GLFW_DONT_CARE;

    /*
    *	 Defines the number of samples to use (For anti-aliasing)
    */
    std::uint32_t samples = 4;


    /*
    *	Vertical sync swap interval
    *
    *	Interval 0: unlimited FPS
    *	Interval 1: 60 FPS
    *	Interval 2: 30 FPS
    */
    std::int32_t swap_interval = 2;


    /*
    *	Specifies if the window is by default in fullscreen or windowed mode
    */
    bool fullscreen = false;

    /*
    *	 Specifies whether the windowed mode window will have window decorations such as a border, a close widget, etc.
    *	 An undecorated window may still allow the user to generate close events on some platforms. This hint is ignored
    *	 for full screen windows.
    */
    bool decorated = true;

    /*
    *	Specifies whether the windowed mode window will be resizable by the user. The window will still be resizable using
    *	the "SetSize(std::int32_t, std::int32_t)" method of the "Window" class. This hint is ignored for full screen windows
    */
    bool resizable = true;

    /*
    *	 Specifies whether the windowed mode window will be given input focus when created. This hint is ignored for
    *	 full screen and initially hidden windows.
    */
    bool focused = true;

    /*
    *	Specifies whether the windowed mode window will be maximized when created. This hint is ignored for full screen windows.
    */
    bool maximized = false;

    /*
    *	 Specifies whether the windowed mode window will be floating above other regular windows, also called topmost or always-on-top.
    *	 This is intended primarily for debugging purposes and cannot be used to implement proper full screen windows. This hint is
    *	 ignored for full screen windows.
    */
    bool floating = false;

    /*
    *	Specifies whether the windowed mode window will be initially visible. This hint is ignored for full screen windows.
    */
    bool visible = true;

    /*
    *	 Specifies whether the full screen window will automatically iconify and restore
    *	 the previous video mode on input focus loss. This hint is ignored for windowed mode windows
	  */
    bool auto_iconify = true;

    /*
    *	Show FPS count next to window title in format: title - FPS: x
    */
    bool show_fps = false;

    /*
    *	Show realtime RAM usage in percentage
    */
    bool show_ram_usage = false;

    /*
    *	Show realtime CPU usage in percentage
    */
    bool show_cpu_usage = false;


    /** Default construct WindowSettings with default values */
    WindowSettings() = default;

    /** Construct WindowSettings from a Config file (.ini) */
    explicit WindowSettings(const Config& config) : title(config.Get<decltype(WindowSettings::title)>("window", "title", "Enigma")),
                                                    width(config.Get<decltype(WindowSettings::width)>("window", "width", 800)),
                                                    height(config.Get<decltype(WindowSettings::height)>("window", "height", 600)),
                                                    minimum_width(config.Get<decltype(WindowSettings::minimum_width)>("window", "minimum_width", -1)),
                                                    minimum_height(config.Get<decltype(WindowSettings::minimum_height)>("window", "minimum_height", -1)),
                                                    maximum_width(config.Get<decltype(WindowSettings::maximum_width)>("window", "maximum_width", -1)),
                                                    maximum_height(config.Get<decltype(WindowSettings::maximum_height)>("window", "maximum_height", -1)),
                                                    refresh_rate(config.Get<decltype(WindowSettings::refresh_rate)>("window", "refresh_rate", -1)),
                                                    samples(config.Get<decltype(WindowSettings::samples)>("window", "samples", 4)),
                                                    swap_interval(config.Get<decltype(WindowSettings::swap_interval)>("window", "swap_interval", 2)),
                                                    resizable(config.Get<decltype(WindowSettings::resizable)>("window", "resizable", true)),
                                                    fullscreen(config.Get<decltype(WindowSettings::fullscreen)>("window", "fullscreen", false)),
                                                    decorated(config.Get<decltype(WindowSettings::decorated)>("window", "decorated", true)),
                                                    focused(config.Get<decltype(WindowSettings::focused)>("window", "focused", true)),
                                                    maximized(config.Get<decltype(WindowSettings::maximized)>("window", "maximized", false)),
                                                    floating(config.Get<decltype(WindowSettings::floating)>("window", "floating", false)),
                                                    visible(config.Get<decltype(WindowSettings::visible)>("window", "visible", true)),
                                                    auto_iconify(config.Get<decltype(WindowSettings::auto_iconify)>("window", "auto_iconify", true)),
                                                    show_fps(config.Get<decltype(WindowSettings::show_fps)>("window", "show_fps", true)),
                                                    show_ram_usage(config.Get<decltype(WindowSettings::show_ram_usage)>("window", "show_ram_usage", true)),
                                                    show_cpu_usage(config.Get<decltype(WindowSettings::show_cpu_usage)>("window", "show_cpu_usage", true)) {
    }

  public:
    std::string toString() noexcept {
      std::ostringstream oss{};
      oss << "[ title:" << title << ", width:" << width << ", height:" << height
          << ", minimum_width:" << minimum_width << ", minimum_height:" << minimum_height
          << ", maximum_width:" << maximum_width << ", maximum_height:" << maximum_height
          << ", refresh_rate:" << refresh_rate << ", swap_interval:" << swap_interval
          << ", resizable:" << resizable << ", decorated:" << decorated
          << ", fullscreen:" << fullscreen << ", samples:" << samples
          << ", focused:" << focused << ", maximized:" << maximized
          << ", floating:" << floating << ", visible:" << visible
          << ", auto_iconify:" << auto_iconify << ", show_fps:" << show_fps
          << " ]";
      return oss.str();
    }
};

NS_ENIGMA_END

#endif // !ENIGMA_WINDOW_SETTINGS_H
