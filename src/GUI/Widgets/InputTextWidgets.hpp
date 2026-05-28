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

class InputTextWidgets final {
  ENIGMA_STATIC_CLASS(InputTextWidgets);

  struct InputTextCallback_UserData {
    std::string *Str;
    ImGuiInputTextCallback ChainCallback;
    void *ChainCallbackUserData;
  };

public:
  /**
    *	InputTextMultiline with stl container std::string instread of char* static size
    *
    *	@note: https://github.com/ocornut/imgui/issues/2035, https://github.com/ocornut/imgui/blob/master/misc/cpp/imgui_stdlib.cpp
    */

  static int InputTextCallback(ImGuiInputTextCallbackData *data);

  /** ImGui InputText with dynamic std::string */
  static bool InputText(const char *label, std::string *str, const float width, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = nullptr, void *user_data = nullptr);

  /** ImGui InputTextMultiline with dynamic std::string */
  static bool InputTextMultiline(const char *label, std::string *str, const ImVec2 &size, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = nullptr, void *user_data = nullptr);

  /** ImGui InputTextWithHint with dynamic std::string */
  static bool InputTextWithHint(const char *label, const char *hint, std::string *str, const float width, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = nullptr, void *user_data = nullptr);
};

NS_ENIGMA_END
