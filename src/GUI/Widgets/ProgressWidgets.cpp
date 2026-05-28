//
// Created by bader on 5/28/26.
//

#include "ProgressWidgets.hpp"
using namespace Enigma;

bool ProgressWidgets::LoadingSpinner(const char* label, float radius, float thickness, const ImU32 color) {
  ImGuiWindow* window = ImGui::GetCurrentWindow();
  if (window->SkipItems)
    return false;

  ImGuiContext& g = *GImGui;
  const ImGuiStyle& style = g.Style;
  const ImGuiID id = window->GetID(label);

  ImVec2 pos = window->DC.CursorPos;
  ImVec2 size((radius) * 2, (radius + style.FramePadding.y) * 2);

  const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
  ImGui::ItemSize(bb, style.FramePadding.y);
  if (!ImGui::ItemAdd(bb, id))
    return false;

  // Render
  window->DrawList->PathClear();

  int num_segments = 30;
  int start = (int) std::abs(sinf((float) g.Time * 1.8f) * (num_segments - 5));

  const float a_min = IM_PI * 2.0f * ((float) start) / (float) num_segments;
  const float a_max = IM_PI * 2.0f * ((float) num_segments - 3) / (float) num_segments;

  const ImVec2 centre = ImVec2(pos.x + radius, pos.y + radius + style.FramePadding.y);

  for (int i = 0; i < num_segments; i++) {
    const float a = a_min + ((float) i / (float) num_segments) * (a_max - a_min);
    window->DrawList->PathLineTo(ImVec2(centre.x + cosf(a + (float) g.Time * 8.0f) * radius,
                                        centre.y + sinf(a + (float) g.Time * 8.0f) * radius));
  }

  window->DrawList->PathStroke(color, false, thickness);

  return true;
}
bool ProgressWidgets::LoadingBar(const char* label, float value, const ImVec2& size_arg, const ImU32 bg_col, const ImU32 fg_col) {
  ImGuiWindow* window = ImGui::GetCurrentWindow();
  if (window->SkipItems)
    return false;

  ImGuiContext& g = *GImGui;
  const ImGuiStyle& style = g.Style;
  const ImGuiID id = window->GetID(label);

  ImVec2 pos = window->DC.CursorPos;
  ImVec2 size = size_arg;
  size.x -= style.FramePadding.x * 2;

  const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
  ImGui::ItemSize(bb, style.FramePadding.y);
  if (!ImGui::ItemAdd(bb, id))
    return false;

  // Render
  const float circleStart = size.x * 0.7f;
  const float circleEnd = size.x;
  const float circleWidth = circleEnd - circleStart;

  window->DrawList->AddRectFilled(bb.Min, ImVec2(pos.x + circleStart, bb.Max.y), bg_col);
  window->DrawList->AddRectFilled(bb.Min, ImVec2(pos.x + circleStart * value, bb.Max.y), fg_col);

  const float t = static_cast<float>(g.Time);
  const float r = size.y / 2;
  const float speed = 1.5f;

  const float a = speed * 0;
  const float b = speed * 0.333f;
  const float c = speed * 0.666f;

  const float o1 = (circleWidth + r) * (t + a - speed * (int) ((t + a) / speed)) / speed;
  const float o2 = (circleWidth + r) * (t + b - speed * (int) ((t + b) / speed)) / speed;
  const float o3 = (circleWidth + r) * (t + c - speed * (int) ((t + c) / speed)) / speed;

  window->DrawList->AddCircleFilled(ImVec2(pos.x + circleEnd - o1, bb.Min.y + r), r, bg_col);
  window->DrawList->AddCircleFilled(ImVec2(pos.x + circleEnd - o2, bb.Min.y + r), r, bg_col);
  window->DrawList->AddCircleFilled(ImVec2(pos.x + circleEnd - o3, bb.Min.y + r), r, bg_col);

  return true;
}
void ProgressWidgets::LoadingDialog(const char* text, const ImVec2& spinner_position, const float spinner_radius, const float spinner_thickness, const ImVec4& spinner_color, const float container_width, const float container_height) {
  static constexpr auto popup_id = "loadingDialog";

  const auto& [win_w, win_h] = Application::getInstance()->GetWindow()->GetSize();
  static ImFont* const& font_ubuntu_regular_20 = ResourceManager::getFont("Ubuntu-Regular-20");

  ImGui::OpenPopup(popup_id, ImGuiPopupFlags_AnyPopup);
  static constexpr const auto popup_window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground;
  if (ImGui::BeginPopupModal(popup_id, nullptr, popup_window_flags)) {
    ImGui::SetWindowSize(ImVec2(container_width, container_height)); // set spinner container's size same size as window (to centerize it)
    ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));                         // top left

    static const std::uint32_t COLOR = ImGui::GetColorU32(spinner_color); // 0xrrggbbaa
    ImGui::SetCursorPos(spinner_position);
    ProgressWidgets::LoadingSpinner("##spinner", spinner_radius, spinner_thickness, COLOR);
    ImGui::PushFont(font_ubuntu_regular_20); // text font
    const auto text_size = ImGui::CalcTextSize(text);
    ImGui::SetCursorPosX((win_w - text_size.x) / 2.0f);
    ImGui::TextWrapped("%s", text);
    ImGui::PopFont();
    ImGui::EndPopup();
  }
}
