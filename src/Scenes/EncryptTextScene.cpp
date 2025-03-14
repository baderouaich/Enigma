#include <pch.hpp>
#include "EncryptTextScene.hpp"
#include <Database/Database.hpp>
#include <Database/Models/Encryption.hpp>
#include <GUI/ImGuiWidgets.hpp>
#include <System/Clipboard/Clipboard.hpp>
#include <Utility/Base64.hpp>
#include <Utility/DialogUtils.hpp>
#include <Algorithm/RSA/RSA.hpp>
//#include <Utility/GZip.hpp>
//#include <Scenes/RSAScene.hpp>

NS_ENIGMA_BEGIN

EncryptTextScene::EncryptTextScene()
    : Enigma::Scene(),
      m_type(Algorithm::Type::AES) // default
{
}

void EncryptTextScene::OnCreate() {
  ENIGMA_TRACE_CURRENT_FUNCTION();

  // Explicit OpenGL old method to et background clear color
  //glAssert(glClearColor(
  //	Constants::Colors::BACKGROUND_COLOR.x,
  //	Constants::Colors::BACKGROUND_COLOR.y,
  //	Constants::Colors::BACKGROUND_COLOR.z,
  //	Constants::Colors::BACKGROUND_COLOR.w
  //));
}

void EncryptTextScene::OnUpdate(const float& /*dt*/) {}

void EncryptTextScene::OnDraw() {
  // Clear GL buffers
  //glAssert(glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}

void EncryptTextScene::OnImGuiDraw() {
  const auto& [win_w, win_h] = Application::getInstance()->GetWindow()->GetSize();
  const auto& [win_x, win_y] = Application::getInstance()->GetWindow()->GetPosition();
  static const auto& io = ImGui::GetIO();

  const auto button_size = ImVec2(win_w / 2.5f, 40.0f);

  static constexpr const auto dummy = [](const float& x, const float& y) noexcept { ImGui::Dummy(ImVec2(x, y)); };
  static constexpr const auto inline_dummy = [](const float& x, const float& y) noexcept {  ImGui::SameLine(); ImGui::Dummy(ImVec2(x, y)); };
  static constexpr const auto spacing = [](const std::uint8_t& n) noexcept { for (std::uint8_t i = 0; i < n; i++) ImGui::Spacing(); };
  static constexpr const auto inline_spacing = [](const std::uint8_t& n) noexcept { for (std::uint8_t i = 0; i < n; i++) { ImGui::SameLine(); ImGui::Spacing(); } };

  static ImFont *const& font_ubuntu_regular_60 = ResourceManager::getFont("Ubuntu-Regular-60");
  static ImFont *const& font_ubuntu_regular_45 = ResourceManager::getFont("Ubuntu-Regular-45");
  static ImFont *const& font_ubuntu_regular_30 = ResourceManager::getFont("Ubuntu-Regular-30");
  static ImFont *const& font_ubuntu_regular_20 = ResourceManager::getFont("Ubuntu-Regular-20");
  static ImFont *const& font_ubuntu_regular_18 = ResourceManager::getFont("Ubuntu-Regular-18");
  static ImFont *const& font_ubuntu_regular_16 = ResourceManager::getFont("Ubuntu-Regular-16");
  static ImFont *const& font_ubuntu_regular_14 = ResourceManager::getFont("Ubuntu-Regular-14");
  static ImFont *const& font_ubuntu_regular_12 = ResourceManager::getFont("Ubuntu-Regular-12");

  static constexpr const auto container_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove; // | ImGuiWindowFlags_NoBackground;

  // Push window's background color
  ImGui::PushStyleColor(ImGuiCol_WindowBg, Constants::Colors::BACKGROUND_COLOR);

  ImGui::Begin("Container", nullptr, container_flags);
  ImGui::SetWindowSize(ImVec2(static_cast<float>(win_w), static_cast<float>(win_h))); // same size as window
  ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));                                            // top left
  {
#pragma region Back button [<] & Scene Title
    static const auto& title_font = font_ubuntu_regular_30;
    static const std::string title = ("Encrypt Text");
    static const ImVec2 title_size((ImGui::CalcTextSize(title.c_str()).x * title_font->Scale) - 45.0f, ImGui::CalcTextSize(title.c_str()).y * title_font->Scale);
    static const ImVec2 back_button_size(45.0f, title_size.y);

    // Back Button [<]
    {
      ImGui::PushFont(font_ubuntu_regular_18); // < arrow is a text too
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
        (void) ImGui::ButtonEx(title.c_str(), ImVec2(static_cast<float>(win_w) - back_button_size.x - 25.0f, title_size.y), ImGuiItemFlags_Disabled);
      }
      ImGui::PopStyleColor(2);
      ImGui::PopFont();
    }
#pragma endregion

    spacing(3);
    ImGui::Separator();
    spacing(3);

    // Algorithm To encrypt text with
    ImGui::PushFont(font_ubuntu_regular_20);
    {
      // Label
      //ImGui::LabelText("##label", "Algorithm:");
      ImGui::Text("%s:", ("Algorithm"));
      ImGui::NewLine();

      // Algo types radio buttons
      const static auto supported_algorithms = Algorithm::GetSupportedAlgorithms();
      for (const auto& [algo_name, algo_type]: supported_algorithms) {
        ImGui::SameLine(0.0f,5.0f);
        if (ImGui::RadioButton(algo_name.c_str(), m_type == algo_type)) {
          m_type = algo_type;
        }
        if (ImGui::IsItemHovered())
          ImGui::SetTooltip("%s", Algorithm::ALGORITHM_DESCRIPTIONS.at(algo_type).data());
      }
    }
    ImGui::PopFont();

    // RSA Key sizes
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

    spacing(2);

    // Save to database widget
    ImGui::PushFont(font_ubuntu_regular_16);
    {
      ImGui::PushFont(font_ubuntu_regular_18);
      ImGui::Text("%s:", ("Save to database"));
      ImGui::PopFont();
      inline_dummy(6.0f, 0.0f);
      ImGui::SameLine();
      ImGui::Checkbox("##checkbox", &m_save_to_database);

      if (m_save_to_database) {
        ImGuiWidgets::InputTextWithHint("##idb", ("A clear encryption title to facilitate future record searches."), &m_db_title, win_w / 1.3f);
      }
    }
    ImGui::PopFont();

    spacing(2);
    ImGui::Separator();
    spacing(2);


    // Text to encrypt
    ImGui::PushFont(font_ubuntu_regular_20);
    {
      // Label
      ImGui::Text("%s:", ("Text"));

      // Input text
      const ImVec2 input_text_size(static_cast<float>(win_w), ImGui::GetTextLineHeightWithSpacing() * 6);
      ImGuiWidgets::InputTextMultiline("##text1", &m_text, input_text_size);
      // Bytes count
      ImGui::PushFont(font_ubuntu_regular_12);
      ImGui::Text("%s", SizeUtils::FriendlySize(m_text.size()).c_str());
      ImGui::PopFont();
    }
    ImGui::PopFont();

    spacing(3);
    ImGui::Separator();
    spacing(3);

    // Encryption Password & Confirm password (Private & Public keys for RSA)
    if (m_type == Algorithm::Type::RSA) {
      ImGui::PushFont(font_ubuntu_regular_18);
      const ImVec2 input_text_size(static_cast<float>(win_w), ImGui::GetTextLineHeightWithSpacing() * 10);
      if (!m_rsa_private_key.empty()) {
        ImGui::Text("%s:", "Private Key");
        ImGuiWidgets::InputTextMultiline("##private_key", &m_rsa_private_key, input_text_size);
        ImGui::PushID("CopyPrivateKeyButton");
        if (ImGuiWidgets::Button("Copy")) {
          Clipboard::Set(m_rsa_private_key);
        }
        ImGui::PopID();
        ImGui::SameLine();
        ImGui::PushID("SavePrivateKeyToFileButton");
        if (ImGuiWidgets::Button("Save to file...")) {
          SaveFileDialog sfd{"Save Private Key", "PrivateKey.pem", true, {"All Files", "*"}};
          if (std::string filename = sfd.Show(); !filename.empty()) {
            FileUtils::Write(filename, std::vector<byte>(m_rsa_private_key.begin(), m_rsa_private_key.end()));
          }
        }
        ImGui::PopID();
      }
      spacing(2);
      if (!m_rsa_public_key.empty()) {
        ImGui::Text("%s:", "Public Key");
        ImGuiWidgets::InputTextMultiline("##public_key", &m_rsa_public_key, input_text_size);
        ImGui::PushID("CopyPublicKeyButton");
        if (ImGuiWidgets::Button("Copy")) {
          Clipboard::Set(m_rsa_public_key);
        }
        ImGui::PopID();
        ImGui::SameLine();
        ImGui::PushID("SavePublicKeyToFileButton");
        if (ImGuiWidgets::Button("Save to file...")) {
          SaveFileDialog sfd{"Save Public Key", "PublicKey.pem", true, {"All Files", "*"}};
          if (std::string filename = sfd.Show(); !filename.empty()) {
            FileUtils::WriteString(filename, m_rsa_public_key);
          }
        }
        ImGui::PopID();
      }
      ImGui::PopFont();
    } else // RSA Doesnt need password
    {
      ImGui::PushFont(font_ubuntu_regular_20);
      {
        // password text coloring for each state
        ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text,
                              (m_password.empty() && m_confirm_password.empty()) ? // if password or confirm password is empty...
                                Constants::Colors::TEXT_COLOR
                                                                                 : // ... set color to white
                                m_password == m_confirm_password ?                 // else if password matches confim password...
                                  Constants::Colors::PASSWORD_MATCH_TEXT_COLOR
                                                                 :    // ... set color to green
                                  Constants::Colors::ERROR_TEXT_COLOR // else set color to red.
        );
        // Label
        ImGui::Text("%s:", ("Password"));
        // Input text
        ImGuiWidgets::InputText("##text2", &m_password, static_cast<float>(win_w), ImGuiInputTextFlags_::ImGuiInputTextFlags_Password);
        ImGui::Text("%s:", ("Confirm Password"));
        ImGuiWidgets::InputText("##text3", &m_confirm_password, static_cast<float>(win_w), ImGuiInputTextFlags_::ImGuiInputTextFlags_Password);
        ImGui::PopStyleColor();
        // Bytes count
        ImGui::PushFont(font_ubuntu_regular_12);
        //ImGui::Text("%zu bytes", m_password.size());
        ImGui::Text("%s", SizeUtils::FriendlySize(m_password.size()).c_str());
        ImGui::PopFont();
      }
      ImGui::PopFont();
    }


    // Encrypted text in Base64 output
    if (!m_cipher.empty()) {
      spacing(3);
      ImGui::Separator();
      spacing(3);

      ImGui::PushFont(font_ubuntu_regular_20);
      {
        // Label
        ImGui::Text("%s:", ("Cipher (in base64)"));
        // Encrypted text
        ImGuiWidgets::InputText("##text4", &m_cipher_base64, win_w * 0.88f);
        ImGui::PushFont(font_ubuntu_regular_14);
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, Constants::Colors::BUTTON_COLOR);              // buttons color idle
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Constants::Colors::BUTTON_COLOR_HOVER); // buttons color hover
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, Constants::Colors::BUTTON_COLOR_ACTIVE); // buttons color pressed
        if (ImGui::Button(("Copy"), ImVec2(win_w * 0.10f, 30.0f))) {
          this->OnCopyEncryptedBase64TextButtonPressed();
        }
        //ImGui::NewLine();
        ImGui::PushFont(font_ubuntu_regular_12);
        ImGui::Text("%s", SizeUtils::FriendlySize(m_cipher_base64.size()).c_str());
        ImGui::PopFont();
        ImGui::PopStyleColor(3);
        ImGui::PopFont();
      }
      ImGui::PopFont();
    }

    spacing(3);
    ImGui::Separator();
    spacing(3);

    // Encrypt Button
    {
      ImGui::PushFont(font_ubuntu_regular_20);                                           // buttons font
      ImGui::PushStyleColor(ImGuiCol_Button, Constants::Colors::BUTTON_COLOR);              // buttons color idle
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Constants::Colors::BUTTON_COLOR_HOVER); // buttons color hover
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, Constants::Colors::BUTTON_COLOR_ACTIVE); // buttons color pressed
      {
        ImGui::SetCursorPosX((io.DisplaySize.x - button_size.x) / 2.0f);
        //ImGui::SetCursorPosY((io.DisplaySize.y - button_size.y) - 10.0f);
        if (ImGui::Button(("Encrypt"), button_size)) {
          Application::getInstance()->LaunchWorkerThread(this, ("Encrypting text..."), [this]() -> void {
            this->OnEncryptButtonPressed();
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

void EncryptTextScene::OnEvent(Event&) {
  //// Go back to main menu scene
  //if (Input::IsKeyPressed(Enigma::Key::Escape))
  //{
  //	// Confirm encryption abortion
  //	if (this->UserWantsToCancel())
  //	{
  //		this->EndScene();
  //	}
  //}
};

void EncryptTextScene::OnDestroy() {
  ENIGMA_TRACE_CURRENT_FUNCTION();

  m_password.clear();
  m_confirm_password.clear();
  m_cipher.clear();
  m_cipher_base64.clear();
  m_text.clear();
}

void EncryptTextScene::OnEncryptButtonPressed() {
  // Validate fields
  if (m_text.empty()) {
    (void) DialogUtils::Warn(("Text to encrypt is empty"));
    return;
  }
  if ((m_password.empty() || m_confirm_password.empty()) && m_type != Algorithm::Type::RSA) {
    (void) DialogUtils::Warn(("Encryption password is empty"));
    return;
  }
  if (m_password.size() < Constants::Algorithm::MINIMUM_PASSWORD_LENGTH && m_type != Algorithm::Type::RSA) {
    (void) DialogUtils::Warn("Password is too weak");
    return;
  }
  if (m_password != m_confirm_password && m_type != Algorithm::Type::RSA) {
    (void) DialogUtils::Warn(("Password doesn't match confirm password"));
    return;
  }

  // Alles gut
  try {
    // Create encryptor based on selected algorithm type
    const auto algorithm = Algorithm::CreateFromType(m_type, Algorithm::Intent::Encrypt);
    ENIGMA_ASSERT_OR_THROW(algorithm, ("Failed to create algorithm from type"));
    // RSA SETTINGS
    if (algorithm->GetType() == Algorithm::Type::RSA) {
      RSA::RSASettings settings{};
      settings.keySize = m_selected_rsa_keySize;
      dynamic_cast<RSA *>(algorithm.get())->setSettings(std::move(settings));
      m_password.clear();
    }

    // Encrypt text
    m_cipher = algorithm->Encrypt(m_password, (const byte *) m_text.data(), m_text.size());
    ENIGMA_ASSERT_OR_THROW(!m_cipher.empty(), ("Failed to encrypt text"));

    // Encode cipher to Base64
    if (std::vector<byte> base64 = Base64::Encode(m_cipher); !base64.empty()) {
      m_cipher_base64 = std::string(base64.begin(), base64.end());
      base64.clear();
    }
    ENIGMA_ASSERT_OR_THROW(!m_cipher_base64.empty(), ("Failed to encode cipher text to Base64"));

    // Fetch RSA private & public key
    if (m_type == Algorithm::Type::RSA) {
      m_rsa_private_key = dynamic_cast<RSA *>(algorithm.get())->getPrivateKey();
      m_rsa_public_key = dynamic_cast<RSA *>(algorithm.get())->getPublicKey();
    }

    // Save to database
    if (m_save_to_database) {
      SQLite::Transaction transaction{*Database::getStorage()};

      // Encryption
      ENIGMA_ASSERT_OR_THROW(ENIGMA_IS_BETWEEN(m_db_title.size(), 3, 255), ("Encryption title must be between 3 and 255 characters"));
      auto e = std::make_unique<Encryption>();
      e->algo = m_type;
      e->title = m_db_title;
      e->is_file = false;
      e->size = m_cipher.size();
      e->ide = Database::addEncryption(e);
      ENIGMA_ASSERT_OR_THROW(e->ide >= 0, ("Failed to save encryption record to database"));

      // CipherChunk
      auto cc = std::make_unique<CipherChunk>();
      cc->ide = e->ide;
      cc->offset = 0; // we're storing the entire chunk
      cc->size = m_cipher.size();
      cc->bytes = m_cipher;
      cc->idc = Database::addCipherChunk(cc);
      ENIGMA_ASSERT_OR_THROW(cc->idc >= 0, ("Failed to save cipher chunk to database"));

      transaction.commit();
    }

    // Spawn notification alert if window is not focused
    if (!Application::getInstance()->GetWindow()->IsFocused()) {
      Notification{"Enigma", "Text encrypted successfully "}.Show();
    }

  } catch (const CryptoPP::Exception& e) {
    const std::string err_msg = CryptoPPUtils::GetFullErrorMessage(e);
    ENIGMA_ERROR("Encryption Failure: {0}", err_msg);
    (void) DialogUtils::Error(("Encryption Failure"), err_msg);
  } catch (const std::exception& e) {
    ENIGMA_ERROR("Encryption Failure: {0}", e.what());
    (void) DialogUtils::Error(("Encryption Failure"), e.what());
  } catch (...) {
    const std::string err_msg = ("Encryption Failure UNKNOWN ERROR");
    ENIGMA_ERROR(err_msg);
    (void) DialogUtils::Error(err_msg);
  }
}

void EncryptTextScene::OnBackButtonPressed() {
  //if (!m_text.empty() || !m_password.empty() || !m_confirm_password.empty())
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

void EncryptTextScene::OnCopyEncryptedBase64TextButtonPressed() {
  Clipboard::Set(m_cipher_base64);
}


NS_ENIGMA_END
