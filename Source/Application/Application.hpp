#pragma once
#ifndef ENIGMA_APPLICATION_H
#define ENIGMA_APPLICATION_H

#include <Core/Core.hpp>
#include <Logger/Logger.hpp>
#include <Scenes/Scene.hpp>
#include <Events/Event.hpp>
#include <Events/ApplicationEvent.hpp>
#include <Events/KeyEvent.hpp>
#include <Events/MouseEvent.hpp>

#include <GLFW/glfw3.h>

#include <string>

/*
*	main entry point is handled by the Enigma::Application
*/
extern int main(int argc, char* argv[]);

NS_ENIGMA_BEGIN

// Forward Declarations
struct WindowSettings;
class Window;
class ImGuiRenderer;

class ENIGMA_API Application
{
private:
	/*
	*	ENTRY POINT
	*	Making the main function a friend so we can access
	*	private function app->Run(); from it
	*/
	friend int ::main(int argc, char* argv[]);

	/*
	*	Application Main Loop
	*/
	void Run();

	/*
	*	Immediate exit from application without waiting for GL flushes or scenes to be destroyed. 
	*/
	void Exit(const String& message, i32 exit_code) noexcept;
	

public: /* Constructor / Destructors */
	Application(const WindowSettings& window_settings = WindowSettings());
	virtual ~Application();

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

private: /* Updaters */
	void UpdateDeltaTime() noexcept;
	void UpdateFPS() noexcept;

private: /* Initializer Functions */
	void InitWindow(const WindowSettings& window_settings);
	void InitImGuiRenderer();

private: /* Window */
	std::unique_ptr<Window> m_window;

private: /* Scenes */
	std::vector<std::shared_ptr<Scene>> m_scenes;

	//TODO: maybe not needed, since we can access everything from Application::GetInstance()
	//Scene data, to access everything we need (window, settings, other scenes...) from all scenes
	//SceneData m_scene_data;

private: /* Delta time */
	f32 m_last_frame_time;
	f32 m_current_frame_time;
	f32 m_delta_time;

private: /* FPS */
	f32 m_FPS_timer;
	ui32 m_FPS;
	//ui32 m_max_FPS;

private: /* ImGui */
	std::unique_ptr<ImGuiRenderer> m_imgui_renderer;

private: /* Instance */
	static Application* m_instance;

};

/*
*	Entry Point to be defined in user side
*/
extern std::unique_ptr<Enigma::Application> CreateApplication();

NS_ENIGMA_END


#endif // !ENIGMA_APPLICATION_H
