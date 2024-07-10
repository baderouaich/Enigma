#include <pch.hpp>
#include "DecryptTextScene.hpp"
#include <Algorithm/RSA/RSA.hpp>
#include <GUI/ImGuiWidgets.hpp>
#include <System/Clipboard/Clipboard.hpp>
#include <Utility/Base64.hpp>
#include <Utility/DialogUtils.hpp>
//#include <Utility/GZip.hpp>
//#include <Scenes/RSAScene.hpp>

NS_ENIGMA_BEGIN

DecryptTextScene::DecryptTextScene()
    : Enigma::Scene(),
      m_type(Algorithm::Type::AES) // default
{
}

void DecryptTextScene::OnCreate() {
  ENIGMA_TRACE_CURRENT_FUNCTION();

  // Explicit OpenGL old method to et background clear color
  //glAssert(glClearColor(
  //	Constants::Colors::BACKGROUND_COLOR.x,
  //	Constants::Colors::BACKGROUND_COLOR.y,
  //	Constants::Colors::BACKGROUND_COLOR.z,
  //	Constants::Colors::BACKGROUND_COLOR.w
  //));
}

void DecryptTextScene::OnUpdate(const float& /*dt*/) {}

void DecryptTextScene::OnDraw() {
  // Clear GL buffers
  //glAssert(glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}

void DecryptTextScene::OnImGuiDraw() {
  const auto& [win_w, win_h] = Application::getInstance()->GetWindow()->GetSize();
  const auto& [win_x, win_y] = Application::getInstance()->GetWindow()->GetPosition();
  static const auto& io = ImGui::GetIO();

  const auto button_size = ImVec2(win_w / 2.5f, 40.0f);

  static constexpr const auto dummy = [](const float& x, const float& y) noexcept { ImGui::Dummy(ImVec2(x, y)); };
  static constexpr const auto inline_dummy = [](const float& x, const float& y) noexcept {  ImGui::SameLine(); ImGui::Dummy(ImVec2(x, y)); };
  static constexpr const auto spacing = [](const std::uint8_t& n) noexcept { for (std::uint8_t i = 0; i < n; i++) ImGui::Spacing(); };
  static constexpr const auto inline_spacing = [](const std::uint8_t& n) noexcept { for (std::uint8_t i = 0; i < n; i++) { ImGui::SameLine(); ImGui::Spacing(); } };

  static const auto& fonts = Application::getInstance()->GetFonts();
  static ImFont *const& font_audiowide_regular_45 = fonts.at("Audiowide-Regular-45");
  static ImFont *const& font_audiowide_regular_30 = fonts.at("Audiowide-Regular-30");
  static ImFont *const& font_audiowide_regular_20 = fonts.at("Audiowide-Regular-20");
  static ImFont *const& font_montserrat_medium_20 = fonts.at("Montserrat-Medium-20");
  static ImFont *const& font_montserrat_medium_18 = fonts.at("Montserrat-Medium-18");
  static ImFont *const& font_montserrat_medium_12 = fonts.at("Montserrat-Medium-12");
  static ImFont *const& font_ubuntu_regular_30 = fonts.at("Ubuntu-Regular-30");
  static ImFont *const& font_ubuntu_regular_20 = fonts.at("Ubuntu-Regular-20");
  static ImFont *const& font_ubuntu_regular_18 = fonts.at("Ubuntu-Regular-18");

  static constexpr const auto container_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove; // | ImGuiWindowFlags_NoBackground;

  // Push window's background color
  ImGui::PushStyleColor(ImGuiCol_WindowBg, Constants::Colors::BACKGROUND_COLOR);

  ImGui::Begin("Container", nullptr, container_flags);
  ImGui::SetWindowSize(ImVec2(static_cast<float>(win_w), static_cast<float>(win_h))); // same size as window
  ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));                                            // top left
  {
#pragma region Back button [<] & Scene Title
    static const auto& title_font = font_audiowide_regular_30;
    static const std::string title = ("Decrypt Text");
    static const ImVec2 title_size((ImGui::CalcTextSize(title.c_str()).x * title_font->Scale) - 45.0f, ImGui::CalcTextSize(title.c_str()).y * title_font->Scale);
    static const ImVec2 back_button_size(45.0f, title_size.y);

    // Back Button [<]
    {
      ImGui::PushFont(font_montserrat_medium_18); // < arrow is a text too
      if (ImGuiWidgets::BackButton("##back", back_button_size)) {
        this->OnBackButtonPressed();
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
        (void) ImGui::ButtonEx(title.c_str(), ImVec2(static_cast<float>(win_w), title_size.y), ImGuiItemFlags_Disabled);
      }
      ImGui::PopStyleColor(2);
      ImGui::PopFont();
    }
#pragma endregion


    spacing(3);
    ImGui::Separator();
    spacing(3);

    // Algorithm used in encryption to decrypt text with
    ImGui::PushFont(font_audiowide_regular_20);
    {
      // Label
      ImGui::Text("%s:", ("Algorithm"));
      ImGui::NewLine();

      // Algo types radio buttons
      const static auto supported_algorithms = Algorithm::GetSupportedAlgorithms();
      for (const auto& [algo_name, algo_type]: supported_algorithms) {
        inline_dummy(1.0f, 0.0f);
        ImGui::SameLine();
        if (ImGui::RadioButton(algo_name.c_str(), m_type == algo_type)) {
          m_type = algo_type;
        }
      }
    }
    ImGui::PopFont();

    // RSA Key sizes (will be auto detected from cipher base64 too)
    ImGui::PushFont(font_ubuntu_regular_18);
    if (m_type == Algorithm::Type::RSA) {
      ImGui::Text("%s:", ("Key Size"));
      ImGui::NewLine();
      for (const auto& [keySize, description]: RSA::RECOMMENDED_KEY_SIZES) {
        inline_dummy(1.0f, 0.0f);
        ImGui::SameLine();
        const std::string keySizeStr = std::to_string(keySize);
        if (ImGui::RadioButton(keySizeStr.c_str(), m_selected_rsa_keySize == keySize)) {
          m_selected_rsa_keySize = keySize;
        }
        if (ImGui::IsItemHovered())
          ImGui::SetTooltip("%s", description.data());
      }
    }
    ImGui::PopFont();

    spacing(3);
    ImGui::Separator();
    spacing(3);

    // Encrypted Text in Base64 to Decrypt
    ImGui::PushFont(font_montserrat_medium_20);
    {
      // Label
      ImGui::Text("%s:", ("Cipher (in base64)"));

      // Input text
      const ImVec2 input_text_size(static_cast<float>(win_w), ImGui::GetTextLineHeightWithSpacing() * 2.5f);
      ImGuiWidgets::InputTextMultiline("##cipher_base64", &m_cipher_base64, input_text_size);

      // Bytes count
      ImGui::PushFont(font_montserrat_medium_12);
      //ImGui::Text("%zu bytes", m_cipher_base64.size());
      ImGui::Text("%s", SizeUtils::FriendlySize(m_cipher_base64.size()).c_str());
      ImGui::PopFont();
    }
    ImGui::PopFont();

    spacing(3);
    ImGui::Separator();
    spacing(3);

    // Private key for RSA
    if (m_type == Algorithm::Type::RSA) {
      ImGui::PushFont(font_ubuntu_regular_18);
      const ImVec2 input_text_size(static_cast<float>(win_w), ImGui::GetTextLineHeightWithSpacing() * 10);
      ImGui::Text("%s:", "Private Key");
      ImGuiWidgets::InputTextMultiline("##private_key", &m_rsa_private_key, input_text_size);
      ImGui::PushID("LoadPrivateKeyFromFileButton");
      if (ImGuiWidgets::Button("Load from file...")) {
        if (std::vector<std::string> filenames = OpenFileDialog{"Load Private Key...", ".", false}.Show(); !filenames.empty()) {
          if (fs::is_regular_file(filenames[0]))
            FileUtils::ReadString(filenames[0], m_rsa_private_key);
        }
      }
      ImGui::PopID();
      ImGui::PopFont();
    } else {
      // Password used for encryption
      ImGui::PushFont(font_montserrat_medium_20);
      {
        // Label
        ImGui::Text("%s:", ("Password"));

        // Input text
        ImGuiWidgets::InputText("##password", &m_password, static_cast<float>(win_w), ImGuiInputTextFlags_::ImGuiInputTextFlags_Password);

        // Bytes count
        ImGui::PushFont(font_montserrat_medium_12);
        //ImGui::Text("%zu bytes", m_password.size());
        ImGui::Text("%s", SizeUtils::FriendlySize(m_password.size()).c_str());
        ImGui::PopFont();
      }
      ImGui::PopFont();
    }


    // Decrypted/Recovered Text
    if (!m_recovered_text.empty()) {
      spacing(3);
      ImGui::Separator();
      spacing(3);

      ImGui::PushFont(font_montserrat_medium_20);
      {
        // Label
        ImGui::Text("%s:", ("Recovered Text"));

        // Input text
        const ImVec2 input_text_size(win_w * 0.88f, ImGui::GetTextLineHeightWithSpacing() * 3.0f);
        ImGuiWidgets::InputTextMultiline("##recovered_text", &m_recovered_text, input_text_size);
        ImGui::SameLine();
        // Copy Button
        if (ImGuiWidgets::Button(("Copy"), ImVec2(win_w * 0.10f, 30.0f), Constants::Colors::BUTTON_COLOR, Constants::Colors::BUTTON_COLOR_HOVER, Constants::Colors::BUTTON_COLOR_ACTIVE)) {
          this->OnCopyDecryptedTextButtonPressed();
        }
        // Bytes count
        ImGui::PushFont(font_montserrat_medium_12);
        //ImGui::Text("%zu bytes", m_recovered_text.size());
        ImGui::Text("%s", SizeUtils::FriendlySize(m_recovered_text.size()).c_str());
        ImGui::PopFont();
      }
      ImGui::PopFont();
    }


    spacing(3);
    ImGui::Separator();
    spacing(3);

    // Decrypt Button
    {
      ImGui::PushFont(font_audiowide_regular_20);                                           // buttons font
      ImGui::PushStyleColor(ImGuiCol_Button, Constants::Colors::BUTTON_COLOR);              // buttons color idle
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Constants::Colors::BUTTON_COLOR_HOVER); // buttons color hover
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, Constants::Colors::BUTTON_COLOR_ACTIVE); // buttons color pressed
      {
        ImGui::SetCursorPosX((io.DisplaySize.x - button_size.x) / 2.0f);
        //ImGui::SetCursorPosY((io.DisplaySize.y - button_size.y) - 10.0f);
        if (ImGui::Button(("Decrypt"), button_size)) {
          Application::getInstance()->LaunchWorkerThread(this, ("Decrypting text..."), [this]() -> void {
            this->OnDecryptButtonPressed();
          });
        }
      }
      ImGui::PopStyleColor(3);
      ImGui::PopFont();
    }
  }
  ImGui::End();

  // Pop window's background color
  ImGui::PopStyleColor(1);
}

void DecryptTextScene::OnEvent(Event&) {
  /*if (Input::IsKeyPressed(KeyCode::Enter))
	{
		this->OnDecryptButtonPressed();
	}*/
  //if (event.IsInCategory(EventCategory::EC_KEYBOARD))
  //{
  //	if (Input::IsKeyPressed(Enigma::Key::Escape))
  //	{
  //		this->EndScene();
  //	}
  //}
}

void DecryptTextScene::OnDestroy() {
  ENIGMA_TRACE_CURRENT_FUNCTION();

  m_recovered_text.clear();
  m_cipher.clear();
  m_cipher_base64.clear();
  m_password.clear();
}


void DecryptTextScene::OnBackButtonPressed() {
  //if (!m_cipher_base64.empty() || !m_password.empty() || !m_recovered_text.empty())
  //{	// Show alert dialog to user asking whether the operation should be aborted
  //	const auto action = DialogUtils::Question("Are you sure you want to cancel the entire operation?");
  //	if (action == Enigma::MessageBox::Action::Yes)
  //	{
  //		Scene::EndScene();
  //	}
  //}
  //else
  Scene::EndScene();
}

void DecryptTextScene::OnCopyDecryptedTextButtonPressed() {
  if (!m_recovered_text.empty())
    Clipboard::Set(m_recovered_text);
}

void DecryptTextScene::OnDecryptButtonPressed() {
  if (m_cipher_base64.empty()) {
    (void) DialogUtils::Warn(("Cipher Base64 is empty"));
    return;
  }
  if (m_password.empty() && m_type != Algorithm::Type::RSA) {
    (void) DialogUtils::Warn(("Encryption password is empty"));
    return;
  }
  if (m_type == Algorithm::Type::RSA && m_rsa_private_key.empty()) {
    (void) DialogUtils::Warn("RSA private key is empty");
    return;
  }

  try {
    // Decode base64 to cipher
    m_cipher = Base64::Decode((const byte *) m_cipher_base64.data(), m_cipher_base64.size());
    ENIGMA_ASSERT_OR_THROW(!m_cipher.empty(), ("Failed to decode cipher base64! please make sure you have the exact cipher text you received on encryption"));

    // Auto detect encryption algorithm
    //m_type = Algorithm::DetectFromCipherBase64(m_cipher_base64);
    Meta::EnigmaFooter footer = Meta::EnigmaFooter::fromBytes(m_cipher);
    m_type = footer.algo;
    ENIGMA_INFO("Successfully auto-detected algorithm used for encryption which is {0}", Algorithm::AlgoTypeEnumToStr(m_type));

    // Auto detect RSA key size
    if (m_type == Algorithm::Type::RSA) {
      ENIGMA_ASSERT_OR_THROW(footer.extra.size() == sizeof(m_selected_rsa_keySize), "Could not read RSA key size from EnigmaFooter::extra");
      std::memcpy(&m_selected_rsa_keySize, footer.extra.data(), footer.extra.size());
      ENIGMA_INFO("Successfully auto-detected RSA key size used for encryption which is {0}", m_selected_rsa_keySize);
    }

    // Create encryptor based on selected algorithm type
    const auto algorithm = Algorithm::CreateFromType(m_type, Algorithm::Intent::Decrypt);
    ENIGMA_ASSERT_OR_THROW(algorithm, ("Failed to create algorithm from type"));

    // RSA SETTINGS
    if (algorithm->GetType() == Algorithm::Type::RSA) {
      RSA::RSASettings settings{};
      settings.keySize = m_selected_rsa_keySize;
      settings.privateKey = m_rsa_private_key; // TODO: maybe allow user to put just the path of the private key?
      dynamic_cast<RSA *>(algorithm.get())->setSettings(std::move(settings));
      m_password.clear();
    }

    // Decrypt text
    std::vector<byte> recovered = algorithm->Decrypt(m_password, m_cipher);
    m_recovered_text = std::string(recovered.begin(), recovered.end());
    recovered.clear();

  } catch (const CryptoPP::Exception& e) {
    const std::string err_msg = CryptoPPUtils::GetFullErrorMessage(e);
    ENIGMA_ERROR("Decryption Failure: {0}", err_msg);
    (void) DialogUtils::Error(("Decryption Failure"), err_msg);
  } catch (const std::exception& e) {
    ENIGMA_ERROR("Decryption Failure: {0}", e.what());
    (void) DialogUtils::Error(("Decryption Failure"), e.what());
  } catch (...) {
    const std::string err_msg = ("Decryption Failure UNKNOWN ERRORr");
    ENIGMA_ERROR("Decryption Failure UNKNOWN ERROR");
    (void) DialogUtils::Error(err_msg);
  }
}

NS_ENIGMA_END
