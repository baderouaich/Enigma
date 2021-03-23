#pragma once
#ifndef ENIGMA_APPLICATION_H
#define ENIGMA_APPLICATION_H

#include <Core/Core.hpp>
#include <Logger/Logger.hpp>

#include <Window/Window.hpp>

#include <Events/ApplicationEvent.hpp>
#include <Events/KeyEvent.hpp>
#include <Events/MouseEvent.hpp>


NS_ENIGMA_BEGIN

// Forward Declarations
class ImGuiRenderer;
class Scene;
class RAMInfo;
class CPUInfo;

class ENIGMA_API Application
{
public: /* Constructor / Destructors */
	Application(const WindowSettings& window_settings = WindowSettings());
	virtual ~Application();

	/*
	*	Application Main Loop
	*/
	void Run();

	/*
	*	Immediate exit from application without waiting for GL flushes or scenes to be destroyed.
	*/
	void Exit(const String& message, i32 exit_code) noexcept;

public: /* Events */
	void OnEvent(Event& event);
	bool OnWindowClose(WindowCloseEvent& event);
	bool OnWindowResize(WindowResizeEvent& event);
	bool OnFrameBufferResize(FrameBufferResizeEvent& event);

public: /*Accessors*/
	/*
	*	Returns the instance of this Application
	*/
	static Application* GetInstance() noexcept { return m_instance; }

	/*
	*	Returns the Window of this Application
	*/
	const std::unique_ptr<Window>& GetWindow() const noexcept { return m_window; }

	/*
	*	Destroys Window and ends app
	*/
	void EndApplication() noexcept;

	/*
	*	Pushes new Scene to the stack
	*	Cleans memory when scene ends
	*/
	void PushScene(const std::shared_ptr<Scene>& scene);

	/*
	*	Returns FPS (Frames Per Second) count
	*/
	constexpr const ui32& GetFPS() const noexcept { return m_FPS; }

	/*
	*	Returns delta time value (the time difference between the previous frame that was drawn and the current frame)
	*/
	constexpr const f32& GetDeltaTime() const noexcept { return m_delta_time; }


	/*
	*	Returns Realtime RAM usage informations
	*/
	constexpr const std::unique_ptr<RAMInfo>& GetRAMInfo() const noexcept { return m_ram_info; }

	/*
	*	Returns Realtime CPU usage informations
	*/
	constexpr const std::unique_ptr<CPUInfo>& GetCPUInfo() const noexcept { return m_cpu_info; }

	/*
	*	Returns Loaded Fonts map
	*/
	constexpr const std::unordered_map<std::string_view, ImFont*>& GetFonts() const noexcept { return m_fonts; }

private: /* Updaters */
	void UpdateDeltaTime() noexcept;
	//void UpdateFPS() noexcept;
	void UpdateHardwareInfo() noexcept;

private: /* Initializer Functions */
	void InitWindow(const WindowSettings& window_settings);
	void InitImGuiRenderer();
	void LoadImGuiFonts();

private: /* Window */
	std::unique_ptr<Window> m_window;

private: /* Scenes */
	std::vector<std::shared_ptr<Scene>> m_scenes;
	std::unique_ptr<Scene> m_loading_scene; // loading scene overlay

private: /* Delta time */
	f32 m_last_frame_time;
	f32 m_current_frame_time;
	f32 m_delta_time;

private: /* Realtime Hardware Info (FPS, CPU, RAM info...) */
	static constexpr f32 HARWARE_INFO_UPDATE_TIME = 1.0f; // time to wait for each hardware info update -> 1 second
	f32 m_hardware_info_timer; // timer to update info each HARWARE_INFO_UPDATE_TIME
	ui32 m_FPS;
	std::unique_ptr<RAMInfo> m_ram_info;
	std::unique_ptr<CPUInfo> m_cpu_info;

private: /* GUI */
	std::unique_ptr<ImGuiRenderer> m_imgui_renderer;

private: /* Fonts */
	std::unordered_map<std::string_view, ImFont*> m_fonts;


private: /* Instance */
	static Application* m_instance;

};

NS_ENIGMA_END


#endif // !ENIGMA_APPLICATION_H
