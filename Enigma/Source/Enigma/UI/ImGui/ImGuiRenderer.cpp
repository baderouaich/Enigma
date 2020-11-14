#include <Epch.hpp>
#include "ImGuiRenderer.hpp"

//ImGui
#include <imgui.h>
#include <examples/imgui_impl_opengl3.h>
#include <examples/imgui_impl_glfw.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

NS_ENIGMA_BEGIN

ImGuiRenderer::ImGuiRenderer()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;     //Prevent ImGui from changing cursor for GLFW	
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;
    //io.IniFilename = nullptr; //Disable .ini file
   

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();
    //ImGui::StyleColorsClassic();


    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }


    // Setup Platform/Renderer bindings
    Application& app = Application::GetInstance();
    GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow()->GetGLFWwindow());
    auto [major, minor] = app.GetWindow()->GetOpenGLVersion();
    std::stringstream version;
    version << "#version " << major << minor << "0";
    if (!ImGui_ImplGlfw_InitForOpenGL(window, true))
    {
        ENIGMA_CORE_ERROR("ImGui init for GLFW failed.");
    }
    if (!ImGui_ImplOpenGL3_Init(version.str().c_str()))
    {
        ENIGMA_CORE_ERROR("ImGui init for OpenGL failed.");
    }
}






void ImGuiRenderer::Begin()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}



void ImGuiRenderer::End()
{  
  ImGuiIO& io = ImGui::GetIO();
  Application& app = Application::GetInstance();
  io.DisplaySize = ImVec2(
      static_cast<float>(app.GetWindow()->GetWidth()),
      static_cast<float>(app.GetWindow()->GetHeight())
  );

  // Rendering
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
  {
      GLFWwindow* backup_current_context = glfwGetCurrentContext();
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      glfwMakeContextCurrent(backup_current_context);
  }
}


ImGuiRenderer::~ImGuiRenderer()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

NS_ENIGMA_END

