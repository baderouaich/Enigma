#pragma once
#ifndef ENIGMA_APPLICATION_H
#define ENIGMA_APPLICATION_H

#include <Core/Core.hpp>
#include <Logger/Logger.hpp>

#include <Window/Window.hpp>

#include <Events/ApplicationEvent.hpp>
#include <Events/KeyEvent.hpp>
#include <Events/MouseEvent.hpp>

#include "SingleProcessInstance.hpp"

NS_ENIGMA_BEGIN

/**
*	Forward Declarations
*/
class ImGuiRenderer;
class Scene;
class RAMInfo;
class CPUInfo;

class ENIGMA_API Application final : public SingleProcessInstance
{
public:
	Application(const WindowSettings& window_settings = WindowSettings());
	virtual ~Application();

	/**
	*	Application Main Loop
	*/
	void Run();

	/**
	*	Immediate exit from application without waiting for GL flushes or scenes to be destroyed.
	*/
	void Exit(const String& message, i32 exit_code) noexcept;

	/**
	*	@brief Launches detached thread seperated from main UI thread
	*	@param loading_text: the reason behind launching this worker thread, will appear bellow loading spinner e.g: "Encrypting file..."
	*	@param scene: pointer to scene which spawns the worker thread
	*	@param work_func: function has work code to run in parallel
	*/
	void LaunchWorkerThread(Scene* scene, const std::string_view& loading_text, const std::function<void()>& work_func);


public: //https://www.doxygen.nl/manual/grouping.html#memgroup
	/** @{ */ 
	/**
	*	@brief Event callbacks
	*	@param event: event that was dispatched
	*	@return true if event is handled, false to pass it to other scenes
	*	@see Event.hpp
	*/
	void OnEvent(Event& event);
	bool OnWindowClose(WindowCloseEvent& event);
	bool OnWindowResize(WindowResizeEvent& event);
	bool OnFrameBufferResize(FrameBufferResizeEvent& event);
	/**	@} */


public: // Accessors
	/**
	*	Returns the instance of this Application
	*/
	static Application* GetInstance() noexcept { return m_instance; }

	/**
	*	Returns the Window of this Application
	*/
	const std::unique_ptr<Window>& GetWindow() const noexcept { return m_window; }

	/**
	*	Destroys Window and ends app
	*/
	void EndApplication() noexcept;

	/**
	*	Pushes new Scene to the stack
	*	Cleans memory when scene ends
	*/
	void PushScene(std::unique_ptr<Scene> scene);

	/**
	*	Returns FPS (Frames Per Second) count
	*/
	ui32 GetFPS() const noexcept { return *m_FPS; }

	/**
	*	Returns delta time value (the time difference between the previous frame that was drawn and the current frame)
	*/
	f32 GetDeltaTime() const noexcept { return m_delta_time; }


	/**
	*	Returns Realtime RAM usage informations
	*/
	constexpr const std::unique_ptr<RAMInfo>& GetRAMInfo() const noexcept { return m_ram_info; }

	/**
	*	Returns Realtime CPU usage informations
	*/
	constexpr const std::unique_ptr<CPUInfo>& GetCPUInfo() const noexcept { return m_cpu_info; }

	/**
	*	Returns Loaded Fonts map
	*/
	constexpr const std::unordered_map<std::string_view, ImFont*>& GetFonts() const noexcept { return m_fonts; }

private: // Updates
	/** Updates delta time */
	void UpdateDeltaTime() noexcept;
	/** Updates hardware info (CPU, RAM, FPS...) */
	void UpdateHardwareInfo() noexcept;


private: // Initializer Functions
	/** Initialize window */
	void InitWindow(const WindowSettings& window_settings);
	/** Initialize ImGui */
	void InitImGuiRenderer();
	/** Initialize Hardware info */
	void InitHardwareInfo(const WindowSettings& window_settings);
	/** Load Fonts */
	void InitImGuiFonts();

private: // Window
	std::unique_ptr<Window> m_window;  /**< App's window */

private: // Scenes
	std::vector<std::unique_ptr<Scene>> m_scenes; /**< List of active scenes */
	std::unique_ptr<Scene> m_loading_scene; /**< Loading scene overlay, will be displayed when a worker
											thread is launched which will show a loading spinner and prevent
											user from interacting with scene widgets until worker thread is finished */

private: // Delta time
	f32 m_last_frame_time; /**< Last frame time, helps calculating m_delta_time */
	f32 m_current_frame_time; /**< Current frame time, helps calculating m_delta_time */
	f32 m_delta_time; /**< Delta time between frames */

private: // Realtime Hardware Info (FPS, CPU, RAM info...), if its enabled, it will not be nullptr
	static constexpr f32 HARWARE_INFO_UPDATE_TIME = 1.0f; /**< Time to wait for each hardware info update -> 1 second */
	f32 m_hardware_info_timer; /**< Timer to update info each HARWARE_INFO_UPDATE_TIME */
	std::unique_ptr<ui32> m_FPS; /**< FPS info (if enabled in WindowSettings.ini) */
	std::unique_ptr<RAMInfo> m_ram_info; /**< RAM info (if enabled in WindowSettings.ini) */
	std::unique_ptr<CPUInfo> m_cpu_info; /**< CPU info (if enabled in WindowSettings.ini) */

private: // GUI
	std::unique_ptr<ImGuiRenderer> m_imgui_renderer;  /**< ImGui renderer wrapper */

private: // Fonts
	std::unordered_map<std::string_view, ImFont*> m_fonts; /**< Loaded fonts */


private: // Instance
	inline static Application* m_instance{ nullptr }; /**< Application singleton */
};

NS_ENIGMA_END


#endif // !ENIGMA_APPLICATION_H
