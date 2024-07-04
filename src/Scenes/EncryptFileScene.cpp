#include "EncryptFileScene.hpp"
#include "pch.hpp"
#include <Database/Database.hpp>
#include <Database/Models/Encryption.hpp>
#include <Events/EventDispatcher.hpp>
#include <GUI/ImGuiWidgets.hpp>
#include <System/Dialogs/MessageBox.hpp>
#include <System/Dialogs/OpenFileDialog.hpp>
#include <System/Dialogs/SaveFileDialog.hpp>
#include <System/Dialogs/SelectFolderDialog.hpp>
#include <Utility/DialogUtils.hpp>
//#include <Utility/GZip.hpp>

NS_ENIGMA_BEGIN

EncryptFileScene::EncryptFileScene()
    : Enigma::Scene(),
      m_type(Algorithm::Type::AES) // default
{
}

void EncryptFileScene::OnCreate() {
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

void EncryptFileScene::OnUpdate(const float& /*dt*/) {}

void EncryptFileScene::OnDraw() {
  // Clear GL buffers
  //glAssert(glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}

void EncryptFileScene::OnImGuiDraw() {
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

  static constexpr const auto container_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove; // | ImGuiWindowFlags_NoBackground;

  // Push window's background color
  ImGui::PushStyleColor(ImGuiCol_WindowBg, Constants::Colors::BACKGROUND_COLOR);

  ImGui::Begin("Container", nullptr, container_flags);
  ImGui::SetWindowSize(ImVec2(static_cast<float>(win_w), static_cast<float>(win_h))); // same size as window
  ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));                                            // top left
  {
#pragma region Back button [<] & Scene Title
    static const auto& title_font = font_audiowide_regular_30;
    static const std::string title = ("Encrypt File");
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


    // Algorithm To encrypt File with
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

#if 0
		spacing(2);

		// Compression widget
		ImGui::PushFont(font_montserrat_medium_16);
		{
			ImGui::PushFont(font_audiowide_regular_20);
			ImGui::Text("Compress file with gzip before encrypting:");
			ImGui::PopFont();
			inline_dummy(6.0f, 0.0f);
			ImGui::SameLine();
			ImGui::Checkbox("##compress_checkbox", &m_compress);
		}
		ImGui::PopFont();
#endif

    spacing(2);

    // Save to database widget
    ImGui::PushFont(font_montserrat_medium_16);
    {
      ImGui::PushFont(font_audiowide_regular_20);
      ImGui::Text("%s:", ("Save to database"));
      ImGui::PopFont();
      inline_dummy(6.0f, 0.0f);
      ImGui::SameLine();
      ImGui::Checkbox("##savetodb_checkbox", &m_save_to_database);

      if (m_save_to_database) {
        ImGui::Text("%s:", ("Encryption Title"));
        ImGuiWidgets::InputTextWithHint("##idb", ("(example > An important video) helps with searching through encryption records in the future"), &m_db_title, win_w / 1.3f);
      }
    }
    ImGui::PopFont();


    spacing(3);
    ImGui::Separator();
    spacing(3);

    // In File to Encrypt
    ImGui::PushFont(font_montserrat_medium_20);
    {
      // Label
      ImGui::Text("%s:", ("File To Encrypt"));
      // Encrypted text
      static const ImVec2 browse_button_size(win_w * 0.10f, 25.0f);
      ImGuiWidgets::InputText("##text1", &m_in_filename, win_w * 0.85f);
      ImGui::PushFont(font_montserrat_medium_12);
      ImGui::SameLine();
      ImGui::PushStyleColor(ImGuiCol_Button, Constants::Colors::BUTTON_COLOR);              // buttons color idle
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Constants::Colors::BUTTON_COLOR_HOVER); // buttons color hover
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, Constants::Colors::BUTTON_COLOR_ACTIVE); // buttons color pressed
      ImGui::PushID("Browse1");
      if (ImGui::Button(("Browse"), browse_button_size)) {
        Application::getInstance()->LaunchWorkerThread(this, ("Browsing input file..."), [this]() -> void {
          this->OnBrowseInFileButtonPressed();
        });
      }
      ImGui::PopID();
      ImGui::PopStyleColor(3);
      ImGui::PopFont();
    }
    ImGui::PopFont();

    spacing(2);

    // Out File Encrypted
    if (!m_in_filename.empty()) {
      ImGui::PushFont(font_montserrat_medium_20);
      {
        // Label
        ImGui::Text("%s:", ("Encrypted File Location"));

        // Encrypted text
        static const ImVec2 browse_button_size(win_w * 0.10f, 25.0f);
        ImGuiWidgets::InputText("##text2", &m_out_filename, win_w * 0.85f);
        ImGui::PushFont(font_montserrat_medium_12);
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, Constants::Colors::BUTTON_COLOR);              // buttons color idle
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Constants::Colors::BUTTON_COLOR_HOVER); // buttons color hover
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, Constants::Colors::BUTTON_COLOR_ACTIVE); // buttons color pressed
        ImGui::PushID("Browse2");
        if (ImGui::Button(("Browse"), browse_button_size)) {
          Application::getInstance()->LaunchWorkerThread(this, ("Browsing output file location..."), [this]() -> void {
            this->OnBrowseOutFileLocationButtonPressed();
          });
        }
        ImGui::PopID();
        ImGui::PopFont();
        ImGui::PopStyleColor(3);
      }
      ImGui::PopFont();
    }


    spacing(3);
    ImGui::Separator();
    spacing(3);

    // Encryption Password & Confirm password
    ImGui::PushFont(font_montserrat_medium_20);
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
      ImGuiWidgets::InputText("##text3", &m_password, static_cast<float>(win_w), ImGuiInputTextFlags_::ImGuiInputTextFlags_Password);
      ImGui::Text("%s:", ("Confirm Password"));
      ImGuiWidgets::InputText("##text4", &m_confirm_password, static_cast<float>(win_w), ImGuiInputTextFlags_::ImGuiInputTextFlags_Password);
      ImGui::PopStyleColor();
      // Bytes count
      ImGui::PushFont(font_montserrat_medium_12);
      //ImGui::Text("%zu bytes", m_password.size());
      ImGui::Text("%s", SizeUtils::FriendlySize(m_password.size()).c_str());
      ImGui::PopFont();
    }
    ImGui::PopFont();


    spacing(3);
    ImGui::Separator();
    spacing(3);


    // Encrypt Button
    {
      ImGui::PushFont(font_audiowide_regular_20);                                           // buttons font
      ImGui::PushStyleColor(ImGuiCol_Button, Constants::Colors::BUTTON_COLOR);              // buttons color idle
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Constants::Colors::BUTTON_COLOR_HOVER); // buttons color hover
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, Constants::Colors::BUTTON_COLOR_ACTIVE); // buttons color pressed
      {
        ImGui::SetCursorPosX((io.DisplaySize.x - button_size.x) / 2.0f);
        //ImGui::SetCursorPosY((io.DisplaySize.y - button_size.y) - 10.0f);
        if (ImGui::Button(("Encrypt"), button_size)) {
          Application::getInstance()->LaunchWorkerThread(this, ("Encrypting file..."), [this]() -> void {
            this->OnEncryptButtonPressed();
          });
        }
      }
      ImGui::PopStyleColor(3);
      ImGui::PopFont();
    }
    // Out File Encrypted
    /*ImGui::PushFont(font_montserrat_medium_20);
		{
			// Label
			ImGui::Text("Save Encrypted File To:");
			// Encrypted text
			static const ImVec2 browse_button_size(45.0f, 25.0f);
			ImGuiUtils::InputText("##text2", &m_out_file_path, win_w - (browse_button_size.x * 1.5f));
			ImGui::PushFont(font_montserrat_medium_12);
			ImGui::SameLine();
			if (ImGui::Button("Browse ", browse_button_size))
			{
				this->OnBrowseOutFileButtonClicked();
			}
			ImGui::PopFont();
		}
		ImGui::PopFont();*/
  }
  ImGui::End();

  // Pop window's background color
  ImGui::PopStyleColor(1);
}

void EncryptFileScene::OnEvent(Event& event) {
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

void EncryptFileScene::OnDestroy() {
  ENIGMA_TRACE_CURRENT_FUNCTION();
}

void EncryptFileScene::OnBrowseInFileButtonPressed() {
  const auto ofd = std::make_unique<Enigma::OpenFileDialog>(
    ("Select A File To Encrypt"),
    m_in_filename, // initial path
    false          // disable multi-select
  );

  ENIGMA_TRACE("Selecting a file to encrypt...");
  const std::vector<std::string> selected_file_paths = ofd->Show();
  if (selected_file_paths.empty()) {
    ENIGMA_TRACE("Nothing is selected.");
    return;
  }
  m_in_filename = *selected_file_paths.begin();

  if (m_out_filename.empty()) {
    m_out_filename = m_in_filename + ".enigma";
  }
}

void EncryptFileScene::OnBrowseOutFileLocationButtonPressed() {
  const auto sfd = std::make_unique<Enigma::SaveFileDialog>(
    ("Select A Location To Save Encrypted File To"),
    m_in_filename, // initial path
    true);

  ENIGMA_TRACE("Selecting a location to save encrypted file to...");
  const std::string selected_location = sfd->Show();
  if (selected_location.empty()) {
    ENIGMA_TRACE("Nothing is selected.");
    return;
  }

  m_out_filename = (fs::path(selected_location) / fs::path(fs::path(m_in_filename).filename().string() + ".enigma")).string();
  //m_out_filename = selected_location + '/' + fs::path(m_in_filename).filename().string() + ".enigma";
}
static std::string extractFileExtension(const fs::path& filename) {
  fs::path fname = filename;
  while (fname.extension().string() == ".enigma") {
    fname.replace_extension("");
  }
  return fname.extension().string();
}
void EncryptFileScene::OnEncryptButtonPressed() {
  // Validate fields
  if (m_in_filename.empty()) {
    (void) DialogUtils::Warn(("Input File to encrypt is empty"));
    return;
  }
  if (!fs::exists(m_in_filename)) {
    (void) DialogUtils::Warn(fmt::format("File {} does not exist", m_in_filename));
    return;
  }
  if (!fs::is_regular_file(m_in_filename)) {
    (void) DialogUtils::Warn(fmt::format("File {} is not a regular file", m_in_filename));
    return;
  }
  if (fs::is_empty(m_in_filename)) {
    (void) DialogUtils::Warn(fmt::format("File {} is empty", m_in_filename));
    return;
  }
  //outfile checks
  if (m_out_filename.empty()) {
    (void) DialogUtils::Warn(("Out File location is empty"));
    return;
  }
  if (!fs::path(m_out_filename).has_filename() || !fs::path(m_out_filename).has_extension()) {
    (void) DialogUtils::Warn(("Please specify a filename and extension to output file"));
    return;
  }
  //pass checks
  if (m_password.empty() || m_confirm_password.empty()) {
    (void) DialogUtils::Warn(("Encryption password is empty"));
    return;
  }
  if (m_password.size() < Constants::Algorithm::MINIMUM_PASSWORD_LENGTH) {
    (void) DialogUtils::Warn(fmt::format("Password is too weak! consider using {} characters or more including special characters like {}", Constants::Algorithm::MINIMUM_PASSWORD_LENGTH, Constants::Algorithm::SPECIAL_CHARACTERS));
    return;
  }
  if (m_password != m_confirm_password) {
    (void) DialogUtils::Warn(("Password doesn't match confirm password"));
    return;
  }

  // Alles gut
  try {
    // Create encryptor based on selected algorithm type
    const auto algorithm = Algorithm::CreateFromType(m_type, Algorithm::Intent::Encrypt);
    ENIGMA_ASSERT_OR_THROW(algorithm, ("Failed to create algorithm from type"));

    // Encrypt file buffer
    algorithm->Encrypt(m_password, fs::path(m_in_filename), fs::path(m_out_filename));

    // Save to database (in chunks)
    // Save to database
    if (m_save_to_database) {
      SQLite::Transaction transaction{*Database::getStorage()};

      // Encryption
      ENIGMA_ASSERT_OR_THROW(ENIGMA_IS_BETWEEN(m_db_title.size(), 3, 255), ("Encryption title must be between 3 and 255 characters"));
      auto e = std::make_unique<Encryption>();
      e->title = m_db_title;
      e->is_file = true;
      e->file_ext = extractFileExtension(m_out_filename);
      e->size = fs::file_size(m_out_filename);
      e->ide = Database::addEncryption(e);
      ENIGMA_ASSERT_OR_THROW(e->ide >= 0, ("Failed to save encryption record to database"));

      // CipherChunks
      bool ok = true;
      std::int64_t offset{0};
      FileUtils::ReadChunks(m_out_filename, Meta::ENIGMA_BUFFER_DEFAULT_SIZE, [ide = e->ide, &offset, &ok](std::vector<byte>&& bytes) -> bool {
        auto cc = std::make_unique<CipherChunk>();
        cc->ide = ide;
        cc->offset = offset;
        cc->size = bytes.size();
        cc->bytes = std::move(bytes);
        cc->idc = Database::addCipherChunk(cc);

        ok &= cc->idc >= 0;
        offset += cc->size;
        return ok;
      });

      if (ok) {
        transaction.commit();
      } else {
        throw std::runtime_error("Failed to save cipher chunk to database");
      }
    }

    // Alert user that encryption was successful
    const std::string msg = fmt::format("Encrypted {} to {} successfully",
                                        fs::path(m_in_filename).filename(),
                                        fs::path(m_out_filename).filename());
    ENIGMA_INFO(msg);
    (void) DialogUtils::Info(msg);

  } catch (const CryptoPP::Exception& e) {
    const std::string err_msg = CryptoPPUtils::GetFullErrorMessage(e);
    ENIGMA_ERROR("Encryption Failure: {0}", err_msg);
    (void) DialogUtils::Error(("Encryption Failure"), err_msg);
  } catch (const std::exception& e) {
    ENIGMA_ERROR("Encryption Failure: {0}", e.what());
    (void) DialogUtils::Error(("Encryption Failure"), e.what());
  } catch (...) {
    const std::string err_msg = ("Encryption Failure: UNKNOWN ERROR");
    ENIGMA_ERROR(err_msg);
    (void) DialogUtils::Error(err_msg);
  }
}

void EncryptFileScene::OnBackButtonPressed() {
  //if (!m_in_filename.empty() || !m_out_filename.empty() || !m_password.empty() || !m_confirm_password.empty())
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
