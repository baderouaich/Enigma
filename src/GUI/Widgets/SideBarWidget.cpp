//
// Created by bader on 5/28/26.
//

#include "SideBarWidget.hpp"
using namespace Enigma;


SideBarWidget::SideBarWidget(Scene *parent)  : m_parent(parent) {
  ENIGMA_ASSERT(parent != nullptr, "Parent shouldn't be nullptr");
}
void SideBarWidget::setItems(std::initializer_list<Item> &&items) {
  ENIGMA_ASSERT(items.size() != 0, "Items shouldn't be empty");
  m_items = std::move(items);

  if (m_selectedIndex >= m_items.size())
    m_selectedIndex = 0;
}
void SideBarWidget::setOnItemSelected(std::function<void(std::size_t)> &&callback) {
  m_onItemSelected = std::move(callback);
}

void SideBarWidget::drawSideBarItem(std::size_t index)
{
  const Item& item = m_items[index];

  ENIGMA_ASSERT(item.icon != nullptr, "Sidebar item icon shouldn't be nullptr");

   constexpr float BUTTON_SIZE = 55.0f;
  constexpr float ICON_SIZE   = 24.0f;
  constexpr float ROUNDING    = 10.0f;

  ImGui::PushID(static_cast<int>(index));

  const bool selected = (m_selectedIndex == index);

  // Center horizontally in sidebar
  const float availableWidth =
      ImGui::GetContentRegionAvail().x;

  const float offsetX =
      (availableWidth - BUTTON_SIZE) * 0.5f;

  ImGui::SetCursorPosX(
      ImGui::GetCursorPosX() + offsetX);

  ImVec2 cursor = ImGui::GetCursorScreenPos();

  ImGui::InvisibleButton(
      "##sidebar_btn",
      ImVec2(BUTTON_SIZE, BUTTON_SIZE));

  const bool hovered = ImGui::IsItemHovered();
  const bool clicked = ImGui::IsItemClicked();

  if (clicked)
  {
    if (m_selectedIndex != index) {
      m_selectedIndex = index;

      if (m_onItemSelected)
        m_onItemSelected(index);
    }
  }
  ImDrawList* drawList =
      ImGui::GetWindowDrawList();

  ImU32 bgColor;

  if (selected)
  {
    bgColor = IM_COL32(72, 96, 180, 255);
  }
  else if (hovered)
  {
    bgColor = IM_COL32(60, 60, 68, 255);
  }
  else
  {
    bgColor = IM_COL32(48, 48, 56, 160);
  }

  // Button background
  drawList->AddRectFilled(
      cursor,
      ImVec2(cursor.x + BUTTON_SIZE,
             cursor.y + BUTTON_SIZE),
      bgColor,
      ROUNDING);

  // Selected indicator
  if (selected)
  {
    drawList->AddRectFilled(
        ImVec2(cursor.x - 10.0f,
               cursor.y + 10.0f),

        ImVec2(cursor.x - 4.0f,
               cursor.y + BUTTON_SIZE - 10.0f),

        IM_COL32(120, 180, 255, 255),
        8.0f);
  }

  // Icon centered
  const float iconX =
      cursor.x + (BUTTON_SIZE - ICON_SIZE) * 0.5f;

  const float iconY =
      cursor.y + (BUTTON_SIZE - ICON_SIZE) * 0.5f;

  drawList->AddImage(
      reinterpret_cast<void*>(
          static_cast<intptr_t>(item.icon->GetID())),

      ImVec2(iconX, iconY),

      ImVec2(iconX + ICON_SIZE,
             iconY + ICON_SIZE));

  // Hover tooltip
  if (hovered)
  {
    ImGui::BeginTooltip();

    ImGui::PushTextWrapPos(300.0f);
    ImGui::TextUnformatted(item.title.c_str());
    ImGui::PopTextWrapPos();

    ImGui::EndTooltip();
  }

  // Vertical spacing between items
  ImGui::Dummy(ImVec2(0.0f, 5.0f));

  ImGui::PopID();

}
#if 0
void SideBarWidget::drawSideBarItem(std::size_t index)
{
  const Item& item = m_items[index];

  ENIGMA_ASSERT(item.icon != nullptr, "Sidebar item icon shouldn't be nullptr");

  constexpr float ITEM_SIZE   = 64.0f;
  constexpr float ICON_SIZE   = 28.0f;
  constexpr float ROUNDING    = 14.0f;

  ImGui::PushID(static_cast<int>(index));

  const bool selected = (m_selectedIndex == index);

  ImVec2 cursor = ImGui::GetCursorScreenPos();

  ImGui::InvisibleButton(
      "##sidebar_item",
      ImVec2(ITEM_SIZE, ITEM_SIZE));

  const bool hovered = ImGui::IsItemHovered();
  const bool clicked = ImGui::IsItemClicked();

  if (clicked)
  {
    if (m_selectedIndex != index) {
      m_selectedIndex = index;

      if (m_onItemSelected)
        m_onItemSelected(index);
    }
  }

  ImDrawList* drawList = ImGui::GetWindowDrawList();

  ImU32 backgroundColor;

  if (selected)
  {
    backgroundColor = IM_COL32(70, 70, 90, 255);
  }
  else if (hovered)
  {
    backgroundColor = IM_COL32(55, 55, 65, 255);
  }
  else
  {
    backgroundColor = IM_COL32(38, 38, 45, 255);
  }

  drawList->AddRectFilled(
      cursor,
      ImVec2(cursor.x + ITEM_SIZE, cursor.y + ITEM_SIZE),
      backgroundColor,
      ROUNDING);

  // Left accent bar for selected item
  if (selected)
  {
    drawList->AddRectFilled(
        ImVec2(cursor.x, cursor.y + 10.0f),
        ImVec2(cursor.x + 4.0f, cursor.y + ITEM_SIZE - 10.0f),
        IM_COL32(120, 170, 255, 255),
        6.0f);
  }

  // Centered icon
  const float iconX =
      cursor.x + (ITEM_SIZE - ICON_SIZE) * 0.5f;

  const float iconY =
      cursor.y + (ITEM_SIZE - ICON_SIZE) * 0.5f;

  drawList->AddImage(
      reinterpret_cast<void*>(
          static_cast<intptr_t>(item.icon->GetID())),
      ImVec2(iconX, iconY),
      ImVec2(iconX + ICON_SIZE, iconY + ICON_SIZE));

  // Tooltip hint on hover
  if (hovered)
  {
    ImGui::BeginTooltip();
    ImGui::TextUnformatted(item.title.c_str());
    ImGui::EndTooltip();
  }

  ImGui::Dummy(ImVec2(0.0f, 10.0f));

  ImGui::PopID();
}
#endif
#if 0
void SideBarWidget::Draw() {
  static ImFont *const& font_ubuntu_regular_60 = ResourceManager::getFont("Ubuntu-Regular-60");
  static ImFont *const& font_ubuntu_regular_45 = ResourceManager::getFont("Ubuntu-Regular-45");
  static ImFont *const& font_ubuntu_regular_30 = ResourceManager::getFont("Ubuntu-Regular-30");
  static ImFont *const& font_ubuntu_regular_20 = ResourceManager::getFont("Ubuntu-Regular-20");
  static ImFont *const& font_ubuntu_regular_18 = ResourceManager::getFont("Ubuntu-Regular-18");
  static ImFont *const& font_ubuntu_regular_16 = ResourceManager::getFont("Ubuntu-Regular-16");
  static ImFont *const& font_ubuntu_regular_14 = ResourceManager::getFont("Ubuntu-Regular-14");
  static ImFont *const& font_ubuntu_regular_12 = ResourceManager::getFont("Ubuntu-Regular-12");

  ImGui::PushFont(font_ubuntu_regular_30);
  ImGui::PushStyleColor(ImGuiCol_ChildBg, Constants::Colors::SIDEBAR_BACKGROUND_COLOR);
  for (std::size_t i{0}; i < m_items.size(); ++i)
  {
    this->drawSideBarItem(i);
  }
  ImGui::PopStyleColor(1);
  ImGui::PopFont();
}
#endif

void SideBarWidget::Draw()
{
  static constexpr ImVec4 kSideBarBackgroundColor = Constants::Colors::MENUBAR_COLOR;

  static ImFont* const& font_ubuntu_regular_16 =
      ResourceManager::getFont("Ubuntu-Regular-16");

  constexpr float SIDEBAR_WIDTH = 70.0f;

  ImGui::PushFont(font_ubuntu_regular_16);

  // Sidebar colors
  ImGui::PushStyleColor(
      ImGuiCol_ChildBg,
      kSideBarBackgroundColor);

  ImGui::PushStyleVar(
      ImGuiStyleVar_ChildRounding,
      0.0f);

  ImGui::PushStyleVar(
      ImGuiStyleVar_WindowPadding,
      ImVec2(12.0f, 18.0f));

  ImGui::BeginChild(
      "##sidebar_container",
      ImVec2(SIDEBAR_WIDTH, 0.0f),
      false,
      ImGuiWindowFlags_NoScrollbar |
      ImGuiWindowFlags_NoScrollWithMouse);

    // top spacing
    ImGui::Dummy(ImVec2(0.0f, 8.0f));

    for (std::size_t i = 0; i < m_items.size(); ++i)
    {
      drawSideBarItem(i);
    }

  ImGui::EndChild();

  ImGui::PopStyleColor();
  ImGui::PopStyleVar(2);
  ImGui::PopFont();
}
