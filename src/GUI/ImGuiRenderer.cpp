#include <pch.hpp>
#include "ImGuiRenderer.hpp"

//ImGui
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Application/Application.hpp>

NS_ENIGMA_BEGIN

ImGuiRenderer::ImGuiRenderer()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;     //Prevent ImGui from changing cursor for GLFW	
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.IniFilename = nullptr; //Disable .ini file

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
    const auto& app = *Application::GetInstance();
    GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow()->GetGLFWwindow());
		std::ostringstream version{};
    version << "#version " << ENIGMA_GL_VERSION_MAJOR << ENIGMA_GL_VERSION_MINOR << '0';
    if (!ImGui_ImplGlfw_InitForOpenGL(window, true))
    {
        ENIGMA_ERROR("ImGui init for GLFW failed.");
    }
    if (!ImGui_ImplOpenGL3_Init(version.str().c_str()))
    {
        ENIGMA_ERROR("ImGui init for OpenGL failed.");
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
  auto& io = ImGui::GetIO();
  const auto& app = *Application::GetInstance();
  io.DisplaySize = ImVec2(
      static_cast<f32>(app.GetWindow()->GetWidth()),
      static_cast<f32>(app.GetWindow()->GetHeight())
  );

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


ImGuiRenderer::~ImGuiRenderer()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

NS_ENIGMA_END

