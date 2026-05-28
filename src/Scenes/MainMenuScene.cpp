#include <pch.hpp>
#include "MainMenuScene.hpp"
#include "DecryptFileScene.hpp"
#include "DecryptTextScene.hpp"
#include "EncryptFileScene.hpp"
#include "EncryptTextScene.hpp"
//#include "Logo.hpp"
#include "icons/Dashboard.hpp"
#include "icons/Encryption.hpp"
#include "icons/Vault.hpp"
#include "icons/Tools.hpp"
#include "icons/Settings.hpp"
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


  m_currentScene = Application::GetOrCreateScene<DashboardScene>();


  SideBarWidget::Item dashboard{};
  dashboard.icon = std::make_shared<ImageWidgets::Image>(Textures::Dashboard_png, Textures::Dashboard_png_len);
  dashboard.title = "Dashboard";
  SideBarWidget::Item encryption{};
  encryption.icon = std::make_shared<ImageWidgets::Image>(Textures::Encryption_png, Textures::Encryption_png_len);
  encryption.title = "Encryption";
  SideBarWidget::Item vault{};
  vault.icon = std::make_shared<ImageWidgets::Image>(Textures::Vault_png, Textures::Vault_png_len);
  vault.title = "Vault";
  SideBarWidget::Item tools{};
  tools.icon = std::make_shared<ImageWidgets::Image>(Textures::Tools_png, Textures::Tools_png_len);
  tools.title = "Tools";
  SideBarWidget::Item settings{};
  settings.icon = std::make_shared<ImageWidgets::Image>(Textures::Settings_png, Textures::Settings_png_len);
  settings.title = "Settings";

  m_sidebar = std::make_unique<SideBarWidget>(this);
  m_sidebar->setItems({dashboard, encryption, vault, tools, settings});
  m_sidebar->setOnItemSelected([this](std::size_t idx) {
    ENIGMA_TRACE("SideBarWidget - Selected {}", idx);
    switch (idx) {
      case 0: // Dashboard
      {
        m_currentScene = Application::GetOrCreateScene<DashboardScene>();
        break;
      }
      case 1: // Encryption
      {
        //@todo: m_currentScene = Application::GetOrCreateScene<EncryptionScene>();
        break;
      }
      case 2: // Vault
      {
        //@todo: m_currentScene = Application::GetOrCreateScene<VaultScene>();
        break;
      }
      case 3: // Tools
      {
        //@todo: m_currentScene = Application::GetOrCreateScene<ToolsScene>();
        break;
      }
      case 4: // Settings
      {
        //@todo: m_currentScene = Application::GetOrCreateScene<SettingsScene>();
        break;
      }
      default:
        ENIGMA_ASSERT(false, "Unhandled side bar item index #{}", idx);
        break;
    }
  });

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

  static constexpr auto spacing = [](const std::uint8_t& n) noexcept { for (std::uint8_t i = 0; i < n; i++) ImGui::Spacing(); };

  static ImFont *const& font_ubuntu_regular_60 = ResourceManager::getFont("Ubuntu-Regular-60");
  static ImFont *const& font_ubuntu_regular_45 = ResourceManager::getFont("Ubuntu-Regular-45");
  static ImFont *const& font_ubuntu_regular_30 = ResourceManager::getFont("Ubuntu-Regular-30");
  static ImFont *const& font_ubuntu_regular_20 = ResourceManager::getFont("Ubuntu-Regular-20");
  static ImFont *const& font_ubuntu_regular_18 = ResourceManager::getFont("Ubuntu-Regular-18");
  static ImFont *const& font_ubuntu_regular_16 = ResourceManager::getFont("Ubuntu-Regular-16");
  static ImFont *const& font_ubuntu_regular_14 = ResourceManager::getFont("Ubuntu-Regular-14");
  static ImFont *const& font_ubuntu_regular_12 = ResourceManager::getFont("Ubuntu-Regular-12");


  static constexpr auto container_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove; // | ImGuiWindowFlags_NoBackground;

  // Push window's background color
  ImGui::PushStyleColor(ImGuiCol_WindowBg, Constants::Colors::BACKGROUND_COLOR);
  ImGui::Begin("#MainMenuContainer", nullptr, container_flags);
  {
    ImGui::SetWindowSize(ImVec2(static_cast<float>(win_w), static_cast<float>(win_h))); // same size as main app window
    ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));                                            // top left

    m_sidebar->Draw(); // left side bar
    //@todo m_currentScene->Draw(); // context scene
    if (auto contextScene = m_currentScene.lock()) {
      contextScene->OnImGuiDraw();
    }
  }
  ImGui::End();
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
