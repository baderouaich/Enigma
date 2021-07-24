#pragma once
#ifndef ENIGMA_IMGUI_RENDERER_H
#define ENIGMA_IMGUI_RENDERER_H

#include <Core/Core.hpp>

NS_ENIGMA_BEGIN
/*Refs: examples/example_glfw_opengl3/main.cpp*/
class ENIGMA_API ImGuiRenderer

{
public:
	ImGuiRenderer();
	~ImGuiRenderer();

	//Begin imgui rendering
	void Begin();

	//End imgui rendering
	void End();
};

NS_ENIGMA_END

#endif // !ENIGMA_IMGUI_RENDERER_H
