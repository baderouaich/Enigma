//
// Created by bader on 5/28/26.
//

#include "ButtonWidgets.hpp"

using namespace Enigma;

bool ButtonWidgets::Button(const char *text, const ImVec2 &size, const ImVec4 &color, const ImVec4 &color_hover, const ImVec4 &color_active) {
  ImGui::PushStyleColor(ImGuiCol_Button, color);              // buttons color idle
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color_hover); // buttons color hover
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, color_active); // buttons color pressed
  bool pressed_or_selected = ImGui::Button(text, size);
  ImGui::PopStyleColor(3);

  return pressed_or_selected;
}
bool ButtonWidgets::BackButton(const char *str_id, const ImVec2 &size, const ImVec4 &color, const ImVec4 &color_hover, const ImVec4 &color_active) {
  ImGui::PushStyleColor(ImGuiCol_Button, color);              // buttons color idle
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color_hover); // buttons color hover
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, color_active); // buttons color pressed
  bool pressed_or_selected = ImGui::ArrowButtonEx(str_id, ImGuiDir_Left, size);
  ImGui::PopStyleColor(3);

  return pressed_or_selected;
}