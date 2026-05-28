#include <pch.hpp>
#include "ImGuiRenderer.hpp"

//ImGui
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

#include <Application/Application.hpp>
#include <GLFW/glfw3.h>
#include <glad/glad.h>

NS_ENIGMA_BEGIN
static void ApplyEnigmaTheme()
{
  ImGuiStyle& style = ImGui::GetStyle();

  style.WindowRounding    = 12.0f;
  style.FrameRounding     = 10.0f;
  style.PopupRounding     = 10.0f;
  style.ScrollbarRounding = 12.0f;
  style.GrabRounding      = 10.0f;
  style.TabRounding       = 10.0f;

#if 0
  style.WindowPadding = ImVec2(16, 16);
  style.FramePadding  = ImVec2(14, 12);
  style.ItemSpacing   = ImVec2(12, 12);

  ImVec4* colors = style.Colors;

  colors[ImGuiCol_WindowBg]           = ImVec4(0.07f, 0.08f, 0.10f, 1.00f);

  colors[ImGuiCol_Header]             = ImVec4(0.12f, 0.45f, 0.55f, 1.00f);
  colors[ImGuiCol_HeaderHovered]      = ImVec4(0.16f, 0.55f, 0.67f, 1.00f);
  colors[ImGuiCol_HeaderActive]       = ImVec4(0.10f, 0.65f, 0.80f, 1.00f);

  colors[ImGuiCol_Button]             = ImVec4(0.10f, 0.48f, 0.55f, 1.00f);
  colors[ImGuiCol_ButtonHovered]      = ImVec4(0.16f, 0.60f, 0.70f, 1.00f);
  colors[ImGuiCol_ButtonActive]       = ImVec4(0.10f, 0.72f, 0.85f, 1.00f);

  colors[ImGuiCol_FrameBg]            = ImVec4(0.11f, 0.13f, 0.16f, 1.00f);

  colors[ImGuiCol_TitleBg]            = ImVec4(0.06f, 0.07f, 0.09f, 1.00f);
  colors[ImGuiCol_TitleBgActive]      = ImVec4(0.06f, 0.07f, 0.09f, 1.00f);

  colors[ImGuiCol_Border]             = ImVec4(0.18f, 0.20f, 0.24f, 1.00f);

  colors[ImGuiCol_Separator]          = ImVec4(0.20f, 0.24f, 0.28f, 1.00f);

  colors[ImGuiCol_Text]               = ImVec4(0.92f, 0.94f, 0.96f, 1.00f);

  colors[ImGuiCol_MenuBarBg]          = ImVec4(0.08f, 0.09f, 0.12f, 1.00f);
#endif
}

ImGuiRenderer::ImGuiRenderer() {
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange; //Prevent ImGui from changing cursor for GLFW
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
  io.IniFilename = nullptr;                               //Disable .ini file

  // for docking branch
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
  //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
  //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
  //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
  //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;


  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  //ImGui::StyleColorsLight();
  //ImGui::StyleColorsClassic();


  // for docking branch
  //// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
  //ImGuiStyle& style = ImGui::GetStyle();
  //if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
  //{
  //    style.WindowRounding = 0.0f;
  //    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  //}


  // Set imgui gl context version (to the same one used by window)
  const auto& app = *Application::getInstance();
  GLFWwindow *window = static_cast<GLFWwindow *>(app.GetWindow()->GetGLFWwindow());
  std::ostringstream version{};
  version << "#version " << ENIGMA_GL_VERSION_MAJOR << ENIGMA_GL_VERSION_MINOR << '0';
  if (!ImGui_ImplGlfw_InitForOpenGL(window, true)) {
    ENIGMA_ERROR("ImGui init for GLFW failed.");
  }
  if (!ImGui_ImplOpenGL3_Init(version.str().c_str())) {
    ENIGMA_ERROR("ImGui init for OpenGL failed.");
  }

  //ApplyEnigmaTheme();
}


void ImGuiRenderer::Begin() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}


void ImGuiRenderer::End() {
  auto& io = ImGui::GetIO();
  const auto& app = *Application::getInstance();
  io.DisplaySize = ImVec2(
    static_cast<float>(app.GetWindow()->GetWidth()),
    static_cast<float>(app.GetWindow()->GetHeight()));

  // Rendering
  // ImGui::EndFrame(); is called by ImGui::Render() bellow
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


  // for docking branch
  //if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
  //{
  //    GLFWwindow* backup_current_context = glfwGetCurrentContext();
  //    ImGui::UpdatePlatformWindows();
  //    ImGui::RenderPlatformWindowsDefault();
  //    glfwMakeContextCurrent(backup_current_context);
  //}
}


ImGuiRenderer::~ImGuiRenderer() {
  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

NS_ENIGMA_END
