//
// Created by bader on 5/28/26.
//
#pragma once

#include <Core/Core.hpp>
#include <imgui.h>
#include <imgui_internal.h>

NS_ENIGMA_BEGIN

class ButtonWidgets final {
  ENIGMA_STATIC_CLASS(ButtonWidgets);

public:
  /** ImGui Button with default color, hover color, active color */
  static bool Button(const char* text,
                     const ImVec2& size = ImVec2(0.0f, 0.0f),
                     const ImVec4& color = Constants::Colors::BUTTON_COLOR,
                     const ImVec4& color_hover = Constants::Colors::BUTTON_COLOR_HOVER,
                     const ImVec4& color_active = Constants::Colors::BUTTON_COLOR_ACTIVE);

  /** ImGui BackButton with default color, hover color, active color */
  static bool BackButton(const char* str_id,
                         const ImVec2& size = ImVec2(0.0f, 0.0f),
                         const ImVec4& color = Constants::Colors::BACK_BUTTON_COLOR,
                         const ImVec4& color_hover = Constants::Colors::BACK_BUTTON_COLOR_HOVER,
                         const ImVec4& color_active = Constants::Colors::BACK_BUTTON_COLOR_ACTIVE);
};

NS_ENIGMA_END