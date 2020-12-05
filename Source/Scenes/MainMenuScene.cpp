#include "pch.hpp"
#include "Enigma.hpp"
#include "MainMenuScene.hpp"

#include "EncryptTextScene.hpp"
#include "DecryptTextScene.hpp"


MainMenuScene::MainMenuScene()  noexcept
	:
	Enigma::Scene()
{
	LOG(ENIGMA_CURRENT_FUNCTION);
}

void MainMenuScene::OnCreate()
{
	LOG(ENIGMA_CURRENT_FUNCTION);

	// Set background clear color
	glAssert(glClearColor(
		Constants::Colors::BACKGROUND_COLOR.x,
		Constants::Colors::BACKGROUND_COLOR.y,
		Constants::Colors::BACKGROUND_COLOR.z,
		Constants::Colors::BACKGROUND_COLOR.w
	));

	// Load dear fonts
	this->LoadImGuiFonts();
}

void MainMenuScene::OnUpdate(const f32& dt)
{}

void MainMenuScene::OnDraw()
{
	// Clear GL buffers
	glAssert(glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}

void MainMenuScene::OnImGuiDraw()
{
	const auto& [win_w, win_h] = Enigma::Application::GetInstance().GetWindow()->GetSize();
	const auto& [win_x, win_y] = Enigma::Application::GetInstance().GetWindow()->GetPosition();
	static const auto& io = ImGui::GetIO();

	const auto button_size = Vec2f(win_w / 2.5f, 40.0f); // 5 buttons

	static constexpr const auto spacing = [](ui8 n) noexcept { for (ui8 i = 0; i < n; i++) ImGui::Spacing(); };

	static ImFont* const& font_audiowide_regular_45 = m_fonts.at("Audiowide-Regular-45");
	static ImFont* const& font_audiowide_regular_20 = m_fonts.at("Audiowide-Regular-20");

	static constexpr const auto container_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_MenuBar;
	ImGui::Begin("Container", nullptr, container_flags);
	ImGui::SetWindowSize(ImVec2(static_cast<f32>(win_w), static_cast<f32>(win_h))); // same size as window
	ImGui::SetWindowPos(ImVec2(0.0f, 0.0f)); // top left
	{
		// Menu bar
		{
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_MenuBarBg, Constants::Colors::BUTTON_COLOR); // Menu bar background color
			ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_FramePadding, ImVec2(0.0f, 16.0f)); // Menu bar padding
			ImGui::PushFont(font_audiowide_regular_20);
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Menu"))
				{
					if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
					if (ImGui::MenuItem("About")) { this->ShowAboutDialog(); }
					if (ImGui::MenuItem("Exit")) { this->EndScene(); }
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}
			ImGui::PopFont();
			ImGui::PopStyleVar(1);
			ImGui::PopStyleColor(1);
		}

		spacing(6);

		// Enigma Version
		{
			ImGui::PushFont(font_audiowide_regular_45); // text font
			ImGui::PushStyleColor(ImGuiCol_Text, Constants::Colors::TEXT_COLOR); // text color
			{
				static const ImVec2 label_size = { ImGui::CalcTextSize("Enigma x.y.z").x * font_audiowide_regular_45->Scale, ImGui::CalcTextSize("Enigma x.y.z").y * font_audiowide_regular_45->Scale };
				ImGui::SetCursorPosX((io.DisplaySize.x - label_size.x) / 2.0f);
				ImGui::Text("Enigma %s", ENIGMA_VERSION);
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
					// Push new EncryptFileScene to perform operation 

				}
				ImGui::SetCursorPosX((io.DisplaySize.x - button_size.x) / 2.0f);
				if (ImGui::Button("Decrypt File", button_size))
				{

				}
				spacing(3);
				ImGui::SetCursorPosX((io.DisplaySize.x - button_size.x) / 2.0f);
				if (ImGui::Button("Encrypt Text", button_size))
				{
					Application::GetInstance().PushScene(new EncryptTextScene(m_fonts));
				}
				ImGui::SetCursorPosX((io.DisplaySize.x - button_size.x) / 2.0f);
				if (ImGui::Button("Decrypt Text", button_size))
				{
					Application::GetInstance().PushScene(new DecryptTextScene(m_fonts));

				}
				spacing(6);
				{
					ImGui::SetCursorPosX((io.DisplaySize.x - button_size.x) / 2.0f);
					if (ImGui::Button("Exit", button_size))
					{
						EndScene();
					}
				}
			}
			ImGui::PopStyleColor(3);
			ImGui::PopFont();
		}

	}
	ImGui::End();


}
#if 0
{
	const auto& [win_w, win_h] = Application::GetInstance().GetWindow()->GetSize();
	const auto& [win_x, win_y] = Application::GetInstance().GetWindow()->GetPosition();
	static const auto& io = ImGui::GetIO();

	const auto button_size = Vec2f(win_w / 2.5f, 40.0f); // 5 buttons

	static constexpr const auto spacing = [](ui8 n) noexcept { for (ui8 i = 0; i < n; i++) ImGui::Spacing(); };

	static ImFont* const& font_audiowide_regular = m_fonts.at("Audiowide-Regular-45");
	static ImFont* const& font_audiowide_regular_20 = m_fonts.at("Audiowide-Regular-20");
	
	static constexpr const auto container_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_MenuBar;
	ImGui::Begin("Container", nullptr, container_flags);
	ImGui::SetWindowSize(ImVec2(static_cast<f32>(win_w), static_cast<f32>(win_h)));
	ImGui::SetWindowPos(ImVec2(static_cast<f32>(win_x), static_cast<f32>(win_y)));
	{
		// Menu bar
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
				if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
				if (ImGui::MenuItem("Close", "Ctrl+W")) { this->EndScene(); }
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		spacing(12);

		// Enigma Version

		ImGui::PushFont(font_audiowide_regular); // text font
		ImGui::PushStyleColor(ImGuiCol_Text, Constants::Colors::TEXT_COLOR); // text color
		{
			static const ImVec2 label_size = { ImGui::CalcTextSize("Enigma x.y.z").x * font_audiowide_regular->Scale, ImGui::CalcTextSize("Enigma x.y.z").y * font_audiowide_regular->Scale };
			ImGui::SetCursorPosX((io.DisplaySize.x - label_size.x) / 2.0f);
			ImGui::Text("Enigma %s", ENIGMA_VERSION);
		}
		ImGui::PopStyleColor(1);
		ImGui::PopFont();

		spacing(9);

		// Buttons
		ImGui::PushFont(font_audiowide_regular_20); // buttons font
		ImGui::PushStyleColor(ImGuiCol_Button, Constants::Colors::BUTTON_COLOR); // buttons color idle
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Constants::Colors::BUTTON_COLOR_HOVER);  // buttons color hover
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, Constants::Colors::BUTTON_COLOR_ACTIVE); // buttons color pressed
		{
			ImGui::SetCursorPosX((io.DisplaySize.x - button_size.x) / 2.0f);
			if (ImGui::Button("Encrypt File", button_size))
			{
				// Push new EncryptFileScene to perform operation 

			}
			ImGui::SetCursorPosX((io.DisplaySize.x - button_size.x) / 2.0f);
			if (ImGui::Button("Decrypt File", button_size))
			{

			}
			spacing(3);
			ImGui::SetCursorPosX((io.DisplaySize.x - button_size.x) / 2.0f);
			if (ImGui::Button("Encrypt Text", button_size))
			{
				Application::GetInstance().PushScene(new EncryptTextScene(m_fonts));
			}
			ImGui::SetCursorPosX((io.DisplaySize.x - button_size.x) / 2.0f);
			if (ImGui::Button("Decrypt Text", button_size))
			{
				Application::GetInstance().PushScene(new DecryptTextScene(m_fonts));

			}
			spacing(6);
			{
				ImGui::SetCursorPosX((io.DisplaySize.x - button_size.x) / 2.0f);
				if (ImGui::Button("Exit", button_size))
				{
					EndScene();
				}
			}
		}
		ImGui::PopStyleColor(3);
		ImGui::PopFont();

		
		/*{
		* bottom left exit btn
			const static ImVec2 exit_btn_size(70.0f, 45.0f);
			ImGui::SetCursorPos(ImVec2(io.DisplaySize.x - exit_btn_size.x, io.DisplaySize.y - exit_btn_size.y));

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.25f, 0.72f, 0.74f, 0.2f));
			ImGui::PushStyleColor(ImGuiCol_Text,  ImVec4(0.10f, 0.56f, 0.58f, 1.0f));

			if (ImGui::Button("Exit", exit_btn_size))
			{
				EndScene();
			}
			ImGui::PopStyleColor(4);
		}*/

	}
	ImGui::End();


}
#endif

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
	LOG(ENIGMA_CURRENT_FUNCTION);
	
	m_fonts.clear();
}



void MainMenuScene::LoadImGuiFonts()
{
	LOG(ENIGMA_CURRENT_FUNCTION);

	static const auto& io = ImGui::GetIO();

	m_fonts["Audiowide-Regular-45"] = io.Fonts->AddFontFromFileTTF(Constants::Resources::Fonts::AUDIOWIDE_FONT_PATH, 45.0f);
	m_fonts["Audiowide-Regular-20"] = io.Fonts->AddFontFromFileTTF(Constants::Resources::Fonts::AUDIOWIDE_FONT_PATH, 20.0f);

	// Check if fonts are loaded
	for (const auto& [font_name, font] : m_fonts)
	{
		if (!font)
		{
			//console alert
			ENIGMA_ERROR("Could not load font {0}", font_name.c_str());

			//ui alert
			Enigma::UniquePtr<Enigma::MessageBox> msg_box = Enigma::MakeUnique<Enigma::MessageBox>("Resource Loading Error",
				"Failed to load font " + font_name,
				Enigma::MessageBox::Icon::Error,
				Enigma::MessageBox::Choice::Ok);
			[[maybe_unused]] auto action = msg_box->Show();

			//no further without dear fonts :c
			EndScene();
		}
	}

}

void MainMenuScene::ShowAboutDialog()
{
	ENIGMA_INFO(ENIGMA_ABOUT);

	Enigma::UniquePtr<Enigma::MessageBox> about_dialog = Enigma::MakeUnique<Enigma::MessageBox>(
		"About",
		Enigma::String(ENIGMA_ABOUT),
		Enigma::MessageBox::Icon::Info,
		Enigma::MessageBox::Choice::Ok);
	[[maybe_unused]] auto action = about_dialog->Show();
}
