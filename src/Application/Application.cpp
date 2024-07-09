#include <pch.hpp>
#include "Application.hpp"

#include <GUI/ImGuiRenderer.hpp>
#include <Utility/DialogUtils.hpp>

#include <Analytics/Hardware/CPU/CPUInfo.hpp>
#include <Analytics/Hardware/RAM/RAMInfo.hpp>

#include <Events/EventDispatcher.hpp>
#include <Scenes/LoadingScene.hpp>
#include <Scenes/MainMenuScene.hpp>
#include <Utility/OpenGLUtils.hpp>

NS_ENIGMA_BEGIN

Application::Application(const WindowSettings& window_settings)
    : SingleProcessInstance(Constants::ENIGMA_SINGLE_PROCESS_UNIQUE_PORT),
      // Delta time
      m_last_frame_time(0.0f),
      m_current_frame_time(0.0f),
      m_delta_time(0.0f),
      // Analytics
      m_FPS(nullptr),
      m_ram_info(nullptr),
      m_cpu_info(nullptr),
      m_hardware_info_timer(0.0f) {
  // Check if there is an enigma process already running or not.
  if (!SingleProcessInstance::IsUnique()) {
    DialogUtils::Warn("Another instance of Enigma is already running!");
    return;
  }


  ENIGMA_ASSERT(!m_instance, "Application Instance already exists");
  m_instance = this;

  // Init thread-pool
  m_threadPool = std::make_unique<cpr::ThreadPool>();
  m_threadPool->SetMinThreadNum(0);
  m_threadPool->SetMaxThreadNum(std::thread::hardware_concurrency());
  m_threadPool->Start(0);

  // Initializers
  this->InitWindow(window_settings);
  this->InitImGuiRenderer();
  this->InitImGuiFonts();
  this->InitHardwareInfo(window_settings);

  // Init loading scene
  m_loading_scene = std::make_unique<LoadingScene>();
  m_loading_scene->OnCreate();

  // Push Main Menu scene as an entry point
  this->PushScene(std::make_unique<MainMenuScene>());
}

void Application::InitWindow(const WindowSettings& window_settings) {
  try {
    // Create Window
    m_window = std::make_unique<Window>(window_settings);

    // Set Window Events callback
    m_window->SetEventCallback(ENIGMA_BIND_FUN(Application::OnEvent));

    // Set Window runtime icon
    m_window->SetIcon(Constants::Resources::Textures::ENIGMA_LOGO_PNG_PATH.string());

    // Set window top left position at center
    const auto [monitor_width, monitor_height] = m_window->GetMonitorSize();
    const auto& [window_width, window_height] = m_window->GetSize();
    m_window->SetPosition(static_cast<std::int32_t>((monitor_width - window_width) / 2), static_cast<std::int32_t>((monitor_height - window_height) / 2));

    // Set window's default cursor mode
    //m_window->SetCursor(CursorMode::CrossHair);
  } catch (const std::exception& e) {
    const std::string err_msg = "Couldn't Construct Window: " + std::string(e.what());
    // console alert
    ENIGMA_CRITICAL(err_msg);
    // ui alert
    (void) DialogUtils::Error(err_msg);
    // exit
    this->EndApplication(); // No Application without a window :c
  }
}

void Application::InitImGuiRenderer() {
  m_imgui_renderer = std::make_unique<ImGuiRenderer>();
}

void Application::InitHardwareInfo(const WindowSettings& window_settings) {
  if (window_settings.is_show_fps)
    m_FPS = std::make_unique<std::uint32_t>(0);
  if (window_settings.is_show_ram_usage)
    m_ram_info = std::make_unique<RAMInfo>();
  if (window_settings.is_show_cpu_usage)
    m_cpu_info = std::make_unique<CPUInfo>();
}


void Application::InitImGuiFonts() {
  ENIGMA_TRACE_CURRENT_FUNCTION();

  ENIGMA_TRACE("Loading Fonts...");

  const auto& io = ImGui::GetIO();

  const fs::path font_audiowide_path = Constants::Resources::Fonts::AUDIOWIDE_FONT_PATH;
  const fs::path font_montserrat_path = Constants::Resources::Fonts::MONTSERRAT_FONT_PATH;
  const fs::path font_ubuntu_path = Constants::Resources::Fonts::UBUNTU_FONT_PATH;
  ENIGMA_ASSERT_OR_THROW(fs::exists(font_audiowide_path), "Font " + font_audiowide_path.string() + " not found");
  ENIGMA_ASSERT_OR_THROW(fs::exists(font_montserrat_path), "Font " + font_montserrat_path.string() + " not found");
  ENIGMA_ASSERT_OR_THROW(fs::exists(font_ubuntu_path), "Font " + font_ubuntu_path.string() + " not found");

#ifdef ENIGMA_DEBUG
  ENIGMA_LOG("Font Audiowide-Regular path: {}", font_audiowide_path.string());
  ENIGMA_LOG("Font Montserrat-Medium path: {}", font_montserrat_path.string());
  ENIGMA_LOG("Font Ubuntu-Regular path: {}", font_ubuntu_path.string());
#endif

  //using namespace std::string_literals;
  //for(auto font_audiowide_size : std::initializer_list<short>{ 20, 30, 45, 60 })
  //	m_fonts["Audiowide-Regular-"sv + std::to_string(font_audiowide_size)] =  io.Fonts->AddFontFromFileTTF(font_audiowide_path.string().c_str(), static_cast<float>(font_audiowide_size));
  //for (auto font_montserrat_size : std::initializer_list<short>{ 12, 14, 16, 18, 20, 45 })
  //	m_fonts["Montserrat-Medium-"sv + std::to_string(font_montserrat_size)] = io.Fonts->AddFontFromFileTTF(font_montserrat_path.string().c_str(), static_cast<float>(font_montserrat_size));

  ImFontConfig fontConfig = ImFontConfig();
  //fontConfig.FontDataOwnedByAtlas = true;

  m_fonts["Audiowide-Regular-60"] = io.Fonts->AddFontFromFileTTF(font_audiowide_path.string().c_str(), 60.0f, &fontConfig);
  m_fonts["Audiowide-Regular-45"] = io.Fonts->AddFontFromFileTTF(font_audiowide_path.string().c_str(), 45.0f, &fontConfig);
  m_fonts["Audiowide-Regular-30"] = io.Fonts->AddFontFromFileTTF(font_audiowide_path.string().c_str(), 30.0f, &fontConfig);
  m_fonts["Audiowide-Regular-20"] = io.Fonts->AddFontFromFileTTF(font_audiowide_path.string().c_str(), 20.0f, &fontConfig);

  m_fonts["Ubuntu-Regular-60"] = io.Fonts->AddFontFromFileTTF(font_ubuntu_path.string().c_str(), 60.0f, &fontConfig);
  m_fonts["Ubuntu-Regular-45"] = io.Fonts->AddFontFromFileTTF(font_ubuntu_path.string().c_str(), 45.0f, &fontConfig);
  m_fonts["Ubuntu-Regular-30"] = io.Fonts->AddFontFromFileTTF(font_ubuntu_path.string().c_str(), 30.0f, &fontConfig);
  m_fonts["Ubuntu-Regular-20"] = io.Fonts->AddFontFromFileTTF(font_ubuntu_path.string().c_str(), 20.0f, &fontConfig);
  m_fonts["Ubuntu-Regular-18"] = io.Fonts->AddFontFromFileTTF(font_ubuntu_path.string().c_str(), 18.0f, &fontConfig);

  m_fonts["Montserrat-Medium-45"] = io.Fonts->AddFontFromFileTTF(font_montserrat_path.string().c_str(), 45.0f, &fontConfig);
  m_fonts["Montserrat-Medium-20"] = io.Fonts->AddFontFromFileTTF(font_montserrat_path.string().c_str(), 20.0f, &fontConfig);
  m_fonts["Montserrat-Medium-18"] = io.Fonts->AddFontFromFileTTF(font_montserrat_path.string().c_str(), 18.0f, &fontConfig);
  m_fonts["Montserrat-Medium-16"] = io.Fonts->AddFontFromFileTTF(font_montserrat_path.string().c_str(), 16.0f, &fontConfig);
  m_fonts["Montserrat-Medium-14"] = io.Fonts->AddFontFromFileTTF(font_montserrat_path.string().c_str(), 14.0f, &fontConfig);
  m_fonts["Montserrat-Medium-12"] = io.Fonts->AddFontFromFileTTF(font_montserrat_path.string().c_str(), 12.0f, &fontConfig);

  // Build added fonts atlas --> imgui issue #3643
  io.Fonts->Build();

  // Check if fonts are loaded
  for (const auto& [font_name, font]: m_fonts) {
    if (font->IsLoaded()) {
      ENIGMA_TRACE("Font {} Loaded", font->ConfigData->Name);
    } else {
      const std::string err_msg = "Failed to load font " + std::string(font_name);
      // console alert
      ENIGMA_ERROR(err_msg);
      // ui alert
      (void) DialogUtils::Error("Resource Loading Error", err_msg);
      // no further app without dear fonts :c
      this->EndApplication();
      break;
    }
  }
}

void Application::PushScene(std::unique_ptr<Scene> scene) {
  ENIGMA_ASSERT(scene, "Scene is nullptr");

  // Push scene & Notify user on scene created
  this->m_scenes.emplace_back(std::move(scene));

  // Notify user on scene created
  this->m_scenes.back()->OnCreate();
}


void Application::LaunchWorkerThread(Scene *scene, const std::string& loading_text, const std::function<void()>& work_func) {
  ENIGMA_ASSERT(scene, "Scene is nullptr!");
  ENIGMA_ASSERT(work_func, "Work function is empty!");
  m_threadPool->Submit([scene, loading_text, work_func, this]() -> void {
    ENIGMA_LOG("Launching Worker Thread ID #{} To Do Work: {}", std::this_thread::get_id(), loading_text);
    dynamic_cast<LoadingScene&>(*this->m_loading_scene).SetLoadingText(loading_text); // set loading status description text will appear bellow loading spinner
    scene->SetLoading(true);
    {
      std::lock_guard<std::mutex> guard{scene->GetMutex()};
      work_func();
    }

    scene->SetLoading(false);
    dynamic_cast<LoadingScene&>(*this->m_loading_scene).SetLoadingText(""); // reset loading text
    ENIGMA_LOG("Finished Worker Thread ID #{}", std::this_thread::get_id());
  });
#if 0
	std::thread worker_thread([this, scene, loading_text, work_func]() -> void //Note: Don't use [&] e.g [&func] when the object or copies of it outlives the current scope. You are capturing references to local variables and storing them beyond the current scope. https://stackoverflow.com/questions/46489068/access-violation-on-stdfunction-assigned-to-a-lambda
		{
			std::scoped_lock<std::mutex> guard{ scene->GetMutex() };

			ENIGMA_LOG("Launching Worker Thread ID #{} To Do Work: {}", std::this_thread::get_id(), loading_text);

			dynamic_cast<LoadingScene&>(*this->m_loading_scene).SetLoadingText(loading_text); // set loading status description text will appear bellow loading spinner
			scene->SetLoading(true);
			work_func();
			scene->SetLoading(false);
			dynamic_cast<LoadingScene&>(*this->m_loading_scene).SetLoadingText(""); // reset loading text

			ENIGMA_LOG("Finished Worker Thread ID #{}", std::this_thread::get_id());
		});
    worker_thread.detach();
#endif
}


void Application::OnEvent(Event& event) {
  // Listen for WindowClose, WindowResize and FrameBufferResizeEvent Events
  EventDispatcher dispatcher(event);
  /*dispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent& e) -> bool
	{
	});*/
  dispatcher.Dispatch<WindowCloseEvent>(ENIGMA_BIND_FUN(Application::OnWindowClose));
  dispatcher.Dispatch<WindowResizeEvent>(ENIGMA_BIND_FUN(Application::OnWindowResize));
  dispatcher.Dispatch<FrameBufferResizeEvent>(ENIGMA_BIND_FUN(Application::OnFrameBufferResize));

  // Alert All Scenes OnEvent
  for (auto it = m_scenes.rbegin(); it != m_scenes.rend(); ++it) {
    if (!(*it)->IsLoading()) // Only pass events to scenes which are not in progress of doing some work. to disable buttons clicking interactions...
    {
      // Alert each scene with the triggered event
      (*it)->OnEvent(event);

      // if event is handled, stop passing it to other scenes
      if (event.IsHandled())
        break;
    }
  }
}

bool Application::OnWindowClose(WindowCloseEvent& /*event*/) {
  ENIGMA_TRACE_CURRENT_FUNCTION();

  // check if there is a scene doing some work in progress...
  const bool there_is_a_scene_still_doing_some_work = // long name.. i know :'D
    std::any_of(m_scenes.rbegin(), m_scenes.rend(),
                [](const auto& scene) {
                  return scene->IsLoading();
                });

  if (there_is_a_scene_still_doing_some_work) {
    (void) DialogUtils::Warn("Warning!", "A Scene is still doing some work, please wait...");
    m_window->SetShouldClose(false); // force GLFW to keep window open. GLFW will close the window when a close window event received.
    return true;                     // handled.
  } else {
#ifdef ENIGMA_DEBUG
    ENIGMA_INFO("Closing Window due WindowCloseEvent");
#endif
    this->EndApplication();
    return true; // the end of the app, all events are handled.
  }
}

bool Application::OnWindowResize(WindowResizeEvent& event) {
  ENIGMA_TRACE_CURRENT_FUNCTION();

#ifdef ENIGMA_DEBUG
  ENIGMA_INFO(event.ToString());
#endif

  // Update OpenGL Viewport
  glAssert(glViewport(0, 0, event.GetWidth(), event.GetHeight()));

  return false;
}


bool Application::OnFrameBufferResize(FrameBufferResizeEvent& event) {
  ENIGMA_TRACE_CURRENT_FUNCTION();

#ifdef ENIGMA_DEBUG
  ENIGMA_INFO(event.ToString());
#endif

  // Update OpenGL Viewport
  glAssert(glViewport(0, 0, event.GetWidth(), event.GetHeight()));

  return false;
}


void Application::Run() {
  if (!m_window) return;
  while (!m_window->ShouldClose()) {
    // Poll Events (no need to poll events, since glfwWaitEvents() above will do this for us)
    //m_window->PollEvents();

    // Poll Events
    if (std::any_of(m_scenes.rbegin(), m_scenes.rend(), [](const auto& scene) noexcept { return scene->IsLoading(); })) // if there is a scene still doing some work (loading..)
      m_window->PollEvents();                                                                                           // Don't wait for input events so loading animation will keep updating
    else
      glfwWaitEvents(); // Waits until events are queued and then processes them (Polls events). (Reduces CPU usage)


    // Update & Draw (only if there are scenes, otherwise end app).
    if (!m_scenes.empty()) {
      //Update
      {
        // Delta time
        UpdateDeltaTime();
        // Hardware Info (FPS & CPU & RAM usage)
        UpdateHardwareInfo();
        // Update back scene (last pushed scene which is the active one)
        m_scenes.back()->OnUpdate(m_delta_time);
        if (m_scenes.back()->IsLoading()) {
          m_loading_scene->OnUpdate(m_delta_time); // Update Loading scene over current active scene if its loading (doing some work in parallel)
        }
      }

      //Draw
      {
        // Draw back scene (last pushed scene which is the active one)
        m_scenes.back()->OnDraw();
        if (m_scenes.back()->IsLoading()) {
          m_loading_scene->OnDraw(); // Draw Loading scene over current active scene if its loading (doing some work in parallel)
        }

        // ImGui
        m_imgui_renderer->Begin();
        m_scenes.back()->OnImGuiDraw();
        if (m_scenes.back()->IsLoading()) {
          m_loading_scene->OnImGuiDraw(); // Draw GUI Loading scene over current active scene if its loading (doing some work in parallel)
        }
        m_imgui_renderer->End();

        // Force execution of GL commands in finite time
        glAssert(glFlush());
      }

      // Swap Buffers
      m_window->SwapBuffers();

      // Check if the current active scene wants to quit (scene must call EndScene to be destroyed)
      if (m_scenes.back()->WantsToQuit()) {
        // Notify user before ending scene
        m_scenes.back()->OnDestroy();
        // Destroy Scene
        m_scenes.pop_back(); // Remove scene from vector (btw vector will call ~unique_ptr to cleanup memory)
      }

    } else // No scenes ? end App.
    {
      this->EndApplication();
    }
  }
}


void Application::Exit(const std::string& message, const std::int32_t exit_code) noexcept {
  const std::string msg = "Enigma Application has exited with code " + std::to_string(exit_code) + " (" + message + ")\n";

  if (Logger::getLogger())
    ENIGMA_CRITICAL(msg);
  else
    std::cerr << msg << std::endl;

  std::exit(exit_code);
}


void Application::UpdateDeltaTime() noexcept {
  m_current_frame_time = static_cast<float>(glfwGetTime());
  m_delta_time = m_current_frame_time - m_last_frame_time;
  m_last_frame_time = m_current_frame_time;
}


void Application::UpdateHardwareInfo() noexcept {
  if (m_FPS || m_ram_info || m_cpu_info) {
    // update timer for anyone enabled
    m_hardware_info_timer += m_delta_time;

    // update fps if enabled
    if (m_FPS)
      (*m_FPS)++;

    // see if we waited HARWARE_INFO_UPDATE_TIME seconds for next hardware info update...
    if (m_hardware_info_timer >= HARWARE_INFO_UPDATE_TIME) {

      // ... if so, refresh to show new hardware info data
      m_window->SetTitle(m_window->GetTitle(), m_FPS, m_ram_info, m_cpu_info);

      // reset fps if enabled
      if (m_FPS)
        *m_FPS = 0;

      // reset hardware update timer
      m_hardware_info_timer = 0.0f;
    }
  }
#if 0
	if (m_window->m_is_show_fps || m_window->m_is_show_ram_usage || m_window->m_is_show_cpu_usage)
	{
		// update timer
		m_hardware_info_timer += m_delta_time;

		// update fps if enabled
		if (m_window->m_is_show_fps)
			(*m_FPS)++;

		// see if we waited HARWARE_INFO_UPDATE_TIME seconds for next hardware info update
		if (m_hardware_info_timer >= HARWARE_INFO_UPDATE_TIME)
		{
			// if so, refresh title to show new hardware info data
			m_window->SetTitle(m_window->GetTitle()); // refresh title
			// reset fps
			m_FPS = 0;
			// reset timer
			m_hardware_info_timer = 0.0f;
		}
	}
#endif
}


void Application::EndApplication() noexcept {
  if (m_window) {
    m_window->SetShouldClose(true);
  }
  if (m_threadPool) {
    m_threadPool->Stop();
  }
}


Application::~Application() {
  EndApplication(); // to be sure

  // Alert scenes OnDestroy()
  std::for_each(m_scenes.rbegin(), m_scenes.rend(), [](const std::unique_ptr<Scene>& scene) {
    // Notify scenes OnDestroy before closing application
    scene->OnDestroy();
  });

  if (m_loading_scene)
    m_loading_scene->OnDestroy(); // Don't forget the loading scene

  m_scenes.clear();
  m_fonts.clear();

  ImGui::GetIO().Fonts->Clear();
}
NS_ENIGMA_END
