#include <pch.hpp>
#include "MainMenuScene.hpp"

#include "EncryptFileScene.hpp"
#include "DecryptFileScene.hpp"
#include "EncryptTextScene.hpp"
#include "DecryptTextScene.hpp"
#include "MyEncryptionsScene.hpp"

#include <GUI/ImGuiWidgets.hpp>
#include <Utility/DialogUtils.hpp>
#include <Networking/CheckForUpdates.hpp>

NS_ENIGMA_BEGIN

MainMenuScene::MainMenuScene() noexcept
	:
	Enigma::Scene()
{
	ENIGMA_TRACE(ENIGMA_CURRENT_FUNCTION);
}

void MainMenuScene::OnCreate()
{
	ENIGMA_TRACE(ENIGMA_CURRENT_FUNCTION);

	// Explicit OpenGL old method to et background clear color
	//glAssert(glClearColor(
	//	Constants::Colors::BACKGROUND_COLOR.x,
	//	Constants::Colors::BACKGROUND_COLOR.y,
	//	Constants::Colors::BACKGROUND_COLOR.z,
	//	Constants::Colors::BACKGROUND_COLOR.w
	//));

}

void MainMenuScene::OnUpdate(const f32&) {}

void MainMenuScene::OnDraw()
{
	// Clear GL buffers
	//glAssert(glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}


void MainMenuScene::OnImGuiDraw()
{
	const auto& [win_w, win_h] = Application::GetInstance()->GetWindow()->GetSize();
	const auto& [win_x, win_y] = Application::GetInstance()->GetWindow()->GetPosition();
	static const auto& io = ImGui::GetIO();

	const auto button_size = Vec2f(win_w / 2.5f, 45.0f);

	static constexpr const auto spacing = [](const ui8& n) noexcept { for (ui8 i = 0; i < n; i++) ImGui::Spacing(); };

	const auto& fonts = Application::GetInstance()->GetFonts();
	static ImFont* const& font_montserrat_medium_12 = fonts.at("Montserrat-Medium-12");
	static ImFont* const& font_audiowide_regular_60 = fonts.at("Audiowide-Regular-60");
	static ImFont* const& font_audiowide_regular_45 = fonts.at("Audiowide-Regular-45");
	static ImFont* const& font_audiowide_regular_20 = fonts.at("Audiowide-Regular-20");

	static constexpr const auto container_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;// | ImGuiWindowFlags_NoBackground;
	
	// Push window's background color
	ImGui::PushStyleColor(ImGuiCol_WindowBg, Constants::Colors::BACKGROUND_COLOR);
	// Push Menu bar background color, see issue #3637
	ImGui::PushStyleColor(ImGuiCol_MenuBarBg, Constants::Colors::MENUBAR_BACKGROUND_COLOR);


	ImGui::Begin("MainMenuContainer", nullptr, container_flags);
	ImGui::SetWindowSize(ImVec2(static_cast<f32>(win_w), static_cast<f32>(win_h))); // same size as main app window
	ImGui::SetWindowPos(ImVec2(0.0f, 0.0f)); // top left
	{
		// Menu bar
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 22.0f)); // 16.0f Menu bar padding
			ImGui::PushFont(font_audiowide_regular_20);
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Menu"))
				{
					if (ImGui::MenuItem("Exit")) { Scene::EndScene(); }
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Help"))
				{
					if (ImGui::MenuItem("Report issue")) { this->OnReportIssueMenuButtonPressed(); }
					if (ImGui::MenuItem("Check for updates")) { this->OnCheckForUpdatesMenuButtonPressed(); }
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
				ImGui::EndMenuBar();
			}
			ImGui::PopFont();
			ImGui::PopStyleVar(1);
		}

		spacing(6);

		// Enigma Version
		{
			ImGui::PushFont(font_audiowide_regular_60); // text font
			ImGui::PushStyleColor(ImGuiCol_Text, Constants::Colors::TEXT_COLOR); // text color
			{
				static constexpr const auto title = "Enigma";
				static const ImVec2 title_size = { ImGui::CalcTextSize(title).x * font_audiowide_regular_60->Scale,ImGui::CalcTextSize(title).y * font_audiowide_regular_60->Scale };
				ImGui::SetCursorPosX((io.DisplaySize.x - title_size.x) / 2.0f);
				ImGui::Text(title);
				//static const ImVec2 label_size = { ImGui::CalcTextSize("Enigma x.y.z").x * font_audiowide_regular_45->Scale, ImGui::CalcTextSize("Enigma x.y.z").y * font_audiowide_regular_45->Scale };
				//ImGui::SetCursorPosX((io.DisplaySize.x - label_size.x) / 2.0f);
				//ImGui::Text("Enigma %s", ENIGMA_VERSION);
			}
			ImGui::PopStyleColor(1);
			ImGui::PopFont();
		}

		spacing(9);

		// Buttons
		{
			ImGui::PushFont(font_audiowide_regular_20); // buttons font
			ImGui::PushStyleColor(ImGuiCol_Button, Constants::Colors::BUTTON_COLOR); // buttons color idle
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Constants::Colors::BUTTON_COLOR_HOVER);  // buttons color hover
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, Constants::Colors::BUTTON_COLOR_ACTIVE); // buttons color pressed
			{
				ImGui::SetCursorPosX((io.DisplaySize.x - button_size.x) / 2.0f);
				if (ImGui::Button("Encrypt File", button_size))
				{
					this->OnEncryptFileButtonPressed();
				}
				ImGui::SetCursorPosX((io.DisplaySize.x - button_size.x) / 2.0f);
				if (ImGui::Button("Decrypt File", button_size))
				{
					this->OnDecryptFileButtonPressed();
				}
				spacing(6);
				ImGui::SetCursorPosX((io.DisplaySize.x - button_size.x) / 2.0f);
				if (ImGui::Button("Encrypt Text", button_size))
				{
					this->OnEncryptTextButtonPressed();
				}
				ImGui::SetCursorPosX((io.DisplaySize.x - button_size.x) / 2.0f);
				if (ImGui::Button("Decrypt Text", button_size))
				{
					this->OnDecryptTextButtonPressed();
				}
				spacing(6);
				{
					ImGui::PushStyleColor(ImGuiCol_Button, Constants::Colors::MY_ENCRYPTIONS_BUTTON_COLOR); // buttons color idle
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Constants::Colors::MY_ENCRYPTIONS_BUTTON_COLOR_HOVER);  // buttons color hover
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, Constants::Colors::MY_ENCRYPTIONS_BUTTON_COLOR_ACTIVE); // buttons color pressed
					ImGui::SetCursorPosX((io.DisplaySize.x - button_size.x) / 2.0f);
					if (ImGui::Button("My Encryptions", button_size))
					{
						this->OnMyEncryptionsButtonPressed();
					}
					ImGui::PopStyleColor(3);
				}
				spacing(9);
				{
					ImGui::SetCursorPosX((io.DisplaySize.x - button_size.x) / 2.0f);
					ImGui::SetCursorPosY((io.DisplaySize.y - button_size.y) - 20.0f);
					if (ImGuiWidgets::Button("Exit", button_size, Constants::Colors::BACK_BUTTON_COLOR, Constants::Colors::BACK_BUTTON_COLOR_HOVER, Constants::Colors::BACK_BUTTON_COLOR_ACTIVE))
					{
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

void MainMenuScene::OnEvent(Enigma::Event&)
{
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

void MainMenuScene::OnDestroy()
{
	ENIGMA_TRACE(ENIGMA_CURRENT_FUNCTION);
}

void MainMenuScene::OnEncryptFileButtonPressed()
{
	Application::GetInstance()->PushScene(std::make_shared<EncryptFileScene>());
}

void MainMenuScene::OnDecryptFileButtonPressed()
{
	Application::GetInstance()->PushScene(std::make_shared<DecryptFileScene>());
}

void MainMenuScene::OnEncryptTextButtonPressed()
{
	Application::GetInstance()->PushScene(std::make_shared<EncryptTextScene>());
}

void MainMenuScene::OnDecryptTextButtonPressed()
{
	Application::GetInstance()->PushScene(std::make_shared<DecryptTextScene>());
}

void MainMenuScene::OnMyEncryptionsButtonPressed()
{
	Application::GetInstance()->PushScene(std::make_shared<MyEncryptionsScene>());
}

void MainMenuScene::OnReportIssueMenuButtonPressed()
{
	// Open Enigma's github repository issues page in a browser
	const String url = Constants::Links::ENIGMA_GITHUB_REPOSITORY + "/issues";
	ENIGMA_TRACE("Reporting issue to {0}", url);

#if defined(ENIGMA_PLATFORM_WINDOWS)
	
	(void)ShellExecuteA(nullptr, "open", url.c_str() , nullptr, nullptr, SW_SHOWNORMAL);

#elif defined(ENIGMA_PLATFORM_LINUX)

	const String cmd = "xdg-open " + url;
	(void)std::system(cmd.c_str());

#else

	(void)DialogUtils::Info("If you face any problems feel free to open an issue at " + url);

#endif
}

void MainMenuScene::OnCheckForUpdatesMenuButtonPressed()
{
	ENIGMA_TRACE(ENIGMA_CURRENT_FUNCTION);
	
	ENIGMA_TRACE("Retrieving Enigma's latest release info from {0}...", Enigma::Constants::Links::ENIGMA_GITHUB_API_LATEST_RELEASE);
	const auto info = CheckForUpdates::GetLatestReleaseInfo();
	if (!info) 
		return;
	
	const auto current_version = "v" + String(Enigma::ENIGMA_VERSION);
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
}

void MainMenuScene::OnAboutMenuButtonPressed()
{
	// Show about dialog
	std::ostringstream oss{};
	oss << "# Version: \n" << ENIGMA_VERSION << "\n\n"
		<< "# Github Repository: \n" << Constants::Links::ENIGMA_GITHUB_REPOSITORY << "\n\n"
		<< "# License: \n" << ENIGMA_LICENSE;
	(void)DialogUtils::Info(oss.str());
	
}



NS_ENIGMA_END
