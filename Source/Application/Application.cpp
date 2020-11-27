#include <pch.hpp>
#include <Core/Main.hpp> // Entry point
#include "Application.hpp"

#include <Window/Window.hpp>
#include <Window/WindowSettings.hpp>
#include <UI/ImGui/ImGuiRenderer.hpp>

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
{
	ENIGMA_CORE_ASSERT(!m_instance, "Application Instance already exists");
	m_instance = this;

	this->InitWindow(window_settings);
	this->InitImGuiRenderer();
	this->InitSceneData();

}

void Application::InitWindow(const WindowSettings& window_settings)
{
	try
	{
		// Create Window
		m_window = Window::CreateShared(window_settings);
		// Set Window Events callback
		m_window->SetEventCallback(ENIGMA_BIND_FUN(Application::OnEvent));
	}
	catch (const std::exception& e)
	{
		ENIGMA_CORE_ERROR("{0}", e.what());
		this->Exit("Couldn't Construct Window", EXIT_FAILURE); // No Application without a window :c
	}
}


void Application::InitSceneData()
{
	m_scene_data.window = &(*m_window);
	m_scene_data.delta_time = &m_delta_time;
	m_scene_data.scenes = &m_scenes;
	m_scene_data.FPS = &m_FPS;
}


void Application::InitImGuiRenderer()
{
	m_imgui_renderer = ImGuiRenderer::CreateUnique();
}

void Application::PushScene(Scene* scene)
{
	ENIGMA_CORE_ASSERT(scene, "Scene is nullptr");

	scene->m_scene_data = &m_scene_data;
	this->m_scenes.emplace_back(scene);

	// Notify user on scene created
	scene->OnCreate();
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
	ENIGMA_CORE_INFO("{0}: Closing Window due WindowCloseEvent", ENIGMA_CURRENT_FUNCTION);
	
	this->EndApplication();
	
	return true; //the end of the app, all events are handled.
}

bool Application::OnWindowResize(WindowResizeEvent& event)
{
	ENIGMA_CORE_INFO("{0}: {1}", ENIGMA_CURRENT_FUNCTION, event.ToString().c_str());

	// Update OpenGL Viewport
	glAssert( glViewport(0, 0, event.GetWidth(), event.GetHeight()) );

	return false;
}



bool Application::OnFrameBufferResize(FrameBufferResizeEvent& event)
{
	ENIGMA_CORE_INFO("{0}: {1}", ENIGMA_CURRENT_FUNCTION, event.ToString().c_str());

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
				if(m_window->m_is_show_fps)
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
				ENIGMA_SAFE_DELETE_PTR(m_scenes.back());
				m_scenes.pop_back();
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


void Application::Exit(const String& message, i32 code)
{
	if (Logger::GetCoreLogger())
	{
		ENIGMA_CORE_ERROR("Application has exited with code {0} : {1}", code, message.c_str());
	}
	else
	{
		std::cerr << "Application has exited with code " << code << " : " << message << std::endl;
	}
	std::exit(code);
}


void Application::UpdateDeltaTime() noexcept
{
	m_current_frame_time = static_cast<f32>(glfwGetTime());
	m_delta_time = m_current_frame_time - m_last_frame_time;
	m_last_frame_time = m_current_frame_time;
}

void Application::UpdateFPS() noexcept
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


void Application::EndApplication() noexcept
{
	m_window->SetShouldClose(true);
}


Application::~Application()
{
	// Cleanup Scenes
	std::for_each(m_scenes.rbegin(), m_scenes.rend(), [](auto& scene)
	{
		ENIGMA_SAFE_DELETE_PTR(scene);
	});
	m_scenes.clear();
}
NS_ENIGMA_END

