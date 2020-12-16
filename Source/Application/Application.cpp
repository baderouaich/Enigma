#include <pch.hpp>
#include <Core/Main.hpp> // Entry point
#include "Application.hpp"

#include <Window/Window.hpp>
#include <Window/WindowSettings.hpp>
#include <UI/ImGui/ImGuiRenderer.hpp>
#include <Utility/DialogUtils.hpp>

NS_ENIGMA_BEGIN

Application* Application::m_instance = nullptr;


Application::Application(const WindowSettings& window_settings)
	:
	//Delta time
	m_last_frame_time(0.0f),
	m_current_frame_time(0.0f),
	m_delta_time(0.0f),
	//FPS
	m_FPS_timer(0.0f),
	m_FPS(0)
	//,m_max_FPS(window_settings.maximum_fps)
{
	ENIGMA_ASSERT(!m_instance, "Application Instance already exists");
	m_instance = this;

	this->InitWindow(window_settings);
	this->InitImGuiRenderer();
}

void Application::InitWindow(const WindowSettings& window_settings)
{
	try
	{
		// Create Window
		m_window = std::make_unique<Window>(window_settings);
		// Set Window Events callback
		m_window->SetEventCallback(ENIGMA_BIND_FUN(Application::OnEvent));
	}
	catch (const std::exception& e)
	{
		const String msg = "Couldn't Construct Window: " + String(e.what());
		// console alert
		ENIGMA_ERROR(msg.c_str());
		// ui alert
		[[maybe_unused]] auto action = DialogUtils::Error(msg);		
		// exit
		this->Exit("Couldn't Construct Window", EXIT_FAILURE); // No Application without a window :c
	}
}

/*
void Application::InitSceneData()
{
	m_scene_data.window = &(*m_window);
	m_scene_data.delta_time = &m_delta_time;
	m_scene_data.scenes = &m_scenes;
	m_scene_data.FPS = &m_FPS;
}
*/

void Application::InitImGuiRenderer()
{
	m_imgui_renderer = std::make_unique<ImGuiRenderer>();
}

void Application::PushScene(const std::shared_ptr<Scene>& scene)
{
	ENIGMA_ASSERT(scene.get(), "Scene is nullptr");

	// Push scene & Notify user on scene created
	this->m_scenes.emplace_back(scene)->OnCreate(); // c++17 only emplace_back returns inserted item
	
	// Notify user on scene created
	//scene->OnCreate();
}

void Application::OnEvent(Event& event)
{
	// Listen for WindowClose & WindowResize Events
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<WindowCloseEvent>(ENIGMA_BIND_FUN(Application::OnWindowClose));
	dispatcher.Dispatch<WindowResizeEvent>(ENIGMA_BIND_FUN(Application::OnWindowResize));
	dispatcher.Dispatch<FrameBufferResizeEvent>(ENIGMA_BIND_FUN(Application::OnFrameBufferResize));
	
	// Alert All Scenes OnEvent
	for (auto it = m_scenes.rbegin(); it != m_scenes.rend(); ++it)
	{
		// Alert each scene with the triggered event
		(*it)->OnEvent(event);

		// if event is handled, stop passing it to other scenes
		if (event.IsHandled())
			break;
	}
	
}

bool Application::OnWindowClose(WindowCloseEvent& event)
{
	ENIGMA_INFO("{0}: Closing Window due WindowCloseEvent", ENIGMA_CURRENT_FUNCTION);

	this->EndApplication();
	
	return true; //the end of the app, all events are handled.
}

bool Application::OnWindowResize(WindowResizeEvent& event)
{
	ENIGMA_INFO("{0}: {1}", ENIGMA_CURRENT_FUNCTION, event.ToString().c_str());

	// Update OpenGL Viewport
	glAssert( glViewport(0, 0, event.GetWidth(), event.GetHeight()) );

	return false;
}



bool Application::OnFrameBufferResize(FrameBufferResizeEvent& event)
{
	ENIGMA_INFO("{0}: {1}", ENIGMA_CURRENT_FUNCTION, event.ToString().c_str());

	// Update OpenGL Viewport
	glAssert( glViewport(0, 0, event.GetWidth(), event.GetHeight()) );

	return false;
}



void Application::Run()
{
	//TODO: MORE WORK
	while (! m_window->ShouldClose())
	{
		// Poll Events
		m_window->PollEvents();

		// Update & Draw (only if there are scenes, otherwise end app).
		if (! m_scenes.empty())
		{
			//Update
			{
				// Delta time
				UpdateDeltaTime();
				// FPS
				UpdateFPS();
				// Update back scene (last pushed scene which is the active one)
				m_scenes.back()->OnUpdate(m_delta_time);
			}
			
			//Draw
			{
				// Draw back scene (last pushed scene which is the active one)
				m_scenes.back()->OnDraw();

				// ImGui
				m_imgui_renderer->Begin();
					m_scenes.back()->OnImGuiDraw();
				m_imgui_renderer->End();

				// Force execution of GL commands in finite time 
				glAssert( glFlush() );
			}

			// Check if the current active scene wants to quit
			if (m_scenes.back()->WantsQuit())
			{
				// Notify user before ending scene
				m_scenes.back()->OnDestroy();
				
				// Destroy Scene
				m_scenes.back()->EndScene(); // just to make sure, even if m_quit is true (who knows what can happen in OnDestroy)
				m_scenes.pop_back(); // Remove scene from vector (btw vector will call ~unique_ptr to cleanup memory)
			}

		}
		else // No scenes ? end App.
		{
			this->EndApplication();
		}

		// Swap Buffers
		m_window->SwapBuffers();
	}
}


void Application::Exit(const String& message, i32 exit_code)
{
	if (Logger::GetLogger())
	{
		ENIGMA_ERROR("Application has exited with code {0} : {1}", exit_code, message.c_str());
	}
	else
	{
		std::cerr << "Application has exited with code " << exit_code << " : " << message << std::endl;
	}
	std::exit(exit_code);
}


void Application::UpdateDeltaTime() noexcept
{
	m_current_frame_time = static_cast<f32>(glfwGetTime());
	m_delta_time = m_current_frame_time - m_last_frame_time;
	m_last_frame_time = m_current_frame_time;
}

void Application::UpdateFPS() noexcept
{
#if 0
	/// Limit FPS
	while (static_cast<f32>(glfwGetTime()) < m_last_frame_time + (1.0f / m_max_FPS))
	{
		// TODO: whats the best to use in this case, yield or sleep_for?
		// Put the thread to sleep
		std::this_thread::yield();

		//const auto sleep_millis = static_cast<i64>(((1.0 / m_max_FPS) - m_delta_time) * 1000.0);
		//std::this_thread::sleep_for(std::chrono::milliseconds(sleep_millis));
		/*
		yield()
		will stop the execution of the current thread and give priority to other process/threads
		(if there are other process/threads waiting in the queue). 
		The execution of the thread is not stopped. (it just release the CPU).

		sleep_for()
		will make your thread sleep for a given time (the thread is stopped for a given time).
		*/
	}
	///
#endif
	/// Update Frames per second & set to window title
	if (m_window->m_is_show_fps)
	{
		m_FPS++;
		m_FPS_timer += m_delta_time;
		if (m_FPS_timer >= 1.0f)
		{
			m_window->SetTitle(m_window->GetTitle());
			m_FPS = 0;
			m_FPS_timer = 0.0f;
		}
	}
	///
}


void Application::EndApplication() noexcept
{
	m_window->SetShouldClose(true);
}


Application::~Application()
{
	// OnDestroy() alert scenes
	std::for_each(m_scenes.rbegin(), m_scenes.rend(), [](const auto& scene)
	{
		// Notify scenes OnDestroy before closing application
		scene->OnDestroy();
	});
	m_scenes.clear();
}
NS_ENIGMA_END

