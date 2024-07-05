#pragma once
#ifndef ENIGMA_IMGUI_RENDERER_H
#define ENIGMA_IMGUI_RENDERER_H

#include <Core/Core.hpp>

NS_ENIGMA_BEGIN
/** 
*	ImGui Renderer
* 
*	@note: see examples/example_glfw_opengl3/main.cpp/
*/
class ImGuiRenderer {
  public:
    /** Constructor
    *	Initializes ImGui context
    */
    ImGuiRenderer();

    /** Destructor
    *	Shuts down ImGui context
    */
    ~ImGuiRenderer();

    /** Begin imgui rendering */
    void Begin();

    /** End imgui rendering */
    void End();
};

NS_ENIGMA_END

#endif // !ENIGMA_IMGUI_RENDERER_H
