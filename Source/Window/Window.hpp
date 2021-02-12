#pragma once
#ifndef ENIGMA_WINDOW_H
#define ENIGMA_WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <Core/Core.hpp>
#include <Window/Cursor/Cursor.hpp>
#include <Events/Event.hpp>

#include <string>
#include <utility>
#include <exception>

#include "WindowSettings.hpp"


NS_ENIGMA_BEGIN

/*
*	OpenGL Based Window
*/
class ENIGMA_API Window
{
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

public:	/* Accessors */
	/*
	*	Returns size (width, height) of this window
	*/
	const std::pair<i32, i32>& GetSize() const noexcept;

	/*
	*	Returns the width of this window
	*/
	const i32& GetWidth() const noexcept;

	/*
	*	Returns the height of this window
	*/
	const i32& GetHeight() const noexcept;

	/*
	*	Returns frame buffer size of this window
	*/
	const std::pair<i32, i32>& GetFrameBufferSize() noexcept;

	/*
	*	Returns frame buffer width of this window
	*/
	const i32& GetFrameBufferWidth() noexcept;
	
	/*
	*	Returns frame buffer height of this window
	*/
	const i32& GetFrameBufferHeight() noexcept;

	/*
	*	Returns the aspect ratio ( width / height ) of this window
	*	Returns 1.0f if height is 0 (to avoid division by zero error)
	*/
	f32 GetAspectRatio() const noexcept;

	/*
	*	Returns const pointer of GLFW window
	*/
	const GLFWwindow* GetGLFWwindow() const noexcept;

	/*
	*	Returns pointer of GLFW window
	*/
	GLFWwindow* GetGLFWwindow() noexcept;

	/*
	*	Returns the title of this window
	*/
	const String& GetTitle() const noexcept;
	
	/*
	*	Returns minimum width, height of this window
	*/
	const std::pair<i32, i32>& GetMinimumSize() const noexcept;

	/*
	*	Returns maximum width, height of this window
	*/
	const std::pair<i32, i32>& GetMaximumSize() const noexcept;

	/*
	*	Returns window top left position
	*/
	const std::pair<i32, i32>& GetPosition() const noexcept;

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

	/*
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
	*	Returns true if vertical sync is enabled
	*/
	const bool& IsVSync() const noexcept;

	/*
	*	Returns true if the window is fullscreen mode
	*/
	bool IsFullscreen() const noexcept;

	/*
	*	Returns refresh rate of this window
	*/
	const i32& GetRefreshRate() noexcept;

public: /* Modifiers */
	using EventCallback = std::function<void(Event&)>;
	/*
	*	Sets event callback function
	*/
	void SetEventCallback(const EventCallback& callback) noexcept;

	/*
	*	Set window minimum width, height
	*/
	void SetMinimumSize(const i32& minimum_width, const i32& minimum_height) noexcept;

	/*
	*	Set window maximum width, height
	*/
	void SetMaximumSize(const i32& maximum_width, const i32& maximum_height) noexcept;

	/*
	*	Close Window
	*/
	void SetShouldClose(const bool close) const noexcept;
	
	/*
	*	Enable / Disable Vertical Sync
	*/
	void SetVsync(bool vsync) noexcept;
	
	/*
	*	Set Window Title
	*/
	void SetTitle(const String& title) noexcept;
	
	/*
	*	Set window top left position
	*/
	void SetPosition(const i32& x, const i32& y) const noexcept;

	/*
	*	Set Window runtime icon
	*/
	void SetIcon(const String& icon_path) noexcept;
	
	/*
	*	Set Window cursor mode
	*/
	void SetCursor(CursorMode mode) noexcept;
	
	/*
	*	Set Window cursor image
	*/
	void SetCursor(const String& image_path, const i32& xhot, const i32& yhot) noexcept;

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
	void SetCursorPosition(const f64& xPos, const f64& yPos) const noexcept;
	
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
	void SetRefreshRate(const i32& refresh_rate) noexcept;

private: /* Initializer Functions */
	bool InitGLFW(const WindowSettings& window_settings);
	bool InitGLFWCallbacks();
	bool InitGLAD();
	bool InitOpenGLOptions();


private: /* Events */
	EventCallback m_event_callback;


private: /* Properties */
	String m_title;
	std::pair<i32, i32> m_position;
	std::pair<i32, i32> m_size;
	std::pair<i32, i32> m_minimum_size;
	std::pair<i32, i32> m_maximum_size;
	std::pair<i32, i32> m_frame_buffer_size;
	bool m_vsync;
	bool m_is_show_fps;
	bool m_is_show_ram_usage;
	bool m_is_show_cpu_usage;

	GLFWwindow* m_GLFWwindow; // GLFW window
	GLFWmonitor* m_monitor;
	const GLFWvidmode* m_video_mode;

	std::unique_ptr<Cursor> m_cursor;	// Window cursor

public: /* Good Friends :) */
	friend class Application;
};

NS_ENIGMA_END


#endif // !ENIGMA_WINDOW_H
