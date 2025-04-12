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

void PasswordGeneratorTool::OnDraw(Scene* parent) {
  const auto& [win_w, win_h] = Application::getInstance()->GetWindow()->GetSize();
  //const auto& [win_x, win_y] = Application::getInstance()->GetWindow()->GetPosition();

  //const auto button_size = ImVec2(win_w / 2.6f, 40.0f);

  static constexpr const auto spacing = [](const std::uint8_t& n) noexcept { for (std::uint8_t i = 0; i < n; i++) ImGui::Spacing(); };

  static ImFont* const& font_ubuntu_regular_45 = ResourceManager::getFont("Ubuntu-Regular-45");
  static ImFont* const& font_ubuntu_regular_30 = ResourceManager::getFont("Ubuntu-Regular-30");
  static ImFont* const& font_ubuntu_regular_20 = ResourceManager::getFont("Ubuntu-Regular-20");
  static ImFont* const& font_ubuntu_regular_18 = ResourceManager::getFont("Ubuntu-Regular-18");
  static ImFont* const& font_ubuntu_regular_12 = ResourceManager::getFont("Ubuntu-Regular-12");

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
  constexpr std::size_t maxPasswordLength = ENIGMA_MB_TO_BYTES(10); // 10mb password? is enough at most

  const std::size_t length = std::stoull(m_length);
  if (length > maxPasswordLength) {
    DialogUtils::Warn("Password length is too long");
    return;
  }
  static constexpr std::string_view digits = "0123456789";
  static constexpr std::string_view lowercase = "abcdefghijklmnopqrstuvwxyz";
  static constexpr std::string_view uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  static constexpr std::string_view special = Constants::Algorithm::SPECIAL_CHARACTERS;

  std::vector<std::string_view> charsets;
  charsets.reserve(4);
  if (m_digits) charsets.push_back(digits);
  if (m_lowercase_alphabets) charsets.push_back(lowercase);
  if (m_uppercase_alphabets) charsets.push_back(uppercase);
  if (m_special_characters) charsets.push_back(special);

  Random::Reseed();
  std::uniform_int_distribution<std::size_t> setDist{0, charsets.size() - 1};
  m_password.clear();
  m_password.resize(length, '\000');
  for (std::size_t i = 0; i < length; ++i) {
    const std::string_view& charset = charsets[setDist(Random::GetEngine())];
    std::uniform_int_distribution<std::size_t> charDist{0, charset.size() - 1};
    m_password[i] = charset[charDist(Random::GetEngine())];
  }

  // Generate helpful sentence to remember password with
  m_remember_password_sentence.clear();
  if (m_lowercase_alphabets || m_uppercase_alphabets) {
    for (const char c: m_password) {
      // if char its a digit or special character, place it directly
      if (!m_remember_password_char_words.contains(c)) {
        m_remember_password_sentence += c;
      } else {
        m_remember_password_sentence += m_remember_password_char_words.at(c);
      }
      m_remember_password_sentence += ' ';
    }
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
