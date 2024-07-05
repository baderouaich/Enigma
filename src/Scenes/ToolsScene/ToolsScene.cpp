#include "ToolsScene.hpp"
#include <Application/Application.hpp>
#include <imgui.h>
#include <pch.hpp>

#include "HashingTool.hpp"
#include "PasswordGeneratorTool.hpp"
#include "SystemAndHardwareInfoTool.hpp"
#include "XOREncryptionTool.hpp"
#include <GUI/ImGuiWidgets.hpp>

NS_ENIGMA_BEGIN

ToolsScene::ToolsScene() noexcept
    : Enigma::Scene() {
}

void ToolsScene::OnCreate() {
  ENIGMA_TRACE_CURRENT_FUNCTION();

  // Add Collapsing header tools
  m_tools.push_back(std::make_unique<PasswordGeneratorTool>());
  m_tools.push_back(std::make_unique<HashingTool>());
  m_tools.push_back(std::make_unique<XOREncryptionTool>());
  m_tools.push_back(std::make_unique<SystemAndHardwareInfoTool>());
  //m_tools.push_back(std::make_unique<XTool>());
  //m_tools.push_back(std::make_unique<XTool>());
  //m_tools.push_back(std::make_unique<XTool>());

  // Call OnCreate for each tool
  for (const auto& tool: m_tools) {
    tool->OnCreate();
  }
}

void ToolsScene::OnUpdate(const float&) {}

void ToolsScene::OnDraw() {}

void ToolsScene::OnImGuiDraw() {
  const auto& [win_w, win_h] = Application::getInstance()->GetWindow()->GetSize();
  const auto& [win_x, win_y] = Application::getInstance()->GetWindow()->GetPosition();

  static const auto& io = ImGui::GetIO();


  static constexpr const auto inline_dummy = [](const float& x, const float& y) noexcept {  ImGui::SameLine(); ImGui::Dummy(ImVec2(x, y)); };
  static constexpr const auto spacing = [](const std::uint8_t& n) noexcept { for (std::uint8_t i = 0; i < n; i++) ImGui::Spacing(); };

  static auto& fonts = Application::getInstance()->GetFonts();
  static ImFont *const& font_audiowide_regular_45 = fonts.at("Audiowide-Regular-45");
  static ImFont *const& font_audiowide_regular_30 = fonts.at("Audiowide-Regular-30");
  static ImFont *const& font_audiowide_regular_20 = fonts.at("Audiowide-Regular-20");
  static ImFont *const& font_montserrat_medium_20 = fonts.at("Montserrat-Medium-20");
  static ImFont *const& font_montserrat_medium_18 = fonts.at("Montserrat-Medium-18");
  static ImFont *const& font_montserrat_medium_12 = fonts.at("Montserrat-Medium-12");

  static constexpr const auto container_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove; // | ImGuiWindowFlags_NoBackground;

  // Push window's background color
  ImGui::PushStyleColor(ImGuiCol_WindowBg, Constants::Colors::BACKGROUND_COLOR);

  ImGui::Begin("Container", nullptr, container_flags);
  ImGui::SetWindowSize(ImVec2(static_cast<float>(win_w), static_cast<float>(win_h))); // same size as window
  ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));                                            // top left
  {
    // Back button [<] & Title
    {
      static const auto& title_font = font_audiowide_regular_30;
      const auto title = ("Tools");
      static const ImVec2 title_size((ImGui::CalcTextSize(title).x * title_font->Scale) - 45.0f, ImGui::CalcTextSize(title).y * title_font->Scale);
      static const ImVec2 back_button_size(45.0f, title_size.y);

      // Back Button [<]
      {
        ImGui::PushFont(font_montserrat_medium_18); // < arrow is a text too
        if (ImGuiWidgets::BackButton("##back", back_button_size)) {
          Scene::EndScene();
        }
        ImGui::PopFont();
      }

      ImGui::SameLine();

      // Scene Title
      {
        ImGui::PushFont(title_font);                                                             // text font
        ImGui::PushStyleColor(ImGuiCol_Text, Constants::Colors::TEXT_COLOR);                     // text color
        ImGui::PushStyleColor(ImGuiCol_Button, Constants::Colors::SCENE_TITLE_BACKGROUND_COLOR); // Scene title back color
        {
          (void) ImGui::ButtonEx(title, ImVec2(static_cast<float>(win_w), title_size.y), ImGuiItemFlags_Disabled);
        }
        ImGui::PopStyleColor(2);
        ImGui::PopFont();
      }
    }


    spacing(2);
    ImGui::Separator();
    spacing(2);

    ////====== Tools Collapsing Headers BEGIN ========///////
    // Call OnDraw for each tool
    for (const auto& tool: m_tools) {
      tool->OnDraw(this);
      spacing(2);
      ImGui::Separator();
      spacing(2);
    }
    ////====== Tools Collapsing Headers END ========///////
  }
  ImGui::End();


  // Pop window's background color
  ImGui::PopStyleColor(1);
}

void ToolsScene::OnEvent(Event& /*event*/) {}

void ToolsScene::OnDestroy() {
  ENIGMA_TRACE_CURRENT_FUNCTION();

  // Call OnDestroy for each tool
  for (const auto& tool: m_tools) {
    tool->OnDestroy();
  }
  m_tools.clear();
}


NS_ENIGMA_END
