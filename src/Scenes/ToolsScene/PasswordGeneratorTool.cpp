#include <pch.hpp>
#include "PasswordGeneratorTool.hpp"
#include <GUI/ImGuiWidgets.hpp>
#include <System/Clipboard/Clipboard.hpp>
#include <Utility/Random.hpp>
#include <cstring>

NS_ENIGMA_BEGIN
void PasswordGeneratorTool::OnCreate() {
  ENIGMA_TRACE_CURRENT_FUNCTION();

  m_digits = m_uppercase_alphabets = m_lowercase_alphabets = m_special_characters = true;
}

void PasswordGeneratorTool::OnDraw(Scene *parent) {
  const auto& [win_w, win_h] = Application::getInstance()->GetWindow()->GetSize();
  //const auto& [win_x, win_y] = Application::getInstance()->GetWindow()->GetPosition();

  //const auto button_size = ImVec2(win_w / 2.6f, 40.0f);

  static constexpr const auto spacing = [](const std::uint8_t& n) noexcept { for (std::uint8_t i = 0; i < n; i++) ImGui::Spacing(); };

  static ImFont *const& font_ubuntu_regular_45 = ResourceManager::getFont("Ubuntu-Regular-45");
  static ImFont *const& font_ubuntu_regular_30 = ResourceManager::getFont("Ubuntu-Regular-30");
  static ImFont *const& font_ubuntu_regular_20 = ResourceManager::getFont("Ubuntu-Regular-20");
  static ImFont *const& font_ubuntu_regular_18 = ResourceManager::getFont("Ubuntu-Regular-18");
  static ImFont *const& font_ubuntu_regular_12 = ResourceManager::getFont("Ubuntu-Regular-12");

  ImGui::PushFont(font_ubuntu_regular_20);
  {
    if (ImGui::CollapsingHeader(("Password Generator"), nullptr, ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
    //if (ImGui::CollapsingHeader("Password Generator", nullptr))
    {
      ImGui::PushFont(font_ubuntu_regular_18);
      spacing(3);
      // Copy button, Output Input Text , Generate Button
      // Copy Generated password button 10% of width
      //ImGui::PushFont(font_ubuntu_regular_20);
      ImGui::PushID("CopyGeneratedPw");
      if (ImGuiWidgets::Button(("Copy"), ImVec2(win_w * 0.10f, 33.0f), Constants::Colors::BUTTON_COLOR, Constants::Colors::BUTTON_COLOR_HOVER, Constants::Colors::BUTTON_COLOR_ACTIVE)) {
        this->OnCopyPasswordButtonPressed();
      }
      ImGui::PopID();
      //
      ImGui::SameLine();

      // Output password input text 65% of width
      ImGuiWidgets::InputTextMultiline("##input_pw", &m_password, ImVec2(win_w * 0.70f, 33.0f));
      //

      ImGui::SameLine();

      // Generate button 25% of width
      if (ImGuiWidgets::Button(("Generate"), ImVec2(150.0f, 33.0f), Constants::Colors::TOOLS_BUTTON_COLOR, Constants::Colors::TOOLS_BUTTON_COLOR_HOVER, Constants::Colors::TOOLS_BUTTON_COLOR_ACTIVE)) {
        Application::getInstance()->LaunchWorkerThread(parent, ("Generating Password..."), [this]() {
          this->OnGenerateButtonPressed();
        });
      }

      spacing(2);


      //ImGui::PopFont();

      // Password Settings
      // Wrap password settings in columns to order things up
      //ImGui::PushFont(font_ubuntu_regular_14);
      ImGui::BeginColumns("Password Settings", 5, ImGuiOldColumnFlags_::ImGuiOldColumnFlags_NoResize);
      // Length Combo Box
      /*
					ImGui::Combo("Length", &m_selected_length_index, m_lengths.data(), m_lengths.size(), -1);
					ImGui::NextColumn();
					ImGui::Checkbox("Digits (0-9)", &m_digits); 
					ImGui::NextColumn();
					ImGui::Checkbox("Uppercase Alphabets (A-Z)", &m_uppercase_alphabets);
					ImGui::NextColumn();
					ImGui::Checkbox("Lowercase Alphabets (a-z)", &m_lowercase_alphabets); 
					ImGui::NextColumn();
					ImGui::Checkbox("Special Characters (+*~.#^)", &m_special_characters);
					*/
      //ImGui::Combo(("Length"), &m_selected_length_index, m_lengths.data(), static_cast<std::int32_t>(m_lengths.size()), -1);
      ImGuiWidgets::InputText(("Length"), &m_length, 70.0f, ImGuiInputTextFlags_::ImGuiInputTextFlags_CharsDecimal);
      ImGui::NextColumn();
      ImGui::Checkbox(("Digits (0-9)"), &m_digits);
      ImGui::NextColumn();
      ImGui::Checkbox(("Uppercase (A-Z)"), &m_uppercase_alphabets);
      ImGui::NextColumn();
      ImGui::Checkbox(("Lowercase (a-z)"), &m_lowercase_alphabets);
      ImGui::NextColumn();
      ImGui::Checkbox(("Symbols (+*~.#^/)"), &m_special_characters);
      ImGui::EndColumns();
      //ImGui::PopFont();
      //

      spacing(2);

      // Remember your password sentence (only when available)
      if (!m_remember_password_sentence.empty()) {
        ImGui::BulletText("%s:", ("Remember Your Password"));
        ImGui::PushID("CopyRememberPw");
        if (ImGuiWidgets::Button(("Copy"), ImVec2(win_w * 0.10f, 33.0f), Constants::Colors::BUTTON_COLOR, Constants::Colors::BUTTON_COLOR_HOVER, Constants::Colors::BUTTON_COLOR_ACTIVE)) {
          this->OnCopyRememberPasswordSentenceButtonPressed();
        }
        ImGui::PopID();
        ImGui::SameLine();
        ImGuiWidgets::InputTextMultiline("##output_rmb_pw", &m_remember_password_sentence, ImVec2(-1.0f, 33.0f));
        ImGui::NewLine();
      }
      //

      ImGui::PopFont();
    }
  }
  ImGui::PopFont();
}

void PasswordGeneratorTool::OnDestroy() {
  ENIGMA_TRACE_CURRENT_FUNCTION();
}

void PasswordGeneratorTool::OnGenerateButtonPressed() {
  if (!m_digits && !m_lowercase_alphabets && !m_uppercase_alphabets && !m_special_characters)
    return;

  if (!std::all_of(m_length.begin(), m_length.end(), [](char c) { return std::isdigit(c); })) {
    DialogUtils::Warn("Invalid password length");
    return;
  }
  constexpr const std::size_t maxPasswordLength = ENIGMA_MB_TO_BYTES(10); // 10mb pw? is enough at most

  const std::size_t length = std::stoull(m_length);
  if (length > maxPasswordLength) {
    DialogUtils::Warn("Password length is too long");
    return;
  }
  const std::string special_characters = Constants::Algorithm::SPECIAL_CHARACTERS;

  Random::Reseed();
  m_password.clear();
  m_password.reserve(length);
  while (m_password.size() < length) {
    char c = '\000';
  again:
    const std::uint16_t r = Random::Int<std::uint16_t>(1, 4);
    switch (r) {
      case 1: // m_digits
        if (m_digits) {
          c = static_cast<char>(Random::Int<int>('0', '9'));
        } else goto again;
        break;
      case 2: // m_uppercase_alphabets
        if (m_uppercase_alphabets) {
          c = static_cast<char>(Random::Int<int>('A', 'Z'));
        } else goto again;
        break;
      case 3: // m_lowercase_alphabets
        if (m_lowercase_alphabets) {
          c = static_cast<char>(Random::Int<int>('a', 'z'));
        } else goto again;
        break;
      case 4: // m_special_characters
        if (m_special_characters) {
          c = special_characters[Random::Int<std::size_t>(0, special_characters.size() - 1)];
        } else goto again;
        break;
      default:
        ENIGMA_ASSERT(false, "Unreachable");
        break;
    }
    m_password += c;
  }


  // Generate helpful sentence to remember password with
  m_remember_password_sentence.clear();
  for (const char c: m_password) {
    // if char its a digit or special character, place it directly
    if (m_remember_password_char_words.find(c) == m_remember_password_char_words.end()) {
      m_remember_password_sentence += c;
    } else {
      m_remember_password_sentence += m_remember_password_char_words.at(c);
    }
    m_remember_password_sentence += ' ';
  }
}

void PasswordGeneratorTool::OnCopyPasswordButtonPressed() {
  if (!m_password.empty())
    Clipboard::Set(m_password);
}

void PasswordGeneratorTool::OnCopyRememberPasswordSentenceButtonPressed() {
  if (!m_remember_password_sentence.empty())
    Clipboard::Set(m_remember_password_sentence);
}

NS_ENIGMA_END
