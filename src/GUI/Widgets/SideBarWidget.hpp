//
// Created by bader on 5/28/26.
//
#pragma once
#include <Core/Core.hpp>
#include <GUI/Widgets/ImageWidgets.hpp>
#include <functional>

NS_ENIGMA_BEGIN
class SideBarWidget final {
public:
  struct Item {
    std::shared_ptr<ImageWidgets::Image> icon;
    std::string title;
  };
  explicit SideBarWidget(Scene* parent);

public:
  void Draw();

public:
  void setItems(std::initializer_list<Item>&& items);
  void setOnItemSelected(std::function<void(std::size_t idx)>&& onSelect);

private:
  void drawSideBarItem(const std::size_t index);

private:
  Scene* m_parent{};
  std::vector<Item> m_items{};
  std::size_t m_selectedIndex = 0;
  std::function<void(std::size_t idx)> m_onItemSelected{};
};
NS_ENIGMA_END