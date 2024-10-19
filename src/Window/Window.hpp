#pragma once
#ifndef ENIGMA_WINDOW_H
#define ENIGMA_WINDOW_H

#include <glad/glad.h> // Warning! MUST ALWAYS BE INCLUDED BEFORE glfw3
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <Core/Core.hpp>
#include <Events/Event.hpp>
#include <Window/Cursor/Cursor.hpp>

#include <exception>
#include <string>
#include <utility>

#include "WindowSettings.hpp"


NS_ENIGMA_BEGIN
class RAMInfo;
class CPUInfo;
/*
*	OpenGL Based Window
*/
class Window {
    using EventCallback = std::function<void(Event&)>;

  public:
    Window(const WindowSettings& window_settings = WindowSettings());
    virtual ~Window();

  private:
    /*
    *	Polls GLFW Events
    */
    void PollEvents() const noexcept;

    /*
    *	Swaps GLFW Buffets
    */
    void SwapBuffers() const noexcept;

    /*
    *	Destroys GLFW Window and Terminates
    */
    void Destroy() noexcept;

  public: /* Accessors */
    /*
    *	Returns size (width, height) of this window
    */
    [[nodiscard]] const std::pair<std::int32_t, std::int32_t>& GetSize() const noexcept;

    /*
    *	Returns the width of this window
    */
    std::int32_t GetWidth() const noexcept;

    /*
    *	Returns the height of this window
    */
    std::int32_t GetHeight() const noexcept;

    /*
    *	Returns frame buffer size of this window
    */
    [[nodiscard]] const std::pair<std::int32_t, std::int32_t>& GetFrameBufferSize() noexcept;

    /*
    *	Returns frame buffer width of this window
    */
    std::int32_t GetFrameBufferWidth() noexcept;

    /*
    *	Returns frame buffer height of this window
    */
    std::int32_t GetFrameBufferHeight() noexcept;

    /*
    *	Returns the aspect ratio ( width / height ) of this window
    *	Returns 1.0f if height is 0 (to avoid division by zero error)
    */
    float GetAspectRatio() const noexcept;

    /*
    *	Returns const pointer of GLFW window
    */
    const GLFWwindow *GetGLFWwindow() const noexcept;

    /*
    *	Returns pointer of GLFW window
    */
    GLFWwindow *GetGLFWwindow() noexcept;

    /*
    *	Returns the title of this window
    */
    [[nodiscard]] const std::string& GetTitle() const noexcept;

    /*
    *	Returns minimum width, height of this window
    */
    [[nodiscard]] const std::pair<std::int32_t, std::int32_t>& GetMinimumSize() const noexcept;

    /*
    *	Returns maximum width, height of this window
    */
    [[nodiscard]] const std::pair<std::int32_t, std::int32_t>& GetMaximumSize() const noexcept;

    /*
    *	Returns window top left position
    */
    [[nodiscard]] const std::pair<std::int32_t, std::int32_t>& GetPosition() const noexcept;

    /*
    *	Returns vsync swap interval level
    * 	Interval 0: unlimited FPS
    *	Interval 1: 60 FPS
    *	Interval 2: 30 FPS
    */
    std::int32_t GetSwapInterval() const noexcept;

    /*
    *	Returns true if the window should be closed
    */
    bool ShouldClose() const noexcept;

    /*
    * Return true if the window is hidden
    */
    bool IsHidden() const noexcept;

    /*
    * Return true if the window is visible
    */
    bool IsVisible() const noexcept;

    /**
    * Return true if the windows is maximized
    */
    bool IsMaximized() const noexcept;

    /*
    * Return true if the windows is minimized
    */
    bool IsMinimized() const noexcept;

    /*
    * Return true if the windows is focused
    */
    bool IsFocused() const noexcept;

    /*
    * Return true if the windows is resizable
    */
    bool IsResizable() const noexcept;

    /*
    * Return true if the windows is decorated
    */
    bool IsDecorated() const noexcept;

    /*
    *	Returns true if the window is fullscreen mode
    */
    bool IsFullscreen() const noexcept;

    /*
    *	Returns refresh rate of this window
    */
    std::int32_t GetRefreshRate() noexcept;

    /*
    *	Returns size (width, height) of the primary monitor
    */
    std::pair<std::int32_t, std::int32_t> GetMonitorSize() noexcept;

  public: /* Modifiers */
    /*
	  *	Sets event callback function
    */
    void SetEventCallback(const EventCallback& callback) noexcept;

    /*
    *	Set window minimum width, height
    */
    void SetMinimumSize(const std::int32_t minimum_width, const std::int32_t minimum_height) noexcept;

    /*
    *	Set window maximum width, height
    */
    void SetMaximumSize(const std::int32_t maximum_width, const std::int32_t maximum_height) noexcept;

    /*
    *	Close Window
    */
    void SetShouldClose(const bool close) const noexcept;

    /*
    *	Sets Vertical Sync swap interval
    * 	Interval 0: unlimited FPS
    *	Interval 1: 60 FPS
    *	Interval 2: 30 FPS
	  */
    void SetSwapInterval(const std::int32_t interval) noexcept;

    /*
    *	Set Window Title
    */
    //void SetTitle(const std::string& title) noexcept;
    void SetTitle(const std::string& title, const std::unique_ptr<std::uint32_t>& fps, const std::unique_ptr<RAMInfo>& ram_info, const std::unique_ptr<CPUInfo>& cpu_info) noexcept;

    /*
    *	Set window top left position
    */
    void SetPosition(const std::int32_t x, const std::int32_t y) const noexcept;

    /*
    *	Set Window runtime icon
    */
    void SetIcon(const std::string& icon_path) noexcept;
    void SetIcon(const byte* iconData, std::size_t iconDataSize) noexcept;

    /*
    *	Set Window cursor mode
    */
    void SetCursor(CursorMode mode) noexcept;

    /*
    *	Set Window cursor image
    */
    void SetCursor(const std::string& image_path, const std::int32_t xhot, const std::int32_t yhot) noexcept;

    /*
    *	Disable Window Cursor
    */
    void DisableCursor() const noexcept;

    /*
    *	Enable Window Cursor
    */
    void EnableCursor() const noexcept;

    /*
    *	Hide Window Cursor
    */
    void HideCursor() const noexcept;

    /*
    *	Sets cursor position at given point
    */
    void SetCursorPosition(const double xPos, const double yPos) const noexcept;

    /*
    *	Minimize Window
    */
    void Minimize() const noexcept;

    /*
    *	Maximize Window
    */
    void Maximize() const noexcept;

    /*
    *	Restore Window
    */
    void Restore() const noexcept;

    /*
    *	Hide Window
    */
    void Hide() const noexcept;

    /*
    *	Show Window
    */
    void Show() const noexcept;

    /*
    *	Focus Window
    */
    void Focus() const noexcept;

    /*
    *	Enable / Disable Fullscreen mode
    */
    void SetFullscreen(bool full_screen) noexcept;

    /*
    *	Set window refresh rate
    */
    void SetRefreshRate(const std::int32_t refresh_rate) noexcept;

  private: /* Initializer Functions */
    bool InitGLFW(const WindowSettings& window_settings);
    bool InitGLFWCallbacks();
    bool InitGLAD();
    bool InitOpenGLOptions();


  private: /* Events */
    EventCallback m_event_callback{};


  private: /* Properties */
    // Window properties
    std::string m_title{};
    std::pair<std::int32_t, std::int32_t> m_position{};
    std::pair<std::int32_t, std::int32_t> m_size{};
    std::pair<std::int32_t, std::int32_t> m_minimum_size{};
    std::pair<std::int32_t, std::int32_t> m_maximum_size{};
    std::pair<std::int32_t, std::int32_t> m_frame_buffer_size{};
    std::int32_t m_swap_interval{};

    // Monitor & Video Mode
    GLFWwindow *m_GLFWwindow{};
    GLFWmonitor *m_monitor{};
    const GLFWvidmode *m_video_mode{};

    // Window Cursor
    std::unique_ptr<Cursor> m_cursor{};

  public: /* Good Friends :) */
    friend class Application;
};

NS_ENIGMA_END


#endif // !ENIGMA_WINDOW_H
