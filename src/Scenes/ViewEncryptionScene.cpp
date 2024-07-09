#include <pch.hpp>
#include "ViewEncryptionScene.hpp"
#include <Algorithm/RSA/RSA.hpp>
#include <Application/Application.hpp>
#include <Utility/DialogUtils.hpp>

#include <Algorithm/Algorithm.hpp>
#include <GUI/ImGuiWidgets.hpp>
#include <System/Clipboard/Clipboard.hpp>
//#include <Utility/GZip.hpp>
#include <Utility/DateTimeUtils.hpp>
#include <Utility/FinalAction.hpp>
#include <imgui.h>

NS_ENIGMA_BEGIN

ViewEncryptionScene::ViewEncryptionScene(const std::int64_t encryption_id)
    : Enigma::Scene() {
  ENIGMA_TRACE("Retrieving encryption#{0}'s data from database...", encryption_id);

  // Get Encryption record from database (minus cipher to save up memory)
  m_encryption = Database::getEncryption(encryption_id);
  if (!m_encryption) {
    (void) DialogUtils::Error("Couldn't get encryption record from database");
    Scene::EndScene();
  }
}

void ViewEncryptionScene::OnCreate() {
  ENIGMA_TRACE_CURRENT_FUNCTION();
}

void ViewEncryptionScene::OnUpdate(const float&) {
}

void ViewEncryptionScene::OnDraw() {
  // Clear GL buffers
  //glAssert(glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}

void ViewEncryptionScene::OnImGuiDraw() {
  const auto& [win_w, win_h] = Application::getInstance()->GetWindow()->GetSize();
  const auto& [win_x, win_y] = Application::getInstance()->GetWindow()->GetPosition();
  static const auto& io = ImGui::GetIO();

  const auto button_size = ImVec2(win_w / 2.6f, 40.0f);

  static constexpr const auto inline_dummy = [](const float& x, const float& y) noexcept {  ImGui::SameLine(); ImGui::Dummy(ImVec2(x, y)); ImGui::SameLine(); };
  static constexpr const auto spacing = [](const std::uint8_t& n) noexcept { for (std::uint8_t i = 0; i < n; i++) ImGui::Spacing(); };

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
    static const std::string title = ("View Encryption");
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


    spacing(2);
    ImGui::Separator();
    spacing(2);

    // Encryption info (title \n Date time  - size)
    {
      // title
      ImGui::PushFont(font_audiowide_regular_20);
      ImGui::TextWrapped("%s", m_encryption->title.c_str()); // we had an issue here, abort was called because the random generated string contains % confuses imgui which thinks it needs format string, solved by adding %s then text
      ImGui::PopFont();

      spacing(2);

      // Date time  - size - ...
      ImGui::PushFont(font_montserrat_medium_18);
      {
        static const std::string format = std::string("Format: ") + (m_encryption->is_file ? ("File") : ("Text"));
        ImGui::ButtonEx(format.c_str(), {0.0f, 0.0f}, ImGuiItemFlags_Disabled);
        inline_dummy(6.0f, 0.0f);

        static const std::string date_time = std::string("Date Time: ") + m_encryption->date_time;
        ImGui::ButtonEx(date_time.c_str(), {0.0f, 0.0f}, ImGuiItemFlags_Disabled);
        inline_dummy(6.0f, 0.0f);

        static const std::string size = SizeUtils::FriendlySize(m_encryption->size);
        ImGui::ButtonEx(size.c_str(), {0.0f, 0.0f}, ImGuiItemFlags_Disabled);
        inline_dummy(6.0f, 0.0f);

        static const std::string algorithm = "Algorithm: " + Algorithm::AlgoTypeEnumToStr(m_encryption->algo);
        ImGui::ButtonEx(algorithm.c_str(), {0.0f, 0.0f}, ImGuiItemFlags_Disabled);
      }
      //const auto text_size = ImGui::CalcTextSize(text.c_str());
      //ImGui::SetCursorPosX((win_w - (text_size.x)) / 2.0f);
      //ImGui::TextWrapped(text.c_str());
      //ImGui::TextWrapped("%s\t-\t%s", m_encryption->date_time.c_str(), SizeUtils::FriendlySize(m_encryption->size).c_str());
      ImGui::PopFont();
    }

    spacing(2);
    ImGui::Separator();
    spacing(2);

    if (m_encryption->algo == Algorithm::Type::RSA) {
      // Private key for RSA
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
        ImGui::Text("%s:", "Password");

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
    if (!m_recovered_text.empty() && !m_encryption->is_file) {
      spacing(3);
      ImGui::Separator();
      spacing(3);


      ImGui::PushFont(font_montserrat_medium_20);
      {
        // Label
        ImGui::Text("%s:", "Recovered Text");

        // Encrypted text
        static const ImVec2 copy_button_size(45.0f, 25.0f);
        ImGuiWidgets::InputTextMultiline("##enc_txt", &m_recovered_text, ImVec2(win_w - (copy_button_size.x * 1.5f), ImGui::GetTextLineHeightWithSpacing() * 10.0f));
        ImGui::PushFont(font_montserrat_medium_12);
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, Constants::Colors::BUTTON_COLOR);              // buttons color idle
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Constants::Colors::BUTTON_COLOR_HOVER); // buttons color hover
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, Constants::Colors::BUTTON_COLOR_ACTIVE); // buttons color pressed
        if (ImGui::Button("Copy", copy_button_size)) {
          this->OnCopyRecoveredTextButtonPressed();
        }
        ImGui::PopStyleColor(3);
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
        if (ImGui::Button("Decrypt", button_size)) {
          Application::getInstance()->LaunchWorkerThread(this, m_encryption->is_file ? ("Decrypting file...") : ("Decrypting text..."),
                                                         [this]() -> void {
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

void ViewEncryptionScene::OnEvent(Event& /*event*/) {}

void ViewEncryptionScene::OnDestroy() {
  ENIGMA_TRACE_CURRENT_FUNCTION();
}


void ViewEncryptionScene::OnBackButtonPressed() {
  Scene::EndScene();
}


void ViewEncryptionScene::OnDecryptButtonPressed() {
  if (m_password.empty() && m_encryption->algo != Algorithm::Type::RSA) {
    (void) DialogUtils::Warn("Password is empty");
    return;
  }
  if(m_encryption->algo == Algorithm::Type::RSA && m_rsa_private_key.empty()) {
    (void) DialogUtils::Warn("RSA private key is empty");
    return;
  }

  try {
    if (m_encryption->is_file) {
      // Ask where to save decrypted file ?
      // Get path to where decrypted file should be saved
      const std::initializer_list<std::string> filters = {(m_encryption->file_ext.empty() ? std::string() : std::string('*' + m_encryption->file_ext)), "All Files", "*"}; // { "Text Files (.txt .text)", "*.txt *.text", "All Files", "*" }
      const std::string output_filename = SaveFileDialog("Select a location to save decrypted file", "Decrypted"+(m_encryption->file_ext.empty() ? "" : m_encryption->file_ext), true, filters).Show();
      if (output_filename.empty()) return;
      // Ensure selected output filename has an extension and it is the same one used in encryption
      if (!m_encryption->file_ext.empty() && fs::path(output_filename).extension() != m_encryption->file_ext) {
        DialogUtils::Error("Original file extension must match recovered file extension (" + m_encryption->file_ext + ").");
        return;
      }
      // 1. create a temp file to  reassemble cipher chunks from db
      fs::path tmpFilename = fs::temp_directory_path() / (DateTimeUtils::now("Enigma_tmp_%Y_%m_%d_%H_%M_%S") + m_encryption->file_ext + ".enigma");
      FinalAction tmpFileDeleter([tmpFilename] { fs::remove(tmpFilename); });
      std::ofstream ofs{tmpFilename, std::ios::binary};
      bool ok = ofs.good();
      Database::getCipherChunks(m_encryption->ide, [&ok, &ofs](std::unique_ptr<CipherChunk>&& cc) -> bool {
        ok &= ofs.write(reinterpret_cast<const char *>(cc->bytes.data()), cc->bytes.size()).good();
        return ok;
      });
      ofs.close();
      ENIGMA_ASSERT_OR_THROW(ok, "Could not reassemble encrypted file cipher chunks from database");

      Meta::EnigmaFooter footer = Meta::EnigmaFooter::fromFile(tmpFilename);
      Algorithm::Type algoType = footer.algo;

      // Auto detect RSA key size
      if (m_encryption->algo == Algorithm::Type::RSA) {
        ENIGMA_ASSERT_OR_THROW(footer.extra.size() == sizeof(m_rsa_keySize), "Could not read RSA key size from EnigmaFooter::extra");
        std::memcpy(&m_rsa_keySize, footer.extra.data(), footer.extra.size());
        ENIGMA_INFO("Successfully auto-detected RSA key size used for encryption which is {0}", m_rsa_keySize);
      }

      // Create decryptor based on selected algorithm type
      auto algorithm = Algorithm::CreateFromType(algoType, Algorithm::Intent::Decrypt);

      // RSA SETTINGS
      if (algorithm->GetType() == Algorithm::Type::RSA) {
        RSA::RSASettings settings{};
        settings.keySize = m_rsa_keySize;
        settings.privateKey = m_rsa_private_key; // TODO: maybe allow user to put just the path of the private key?
        dynamic_cast<RSA *>(algorithm.get())->setSettings(std::move(settings));
      }

      // Decrypt file
      algorithm->Decrypt(m_password, tmpFilename, output_filename);
      // little happy msg
      DialogUtils::Info("File decrypted successfully to " + output_filename);

    } else {
      // Note: for non-file encryptions, the whole cipher is considered cipher chunk.
      const std::unique_ptr<CipherChunk> cipherChunk = Database::getCipherChunk(m_encryption->ide);
      ENIGMA_ASSERT_OR_THROW(cipherChunk, ("Failed to get cipher chunk by encryption id"));

      Meta::EnigmaFooter footer = Meta::EnigmaFooter::fromBytes(cipherChunk->bytes);
      Algorithm::Type algo_type = footer.algo;
      ENIGMA_INFO("Successfully auto-detected algorithm used for encryption which is {}", Algorithm::AlgoTypeEnumToStr(algo_type));

      // Auto detect RSA key size
      if (m_encryption->algo == Algorithm::Type::RSA) {
        ENIGMA_ASSERT_OR_THROW(footer.extra.size() == sizeof(m_rsa_keySize), "Could not read RSA key size from EnigmaFooter::extra");
        std::memcpy(&m_rsa_keySize, footer.extra.data(), footer.extra.size());
        ENIGMA_INFO("Successfully auto-detected RSA key size used for encryption which is {0}", m_rsa_keySize);
      }

      // Create decryptor based on selected algorithm type
      const auto algorithm = Algorithm::CreateFromType(algo_type, Algorithm::Intent::Decrypt);
      ENIGMA_ASSERT_OR_THROW(algorithm, ("Failed to create algorithm from type"));

      // RSA SETTINGS
      if (algorithm->GetType() == Algorithm::Type::RSA) {
        RSA::RSASettings settings{};
        settings.keySize = m_rsa_keySize;
        settings.privateKey = m_rsa_private_key; // TODO: maybe allow user to put just the path of the private key?
        dynamic_cast<RSA *>(algorithm.get())->setSettings(std::move(settings));
      }

      // Decrypt cipher text
      std::vector<byte> recovered = algorithm->Decrypt(m_password, cipherChunk->bytes);
      m_recovered_text = std::string(recovered.begin(), recovered.end());
      recovered.clear();
      ENIGMA_ASSERT_OR_THROW(!m_recovered_text.empty(), ("Failed to decrypt text"));
    }

  } catch (const CryptoPP::Exception& e) {
    const std::string err_msg = CryptoPPUtils::GetFullErrorMessage(e);
    ENIGMA_ERROR("Decryption Failure: {0}", err_msg);
    (void) DialogUtils::Error(("Decryption Failure"), err_msg);
  } catch (const std::exception& e) {
    ENIGMA_ERROR("Decryption Failure: {0}", e.what());
    (void) DialogUtils::Error(("Decryption Failure"), e.what());
  } catch (...) {
    const std::string err_msg = "Decryption Failure: Unknown Error";
    ENIGMA_ERROR(("Decryption Failure UNKNOWN ERROR"));
    (void) DialogUtils::Error(err_msg);
  }
}

void ViewEncryptionScene::OnCopyRecoveredTextButtonPressed() {
  Clipboard::Set(m_recovered_text);
}
NS_ENIGMA_END
