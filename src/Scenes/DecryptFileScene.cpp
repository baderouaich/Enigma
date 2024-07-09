#include <pch.hpp>
#include "DecryptFileScene.hpp"
#include <Algorithm/RSA/RSA.hpp>
#include <Events/EventDispatcher.hpp>
#include <GUI/ImGuiWidgets.hpp>
#include <System/Dialogs/MessageBox.hpp>
#include <System/Dialogs/OpenFileDialog.hpp>
#include <System/Dialogs/SaveFileDialog.hpp>
#include <System/Dialogs/SelectFolderDialog.hpp>
#include <Utility/DialogUtils.hpp>
#include <Utility/StringUtils.hpp>

NS_ENIGMA_BEGIN

DecryptFileScene::DecryptFileScene()
    : Enigma::Scene(),
      m_type(Algorithm::Type::AES) {
}

void DecryptFileScene::OnCreate() {
  ENIGMA_TRACE_CURRENT_FUNCTION();

  // Explicit OpenGL old method to et background clear color
  //glAssert(glClearColor(
  //	Constants::Colors::BACKGROUND_COLOR.x,
  //	Constants::Colors::BACKGROUND_COLOR.y,
  //	Constants::Colors::BACKGROUND_COLOR.z,
  //	Constants::Colors::BACKGROUND_COLOR.w
  //));

  // Set default m_in_filename as enigma exe path
  //m_in_filename = fs::current_path().string();
}

void DecryptFileScene::OnUpdate(const float&) {}

void DecryptFileScene::OnDraw() {
  // Clear GL buffers
  //glAssert(glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}

void DecryptFileScene::OnImGuiDraw() {
  const auto& [win_w, win_h] = Application::getInstance()->GetWindow()->GetSize();
  const auto& [win_x, win_y] = Application::getInstance()->GetWindow()->GetPosition();
  static const auto& io = ImGui::GetIO();

  const auto button_size = ImVec2(win_w / 2.5f, 40.0f);

  static constexpr const auto inline_dummy = [](const float& x, const float& y) noexcept {  ImGui::SameLine(); ImGui::Dummy(ImVec2(x, y)); };
  static constexpr const auto spacing = [](const std::uint8_t& n) noexcept { for (std::uint8_t i = 0; i < n; i++) ImGui::Spacing(); };

  static const auto& fonts = Application::getInstance()->GetFonts();
  static ImFont *const& font_audiowide_regular_45 = fonts.at("Audiowide-Regular-45");
  static ImFont *const& font_audiowide_regular_30 = fonts.at("Audiowide-Regular-30");
  static ImFont *const& font_audiowide_regular_20 = fonts.at("Audiowide-Regular-20");
  static ImFont *const& font_montserrat_medium_20 = fonts.at("Montserrat-Medium-20");
  static ImFont *const& font_montserrat_medium_18 = fonts.at("Montserrat-Medium-18");
  static ImFont *const& font_montserrat_medium_16 = fonts.at("Montserrat-Medium-16");
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
    static const std::string title = ("Decrypt File");
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

    // Algorithm used for encryption
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

    // RSA Key sizes (will be auto-detected from cipher base64 too)
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

    // In File to Decrypt
    ImGui::PushFont(font_montserrat_medium_20);
    {
      // Label
      ImGui::Text("%s:", ("File To Decrypt"));
      // Encrypted text
      static const ImVec2 browse_button_size(45.0f, 25.0f);
      ImGuiWidgets::InputText("##text1", &m_in_filename, win_w - (browse_button_size.x * 1.5f));
      ImGui::PushFont(font_montserrat_medium_12);
      ImGui::SameLine();
      ImGui::PushStyleColor(ImGuiCol_Button, Constants::Colors::BUTTON_COLOR);              // buttons color idle
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Constants::Colors::BUTTON_COLOR_HOVER); // buttons color hover
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, Constants::Colors::BUTTON_COLOR_ACTIVE); // buttons color pressed
      ImGui::PushID("Browse1");
      if (ImGui::Button("Browse", browse_button_size)) {
        Application::getInstance()->LaunchWorkerThread(this, "Browsing input file...", [this]() -> void {
          this->OnBrowseInFileButtonPressed();
        });
      }
      ImGui::PopID();
      ImGui::PopStyleColor(3);
      ImGui::PopFont();
    }
    ImGui::PopFont();

    spacing(2);

    // Out File Decrypted
    if (!m_in_filename.empty()) {
      ImGui::PushFont(font_montserrat_medium_20);
      {
        // Label
        ImGui::Text("%s:", ("Decrypted File Location"));
        // Encrypted text
        static const ImVec2 browse_button_size(45.0f, 25.0f);
        ImGuiWidgets::InputText("##text2", &m_out_filename, win_w - (browse_button_size.x * 1.5f));
        ImGui::PushFont(font_montserrat_medium_12);
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, Constants::Colors::BUTTON_COLOR);              // buttons color idle
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Constants::Colors::BUTTON_COLOR_HOVER); // buttons color hover
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, Constants::Colors::BUTTON_COLOR_ACTIVE); // buttons color pressed
        ImGui::PushID("Browse2");
        if (ImGui::Button("Browse", browse_button_size)) {
          Application::getInstance()->LaunchWorkerThread(this, "Browsing output file location...", [this]() -> void {
            this->OnBrowseOutFileLocationButtonPressed();
          });
        }
        ImGui::PopID();
        ImGui::PopStyleColor(3);
        ImGui::PopFont();
      }
      ImGui::PopFont();
    }


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
        ImGuiWidgets::InputText("##text3", &m_password, static_cast<float>(win_w), ImGuiInputTextFlags_::ImGuiInputTextFlags_Password);

        // Bytes count
        ImGui::PushFont(font_montserrat_medium_12);
        //ImGui::Text("%zu bytes", m_password.size());
        ImGui::Text("%s", SizeUtils::FriendlySize(m_password.size()).c_str());
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
          Application::getInstance()->LaunchWorkerThread(this, ("Decrypting file..."), [this]() -> void {
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

void DecryptFileScene::OnEvent(Event& event) {
  // Listen to Window File Drop Event for input filename
  EventDispatcher dispatcher(event);
  dispatcher.Dispatch<WindowFileDropEvent>([this](const WindowFileDropEvent& e) -> bool {
    // set dropped file into in filename
    if (const std::vector<fs::path>& filenames = e.GetFilenames();
        !filenames.empty() &&
        fs::exists(filenames[0]) &&
        fs::is_regular_file(filenames[0])) {
      this->m_in_filename = filenames[0].string();
    }

    return true; // handled, return false if other scenes may use the event
  });
}

void DecryptFileScene::OnDestroy() {
  ENIGMA_TRACE_CURRENT_FUNCTION();
}


void DecryptFileScene::OnBrowseInFileButtonPressed() {
  const auto ofd = std::make_unique<Enigma::OpenFileDialog>(
    "Select A File To Decrypt",
    m_in_filename, // initial path
    false          // disable multi-select
  );

  ENIGMA_TRACE("Selecting a file to decrypt...");
  const std::vector<std::string> selected_file_paths = ofd->Show();
  if (selected_file_paths.empty()) {
    ENIGMA_TRACE("Nothing is selected.");
    return;
  }
  m_in_filename = *selected_file_paths.begin();
}

void DecryptFileScene::OnBrowseOutFileLocationButtonPressed() {
  const auto sfd = std::make_unique<Enigma::SaveFileDialog>(
    ("Select A Location To Save Decrypted File To"),
    fs::path(m_in_filename).has_parent_path() ? fs::path(m_in_filename).parent_path().string() : m_in_filename, // in file location as an initial path
    true);

  ENIGMA_TRACE("Selecting a location to save decrypted file to...");
  const std::string selected_location = sfd->Show();
  if (selected_location.empty()) {
    ENIGMA_TRACE("Nothing is selected.");
    return;
  }

  // check if file ends with .enigma extension, if so, remove that extention to recover default file's one.
  if (StringUtils::LowerCopy(fs::path(m_in_filename).extension().string()) == ".enigma") {
    ENIGMA_TRACE("in file ends with .enigma ext. removing it...");
    // file ends with .enigma ext, remove it.
    m_out_filename = (fs::path(selected_location) / fs::path(m_in_filename).replace_extension("").filename()).string();
  } else {
    // file doesnt end with .enigma ext, assume it is its original ext.
    m_out_filename = (fs::path(selected_location) / fs::path(m_in_filename)).string();
  }

#if 0
	if (fs::path(m_in_filename).extension().string() == ".enigma")
		m_out_filename = selected_location + '/' + fs::path(m_in_filename).replace_extension("").filename().string();
	else
		m_out_filename = selected_location;
#endif
}

void DecryptFileScene::OnDecryptButtonPressed() {
  // Validate fields
  if (m_in_filename.empty()) {
    (void) DialogUtils::Warn(("Input File to decrypt is empty"));
    return;
  }
  if (m_out_filename.empty()) {
    (void) DialogUtils::Warn(("Out File location is empty"));
    return;
  }
  if (!fs::exists(m_in_filename) || !fs::is_regular_file(m_in_filename)) {
    (void) DialogUtils::Warn(("Input File does not exist"));
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

  // Alles gut
  try {
    // Auto detect encryption algorithm
    Meta::EnigmaFooter footer = Meta::EnigmaFooter::fromFile(m_in_filename);
    m_type = footer.algo;
    ENIGMA_INFO("Successfully auto-detected algorithm used for encryption which is {0}", Algorithm::AlgoTypeEnumToStr(m_type));

    // Auto detect RSA key size
    if (m_type == Algorithm::Type::RSA) {
      ENIGMA_ASSERT_OR_THROW(footer.extra.size() == sizeof(m_selected_rsa_keySize), "Could not read RSA key size from EnigmaFooter::extra");
      std::memcpy(&m_selected_rsa_keySize, footer.extra.data(), footer.extra.size());
      ENIGMA_INFO("Successfully auto-detected RSA key size used for encryption which is {0}", m_selected_rsa_keySize);
    }

    // Create decryptor based on selected algorithm type
    const auto algorithm = Algorithm::CreateFromType(m_type, Algorithm::Intent::Decrypt);
    ENIGMA_ASSERT_OR_THROW(algorithm, ("Failed to create algorithm from type"));

    // RSA SETTINGS
    if (algorithm->GetType() == Algorithm::Type::RSA) {
      RSA::RSASettings settings{};
      settings.keySize = m_selected_rsa_keySize;
      settings.privateKey = m_rsa_private_key; // TODO: maybe allow user to put just the path of the private key?
      dynamic_cast<RSA *>(algorithm.get())->setSettings(std::move(settings));
    }

    // Decrypt file cipher
    algorithm->Decrypt(m_password, fs::path(m_in_filename), fs::path(m_out_filename));

    // Alert user that decryption was successfull
    const std::string msg = fmt::format("Decrypted {} to {} successfully",
                                        fs::path(m_in_filename).filename(),
                                        fs::path(m_out_filename).filename());
    ENIGMA_INFO(msg);
    (void) DialogUtils::Info(msg);
  } catch (const CryptoPP::Exception& e) {
    const std::string err_msg = CryptoPPUtils::GetFullErrorMessage(e);
    ENIGMA_ERROR("Decryption Failure: {0}", err_msg);
    (void) DialogUtils::Error(("Decryption Failure"), err_msg);
  } catch (const std::exception& e) {
    ENIGMA_ERROR("Decryption Failure: {0}", e.what());
    (void) DialogUtils::Error(("Decryption Failure"), e.what());
  } catch (...) {
    const std::string err_msg = ("Decryption Failure UNKNOWN ERROR");
    ENIGMA_ERROR(err_msg);
    (void) DialogUtils::Error(err_msg);
  }
}

void DecryptFileScene::OnBackButtonPressed() {
  //if (!m_in_filename.empty() || !m_out_filename.empty() || !m_password.empty())
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


NS_ENIGMA_END
