#include <pch.hpp>
#include "MainMenuScene.hpp"
#include "DecryptFileScene.hpp"
#include "DecryptTextScene.hpp"
#include "EncryptFileScene.hpp"
#include "EncryptTextScene.hpp"
#include "MyEncryptionsScene.hpp"
#include "ToolsScene/ToolsScene.hpp"
#include "Utility/DateTimeUtils.hpp"

#include <GUI/ImGuiWidgets.hpp>
#include <Networking/CheckForUpdates.hpp>
#include <Utility/DialogUtils.hpp>

//#include <Translation/Translation.hpp>

NS_ENIGMA_BEGIN

MainMenuScene::MainMenuScene() noexcept
    : Enigma::Scene() {
  ENIGMA_TRACE_CURRENT_FUNCTION();
}

void MainMenuScene::OnCreate() {
  ENIGMA_TRACE_CURRENT_FUNCTION();

  // Explicit OpenGL old method to et background clear color
  //glAssert(glClearColor(
  //	Constants::Colors::BACKGROUND_COLOR.x,
  //	Constants::Colors::BACKGROUND_COLOR.y,
  //	Constants::Colors::BACKGROUND_COLOR.z,
  //	Constants::Colors::BACKGROUND_COLOR.w
  //));

  // Load logo image
  //m_logo = std::make_unique<ImGuiWidgets::Image>(Constants::Resources::Textures::ENIGMA_LOGO_PNG_PATH);
}

void MainMenuScene::OnUpdate(const float&) {}

void MainMenuScene::OnDraw() {
  // Clear GL buffers
  //glAssert(glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}


void MainMenuScene::OnImGuiDraw() {
  const auto& [win_w, win_h] = Application::getInstance()->GetWindow()->GetSize();
  const auto& [win_x, win_y] = Application::getInstance()->GetWindow()->GetPosition();
  static const auto& io = ImGui::GetIO();

  const auto button_size = ImVec2(win_w / 2.5f, 45.0f);

  static constexpr const auto spacing = [](const std::uint8_t& n) noexcept { for (std::uint8_t i = 0; i < n; i++) ImGui::Spacing(); };

  static ImFont *const& font_ubuntu_regular_60 = ResourceManager::getFont("Ubuntu-Regular-60");
  static ImFont *const& font_ubuntu_regular_45 = ResourceManager::getFont("Ubuntu-Regular-45");
  static ImFont *const& font_ubuntu_regular_30 = ResourceManager::getFont("Ubuntu-Regular-30");
  static ImFont *const& font_ubuntu_regular_20 = ResourceManager::getFont("Ubuntu-Regular-20");
  static ImFont *const& font_ubuntu_regular_18 = ResourceManager::getFont("Ubuntu-Regular-18");
  static ImFont *const& font_ubuntu_regular_16 = ResourceManager::getFont("Ubuntu-Regular-16");
  static ImFont *const& font_ubuntu_regular_14 = ResourceManager::getFont("Ubuntu-Regular-14");
  static ImFont *const& font_ubuntu_regular_12 = ResourceManager::getFont("Ubuntu-Regular-12");


  static constexpr const auto container_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove; // | ImGuiWindowFlags_NoBackground;

  // Push window's background color
  ImGui::PushStyleColor(ImGuiCol_WindowBg, Constants::Colors::BACKGROUND_COLOR);
  // Push Menu bar background color, see issue #3637
  ImGui::PushStyleColor(ImGuiCol_MenuBarBg, Constants::Colors::MENUBAR_BACKGROUND_COLOR);


  ImGui::Begin("MainMenuContainer", nullptr, container_flags);
  ImGui::SetWindowSize(ImVec2(static_cast<float>(win_w), static_cast<float>(win_h))); // same size as main app window
  ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));                                            // top left
  {
    // Menu bar
    {
      ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 22.0f)); // 22.0f Menu bar padding
      ImGui::PushFont(font_ubuntu_regular_20);
      if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
          if (ImGui::MenuItem("Export")) { this->OnExportEncryptionsButtonPressed(); }

          if (ImGui::MenuItem("Import")) { this->OnImportEncryptionsButtonPressed(); }

          if (ImGui::MenuItem("Exit", "CTRL+E")) { Scene::EndScene(); }

          ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View")) {
          if (ImGui::MenuItem("Encrypt File", "E+F")) { this->OnEncryptFileButtonPressed(); }

          if (ImGui::MenuItem("Decrypt File", "D+F")) { this->OnDecryptFileButtonPressed(); }

          if (ImGui::MenuItem("Encrypt Text", "E+T")) { this->OnEncryptTextButtonPressed(); }

          if (ImGui::MenuItem("Decrypt Text", "D+T")) { this->OnDecryptTextButtonPressed(); }

          if (ImGui::MenuItem("Tools", "CTRL+T")) { this->OnToolsButtonPressed(); }

          ImGui::EndMenu();
        }

        /*
				if (ImGui::BeginMenu(("Language")))
				{
					for (std::uint16_t i = static_cast<std::uint16_t>(Translation::Language::BEGIN); i <= static_cast<std::uint16_t>(Translation::Language::END); ++i)
					{
						bool selected = Translation::GetLanguage() == static_cast<Translation::Language>(i);
						if (ImGui::MenuItem(Translation::StringifyLanguageEnum(static_cast<Translation::Language>(i)).c_str(), nullptr, &selected))
						{
							Translation::SetLanguage(static_cast<Translation::Language>(i));
						}
					}
					ImGui::EndMenu();
				}
				*/
        if (ImGui::BeginMenu("Help")) {
          if (ImGui::MenuItem("Report an issue")) {
            Application::getInstance()->LaunchWorkerThread(this, ("Reporting issue..."), [this]() -> void {
              this->OnReportIssueMenuButtonPressed();
            });
          }
          if (ImGui::MenuItem("Check for updates")) {
            Application::getInstance()->LaunchWorkerThread(this, ("Checking for updates..."), [this]() -> void {
              this->OnCheckForUpdatesMenuButtonPressed();
            });
          }
          if (ImGui::MenuItem("About")) { this->OnAboutMenuButtonPressed(); }
          ImGui::EndMenu();
        }

        /*if (ImGui::BeginMenu("Examples"))
				{
					ImGui::MenuItem("Example 1");
					ImGui::MenuItem("Example 2");
					ImGui::MenuItem("Example 3");
					ImGui::MenuItem("Example 4");
					ImGui::EndMenu();
				}*/

        // Process Menu items shortcuts
        {
          const static std::map<std::pair<KeyCode, KeyCode>, void (MainMenuScene::*)()> shortcuts =
            {
              // Encrypt File E+F
              {{KeyCode::E, KeyCode::F}, &MainMenuScene::OnEncryptFileButtonPressed},
              // Decrypt File D+F
              {{KeyCode::D, KeyCode::F}, &MainMenuScene::OnDecryptFileButtonPressed},
              // Encrypt Text E+T
              {{KeyCode::E, KeyCode::T}, &MainMenuScene::OnEncryptTextButtonPressed},
              // Decrypt Text D+T
              {{KeyCode::D, KeyCode::T}, &MainMenuScene::OnDecryptTextButtonPressed},
              // Tools CTRL+T
              {{KeyCode::LeftControl, KeyCode::T}, &MainMenuScene::OnToolsButtonPressed},
              // Exit CTRL+E
              {{KeyCode::LeftControl, KeyCode::E}, &MainMenuScene::EndScene},

              // About CTRL+A
              {{KeyCode::LeftControl, KeyCode::A}, &MainMenuScene::OnAboutMenuButtonPressed},
            };

          for (const auto& [keys, method_ptr]: shortcuts) {
            if (Input::AreKeysPressed({keys.first, keys.second})) {
#ifdef ENIGMA_DEBUG
              ENIGMA_LOG("Menu item shortcut pressed {}+{}", keys.first, keys.second);
#endif
              (*this.*method_ptr)();
            }
          }
        }


        ImGui::EndMenuBar();
      }
      ImGui::PopFont();
      ImGui::PopStyleVar(1);
    }

    spacing(3);

#if true // use text
    // Enigma Version
    {
      ImGui::PushFont(font_ubuntu_regular_60);                          // text font
      ImGui::PushStyleColor(ImGuiCol_Text, Constants::Colors::TEXT_COLOR); // text color
      {

        static constexpr const auto title = "Enigma";
        static const ImVec2 title_size = {ImGui::CalcTextSize(title).x * font_ubuntu_regular_60->Scale, ImGui::CalcTextSize(title).y * font_ubuntu_regular_60->Scale};
        ImGui::SetCursorPosX((io.DisplaySize.x - title_size.x) / 2.0f);
        ImGui::Text(title);
        //static const ImVec2 label_size = { ImGui::CalcTextSize("Enigma x.y.z").x * font_audiowide_regular_45->Scale, ImGui::CalcTextSize("Enigma x.y.z").y * font_audiowide_regular_45->Scale };
        //ImGui::SetCursorPosX((io.DisplaySize.x - label_size.x) / 2.0f);
        //ImGui::Text("Enigma %s", ENIGMA_VERSION);
      }
      ImGui::PopStyleColor(1);
      ImGui::PopFont();
    }
#else // use logo
    // Enigma Logo
    {
      const float logo_width = static_cast<float>(m_logo->GetWidth());
      const float logo_height = static_cast<float>(m_logo->GetHeight());
      const float logo_x = (win_w - logo_width) / 2.0f;
      const float logo_y = ImGui::GetFrameHeight(); // GetFrameHeight ~=> Menubar height
      m_logo->Draw({logo_x, logo_y}, logo_width, logo_height);
    }
#endif


    spacing(6);

    // Buttons
    {
      ImGui::PushFont(font_ubuntu_regular_20);                                           // buttons font
      ImGui::PushStyleColor(ImGuiCol_Button, Constants::Colors::BUTTON_COLOR);              // buttons color idle
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Constants::Colors::BUTTON_COLOR_HOVER); // buttons color hover
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, Constants::Colors::BUTTON_COLOR_ACTIVE); // buttons color pressed
      {
        ImGui::SetCursorPosX((io.DisplaySize.x - button_size.x) / 2.0f);
        if (ImGui::Button(("Encrypt File"), button_size)) {
          this->OnEncryptFileButtonPressed();
        }
        ImGui::SetCursorPosX((io.DisplaySize.x - button_size.x) / 2.0f);
        if (ImGui::Button(("Decrypt File"), button_size)) {
          this->OnDecryptFileButtonPressed();
        }
        spacing(6);
        ImGui::SetCursorPosX((io.DisplaySize.x - button_size.x) / 2.0f);
        if (ImGui::Button(("Encrypt Text"), button_size)) {
          this->OnEncryptTextButtonPressed();
        }
        ImGui::SetCursorPosX((io.DisplaySize.x - button_size.x) / 2.0f);
        if (ImGui::Button(("Decrypt Text"), button_size)) {
          this->OnDecryptTextButtonPressed();
        }
        spacing(6);
        {
          ImGui::PushStyleColor(ImGuiCol_Button, Constants::Colors::MY_ENCRYPTIONS_BUTTON_COLOR);              // buttons color idle
          ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Constants::Colors::MY_ENCRYPTIONS_BUTTON_COLOR_HOVER); // buttons color hover
          ImGui::PushStyleColor(ImGuiCol_ButtonActive, Constants::Colors::MY_ENCRYPTIONS_BUTTON_COLOR_ACTIVE); // buttons color pressed
          ImGui::SetCursorPosX((io.DisplaySize.x - button_size.x) / 2.0f);
          if (ImGui::Button(("My Encryptions"), button_size)) {
            this->OnMyEncryptionsButtonPressed();
          }
          ImGui::PopStyleColor(3);
        }
        spacing(6);
        {
          ImGui::PushStyleColor(ImGuiCol_Button, Constants::Colors::TOOLS_BUTTON_COLOR);              // buttons color idle
          ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Constants::Colors::TOOLS_BUTTON_COLOR_HOVER); // buttons color hover
          ImGui::PushStyleColor(ImGuiCol_ButtonActive, Constants::Colors::TOOLS_BUTTON_COLOR_ACTIVE); // buttons color pressed
          ImGui::SetCursorPosX((io.DisplaySize.x - button_size.x) / 2.0f);
          if (ImGui::Button(("Tools"), button_size)) {
            this->OnToolsButtonPressed();
          }
          ImGui::PopStyleColor(3);
        }
        spacing(7);

        {
          ImGui::SetCursorPosX((io.DisplaySize.x - button_size.x) / 2.0f);
          //ImGui::SetCursorPosY((io.DisplaySize.y - button_size.y) - 20.0f);
          if (ImGuiWidgets::Button(("Exit"), button_size, Constants::Colors::BACK_BUTTON_COLOR, Constants::Colors::BACK_BUTTON_COLOR_HOVER, Constants::Colors::BACK_BUTTON_COLOR_ACTIVE)) {
            Scene::EndScene();
          }
        }
      }
      ImGui::PopStyleColor(3);
      ImGui::PopFont();
    }
  }
  ImGui::End();

  // Pop Menu bar background color, see issue #3637
  ImGui::PopStyleColor(1);
  // Pop window's background color
  ImGui::PopStyleColor(1);
}

void MainMenuScene::OnEvent(Enigma::Event&) {
  /*
	//Listening to specific event
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<MouseButtonPressedEvent>([](const MouseButtonPressedEvent& e) -> bool
		{
			LOG(e.ToString());

			return true; // handled, return false if other scenes may use the event
		});

	dispatcher.Dispatch<KeyPressedEvent>([](const KeyPressedEvent& e) -> bool
		{
			LOG(e.ToString());

			return true; // handled, return false if other scenes may use the event
		});


	if (Input::IsKeyPressed(Enigma::Key::S))
	{
		const auto& [win_x, win_y] = GetSceneData().window->GetPosition();

		ENIGMA_TRACE("{0} {1}", win_x, win_y);
	}
	*/
};

void MainMenuScene::OnDestroy() {
  ENIGMA_TRACE_CURRENT_FUNCTION();
}

void MainMenuScene::OnEncryptFileButtonPressed() {
  Application::getInstance()->PushScene(std::make_unique<EncryptFileScene>());
}

void MainMenuScene::OnDecryptFileButtonPressed() {
  Application::getInstance()->PushScene(std::make_unique<DecryptFileScene>());
}

void MainMenuScene::OnEncryptTextButtonPressed() {
  Application::getInstance()->PushScene(std::make_unique<EncryptTextScene>());
}

void MainMenuScene::OnDecryptTextButtonPressed() {
  Application::getInstance()->PushScene(std::make_unique<DecryptTextScene>());
}

void MainMenuScene::OnMyEncryptionsButtonPressed() {
  Application::getInstance()->PushScene(std::make_unique<MyEncryptionsScene>());
}

void MainMenuScene::OnToolsButtonPressed() {
  Application::getInstance()->PushScene(std::make_unique<ToolsScene>());
}

void MainMenuScene::OnReportIssueMenuButtonPressed() {
  // Open Enigma's github repository issues page in a browser
  const std::string url = Constants::Links::ENIGMA_GITHUB_REPOSITORY + "/issues";
  ENIGMA_TRACE("Reporting issue to {0}", url);

#if defined(ENIGMA_PLATFORM_WINDOWS)

  [[maybe_unused]] auto _ = ShellExecuteA(nullptr, "open", url.c_str(), nullptr, nullptr, SW_SHOWNORMAL);

#elif defined(ENIGMA_PLATFORM_LINUX)

  const std::string cmd = "xdg-open " + url;
  [[maybe_unused]] auto _ = std::system(cmd.c_str());

#else

  [[maybe_unused]] auto _ = DialogUtils::Info(("If you face any problems feel free to open an issue at {}", url));

#endif
}

void MainMenuScene::OnCheckForUpdatesMenuButtonPressed() {
  ENIGMA_TRACE_CURRENT_FUNCTION();

  // Check for enigma updates from github api --check-for-updates
  ENIGMA_TRACE("Retrieving Enigma's latest release info from {}...", Enigma::Constants::Links::ENIGMA_GITHUB_API_LATEST_RELEASE);
  const auto info = CheckForUpdates::GetLatestReleaseInfo();
  if (!info) {
    (void) DialogUtils::Error("Could not retrieve Enigma's latest release info");
    return;
  }

  const CheckForUpdates::Version current_version(ENIGMA_VERSION);
  const CheckForUpdates::Version& latest_version = info->version;

  LOG_TRACE("Checking version [curr={}, latest={}]", current_version, latest_version);

  std::ostringstream oss{};
  if (current_version == latest_version) {
    oss << "You are using the latest Enigma version " << info->tag_name;
  } else if (latest_version > current_version) {
    oss << "New version " << info->tag_name << " is available! " << " published on " << info->published_at;
    if (!info->body.empty())
      oss << '\n'
          << "What's new?\n"
          << info->body;
    //			<< "- Version: " << info->tag_name << '\n'
    //			<< "- Created At: " << info->created_at << '\n'
    //			<< "- Published At: " << info->published_at << '\n'
    //			<< "- What's new ?: " << info->body << '\n'
    //			<< "- .tar release download url: " << info->tarball_url << '\n'
    //			<< "- .zip release download url: " << info->zipball_url << '\n';
  } else if (latest_version < current_version) [[unlikely]] {
    oss << "This version of Enigma is newer than the latest version available!"
           " there must have been some bug, or you have compiled Enigma with a"
           " higher version than it currently is, please report this issue to "
        << Constants::Links::ENIGMA_GITHUB_REPOSITORY_ISSUES;
  }

  const std::string text = oss.str();
  (void) DialogUtils::Info(text);
  ENIGMA_LOG(text);

#if 0
	ENIGMA_TRACE("Retrieving Enigma's latest release info from {0}...", Enigma::Constants::Links::ENIGMA_GITHUB_API_LATEST_RELEASE);
	const auto info = CheckForUpdates::GetLatestReleaseInfo();
	if (!info) 
		return;
	
	const auto current_version = "v" + std::string(ENIGMA_VERSION);
	std::ostringstream oss{};
	if (info->tag_name == current_version)
	{
		oss << "You are using the latest Enigma version " << current_version;
	}
	else
	{
		oss << "New version is available!\n"
			<< "# Name: " << info->name << '\n'
			<< "# Version: " << info->tag_name << '\n'
			<< "# Created At: " << info->created_at << '\n'
			<< "# Published At: " << info->published_at << '\n'
			<< "# What's new ?: " << info->body << '\n'
			<< "# .tar release download url: " << info->tarball_url << '\n'
			<< "# .zip release download url: " << info->zipball_url << '\n';
	}
	(void)DialogUtils::Info(oss.str());
	ENIGMA_LOG(oss.str());
#endif
}

void MainMenuScene::OnAboutMenuButtonPressed() {
  // Show about dialog
  std::ostringstream oss{};
  oss << "# Version: \n"
      << ENIGMA_VERSION << "\n\n"
      << "# Github Repository: \n"
      << Constants::Links::ENIGMA_GITHUB_REPOSITORY << "\n\n"
      << "# License: \n"
      << ENIGMA_LICENSE;
  (void) DialogUtils::Info(oss.str());
}

void MainMenuScene::OnExportEncryptionsButtonPressed() {
  const fs::path defaultExportFilename = ResourceManager::getResourcesDir() / DateTimeUtils::now("Enigma_export_%Y_%m_%d_%H_%M_%S.db");
  const SaveFileDialog sfd("Export Database to ...", defaultExportFilename.string(), true);
  const std::string outputFilename = sfd.Show();
  if (outputFilename.empty()) return;

  ENIGMA_INFO("Exporting database to {}", outputFilename);
  Application::getInstance()->LaunchWorkerThread(this, "Exporting database to\n" + outputFilename, [outputFilename]() {
    try {
      Database::Export(outputFilename);
      DialogUtils::Info("Backup successfully saved to\n" + outputFilename);
    } catch (const std::exception& e) {
      ENIGMA_ERROR(e.what());
      DialogUtils::Error(e.what());
    }
  });
}
void MainMenuScene::OnImportEncryptionsButtonPressed() {
  const OpenFileDialog ofd("Import Database from ...", ResourceManager::getResourcesDir().string(), false);
  const std::vector<std::string> selectedFiles = ofd.Show();
  if (selectedFiles.empty()) return;
  const std::string& inputFilename = selectedFiles[0];

  if (const auto action = DialogUtils::Question("Importing will overwrite all saved encryptions.\nExport the current database first if needed.\nContinue importing?",
                                                Enigma::MessageBox::Choice::Yes_No_Cancel);
      action != Enigma::MessageBox::Action::Yes) {
    return;
  }

  ENIGMA_INFO("Importing database from {}", inputFilename);

  Application::getInstance()->LaunchWorkerThread(this, "Importing database from\n" + inputFilename, [inputFilename]() {
    try {
      Database::Import(inputFilename);
      DialogUtils::Info("Database successfully imported from\n" + inputFilename);
    } catch (const std::exception& e) {
      ENIGMA_ERROR(e.what());
      DialogUtils::Error(e.what());
    }
  });
}


NS_ENIGMA_END
