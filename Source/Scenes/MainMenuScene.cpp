#include <pch.hpp>
#include "MainMenuScene.hpp"

#include "EncryptFileScene.hpp"
#include "EncryptTextScene.hpp"
#include "DecryptTextScene.hpp"
#include <Utility/DialogUtils.hpp>


MainMenuScene::MainMenuScene() noexcept
	:
	Enigma::Scene()
{
	ENIGMA_TRACE(ENIGMA_CURRENT_FUNCTION);
}

void MainMenuScene::OnCreate()
{
	ENIGMA_TRACE(ENIGMA_CURRENT_FUNCTION);

	// Set background clear color
	glAssert(glClearColor(
		Constants::Colors::BACKGROUND_COLOR.x,
		Constants::Colors::BACKGROUND_COLOR.y,
		Constants::Colors::BACKGROUND_COLOR.z,
		Constants::Colors::BACKGROUND_COLOR.w
	));

	// Load dear fonts
	this->LoadImGuiFonts();

	// ImGui Push Menu bar background color, see issue #3637
	ImGui::PushStyleColor(ImGuiCol_MenuBarBg, Constants::Colors::MENUBAR_BACKGROUND_COLOR);
}

void MainMenuScene::OnUpdate(const f32& dt) {}

void MainMenuScene::OnDraw()
{
	// Clear GL buffers
	glAssert(glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}


void MainMenuScene::OnImGuiDraw()
{
	const auto& [win_w, win_h] = Application::GetInstance()->GetWindow()->GetSize();
	const auto& [win_x, win_y] = Application::GetInstance()->GetWindow()->GetPosition();
	static const auto& io = ImGui::GetIO();

	const auto button_size = Vec2f(win_w / 2.5f, 45.0f);

	static constexpr const auto spacing = [](const ui8& n) noexcept { for (ui8 i = 0; i < n; i++) ImGui::Spacing(); };

	static ImFont* const& font_montserrat_medium_12 = m_fonts.at("Montserrat-Medium-12");
	static ImFont* const& font_audiowide_regular_60 = m_fonts.at("Audiowide-Regular-60");
	static ImFont* const& font_audiowide_regular_45 = m_fonts.at("Audiowide-Regular-45");
	static ImFont* const& font_audiowide_regular_20 = m_fonts.at("Audiowide-Regular-20");

	static constexpr const auto container_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground;
	//static constexpr const auto container_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_MenuBar;
	ImGui::Begin("Container", nullptr, container_flags);
	ImGui::SetWindowSize(ImVec2(static_cast<f32>(win_w), static_cast<f32>(win_h))); // same size as window
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
					if (ImGui::MenuItem("Exit")) { this->EndScene(); }
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
				if (ImGui::BeginMenu("Help"))
				{
					if (ImGui::MenuItem("About")) { this->OnAboutMenuButtonPressed(); }
					ImGui::EndMenu();
				}

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
				spacing(9);
				{
					ImGui::SetCursorPosX((io.DisplaySize.x - button_size.x) / 2.0f);
					if (ImGui::Button("Exit", button_size))
					{
						this->EndScene();
					}
				}
			}
			ImGui::PopStyleColor(3);
			ImGui::PopFont();
		}
	}
	ImGui::End();


}

void MainMenuScene::OnEvent(Enigma::Event& event)
{
	/*
	//Listening to specific event
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<MouseButtonPressedEvent>([](const MouseButtonPressedEvent& e) -> bool
		{
			LOG(e.ToString().c_str());

			return true; // handled, return false if other scenes may use the event
		});

	dispatcher.Dispatch<KeyPressedEvent>([](const KeyPressedEvent& e) -> bool
		{
			LOG(e.ToString().c_str());

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
	ENIGMA_LOG(ENIGMA_CURRENT_FUNCTION);

	// ImGui Pop Menu bar background color, see issue #3637
	ImGui::PopStyleColor(1); 

	m_fonts.clear();

}


void MainMenuScene::LoadImGuiFonts()
{
	static const auto& io = ImGui::GetIO();

	m_fonts["Audiowide-Regular-60"] = io.Fonts->AddFontFromFileTTF(Constants::Resources::Fonts::AUDIOWIDE_FONT_PATH, 60.0f);
	m_fonts["Audiowide-Regular-45"] = io.Fonts->AddFontFromFileTTF(Constants::Resources::Fonts::AUDIOWIDE_FONT_PATH, 45.0f);
	m_fonts["Audiowide-Regular-20"] = io.Fonts->AddFontFromFileTTF(Constants::Resources::Fonts::AUDIOWIDE_FONT_PATH, 20.0f);
	
	m_fonts["Montserrat-Medium-45"] = io.Fonts->AddFontFromFileTTF(Constants::Resources::Fonts::MONTSERRAT_FONT_PATH, 45.0f);
	m_fonts["Montserrat-Medium-20"] = io.Fonts->AddFontFromFileTTF(Constants::Resources::Fonts::MONTSERRAT_FONT_PATH, 20.0f);
	m_fonts["Montserrat-Medium-12"] = io.Fonts->AddFontFromFileTTF(Constants::Resources::Fonts::MONTSERRAT_FONT_PATH, 12.0f);

	io.Fonts->Build(); //Build added fonts atlas --> imgui issue #3643

	// Check if fonts are loaded
	for (const auto& [font_name, font] : m_fonts)
	{
		if (!font->IsLoaded())
		{
			const String err_msg = "Failed to load font " + String(font_name);
			// console alert
			ENIGMA_ERROR(err_msg.c_str());
			// ui alert
			std::unique_ptr<Enigma::MessageBox> msg_box = std::make_unique<Enigma::MessageBox>(
				"Resource Loading Error",
				err_msg,
				Enigma::MessageBox::Icon::Error,
				Enigma::MessageBox::Choice::Ok);
			[[maybe_unused]] auto action = msg_box->Show();
			
			// no further without dear fonts :c
			EndScene();
			break;
		}
	}
}


void MainMenuScene::OnEncryptFileButtonPressed()
{
	Application::GetInstance()->PushScene(std::make_shared<EncryptFileScene>(m_fonts));
}

void MainMenuScene::OnDecryptFileButtonPressed()
{
}

void MainMenuScene::OnEncryptTextButtonPressed()
{
	Application::GetInstance()->PushScene(std::make_shared<EncryptTextScene>(m_fonts));
}

void MainMenuScene::OnDecryptTextButtonPressed()
{
	Application::GetInstance()->PushScene(std::make_shared<DecryptTextScene>(m_fonts));
}

void MainMenuScene::OnAboutMenuButtonPressed()
{
	ENIGMA_INFO(ENIGMA_ABOUT);
	// Show about dialog
	[[maybe_unused]] auto action = 
		DialogUtils::Info("Version: " + String(ENIGMA_VERSION) + '\n' + String(ENIGMA_ABOUT));
}

