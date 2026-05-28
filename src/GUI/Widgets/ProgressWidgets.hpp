//
// Created by bader on 5/28/26.
//
#pragma once

#include <Application/Application.hpp>
#include <ResourceManager/ResourceManager.hpp>
#include <Core/Core.hpp>
#include <Utility/OpenGLUtils.hpp>
#include <Utility/SizeUtils.hpp>
#include <imgui.h>
#include <imgui_internal.h>

NS_ENIGMA_BEGIN

class ProgressWidgets final {
  ENIGMA_STATIC_CLASS(ProgressWidgets);

public:
  /**
	*	ImGui Loading spinner
	*
	*	@note: https://github.com/ocornut/imgui/issues/1901
	*/
  static bool LoadingSpinner(const char* label, float radius, float thickness, const ImU32 color);
  /**
	*	ImGui Loading bar
	*
	*	@note: https://github.com/ocornut/imgui/issues/1901
	*/
  static bool LoadingBar(const char* label, float value, const ImVec2& size_arg, const ImU32 bg_col, const ImU32 fg_col);

  /**
	*	Popup dialog has spinner loading inside it, displayed when a worker thread is launched
	*	to prevent interactions with the scene widgets until the thread is finished.
	*
	*	@note: https://github.com/ocornut/imgui/issues/249
	*/
  static void LoadingDialog(const char* text, const ImVec2& spinner_position, const float spinner_radius, const float spinner_thickness, const ImVec4& spinner_color,
                            const float container_width = (float) Application::getInstance()->GetWindow()->GetWidth(),
                            const float container_height = (float) Application::getInstance()->GetWindow()->GetHeight());
};
NS_ENIGMA_END