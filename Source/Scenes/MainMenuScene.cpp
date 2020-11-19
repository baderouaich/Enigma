#include "pch.hpp"
#include "Enigma.hpp"
#include "MainMenuScene.hpp"

#include "EncryptTextScene.hpp"
#include "DecryptTextScene.hpp"

MainMenuScene::MainMenuScene()
	:
	Enigma::Scene()
{
	LOG(ENIGMA_CURRENT_FUNCTION);
}


void MainMenuScene::LoadImGuiFonts()
{
	LOG(ENIGMA_CURRENT_FUNCTION);

	m_fonts["Enigma"] = ImGui::GetIO().Fonts->AddFontFromFileTTF(Constants::Fonts::ENIGMA_FONT_PATH, 45.0f);
	m_fonts["Pieces of Eight"] = ImGui::GetIO().Fonts->AddFontFromFileTTF(Constants::Fonts::PIECES_OF_EIGHT_FONT_PATH, 30.0f);
	m_fonts["NunitoSans-Regular"] = ImGui::GetIO().Fonts->AddFontFromFileTTF(Constants::Fonts::NUNITO_SANS_REGULAR_FONT_PATH, 30.0f);
	m_fonts["Goldman-Bold"] = ImGui::GetIO().Fonts->AddFontFromFileTTF(Constants::Fonts::GOLDMAN_BOLD_FONT_PATH, 20.0f);
	m_fonts["Goldman-Regular"] = ImGui::GetIO().Fonts->AddFontFromFileTTF(Constants::Fonts::GOLDMAN_REGULAR_FONT_PATH, 20.0f);
	m_fonts["Ubuntu-Bold"] = ImGui::GetIO().Fonts->AddFontFromFileTTF(Constants::Fonts::UBUNTU_BOLD_FONT_PATH, 20.0f);
	m_fonts["Ubuntu-Bold-Italic"] = ImGui::GetIO().Fonts->AddFontFromFileTTF(Constants::Fonts::UBUNTU_BOLD_ITALIC_FONT_PATH, 20.0f);
	m_fonts["Ubuntu-Italic"] = ImGui::GetIO().Fonts->AddFontFromFileTTF(Constants::Fonts::UBUNTU_ITALIC_FONT_PATH, 20.0f);
	m_fonts["Ubuntu-Light"] = ImGui::GetIO().Fonts->AddFontFromFileTTF(Constants::Fonts::UBUNTU_LIGHT_FONT_PATH, 20.0f);
	m_fonts["Ubuntu-LightItalic"] = ImGui::GetIO().Fonts->AddFontFromFileTTF(Constants::Fonts::UBUNTU_LIGHT_ITALIC_FONT_PATH, 20.0f);
	m_fonts["Ubuntu-Medium"] = ImGui::GetIO().Fonts->AddFontFromFileTTF(Constants::Fonts::UBUNTU_MEDIUM_FONT_PATH, 20.0f);
	m_fonts["Ubuntu-MediumItalic"] = ImGui::GetIO().Fonts->AddFontFromFileTTF(Constants::Fonts::UBUNTU_MEDIUM_ITALIC_FONT_PATH, 20.0f);
	m_fonts["Ubuntu-Regular"] = ImGui::GetIO().Fonts->AddFontFromFileTTF(Constants::Fonts::UBUNTU_REGULAR_FONT_PATH, 20.0f);

	for (const auto& [k, v] : m_fonts)
	{
		if (!v)
		{
			//console alert
			ENIGMA_ERROR("Could not load font {0}", k.c_str());
			//ui alert
			Enigma::MessageBox msg_box("Resource Loading Error",
				"Failed to load font " + k,
				Enigma::MessageBox::Icon::Error,
				Enigma::MessageBox::Choice::Ok);
			auto action = msg_box.Show();
			UNUSED(action);
			//no further without dear fonts :c
			__super::EndScene();
		}
	}

}

void MainMenuScene::OnCreate()
{
	LOG(ENIGMA_CURRENT_FUNCTION);

	// Set background clear color
	glClearColor(
		Constants::Colors::BACKGROUND_COLOR[0],
		Constants::Colors::BACKGROUND_COLOR[1],
		Constants::Colors::BACKGROUND_COLOR[2],
		Constants::Colors::BACKGROUND_COLOR[3]
	);
	// Load dear fonts
	this->LoadImGuiFonts();
}

void MainMenuScene::OnUpdate(const f32& dt)
{}

void MainMenuScene::OnDraw()
{}

void MainMenuScene::OnImGuiDraw()
{
	const auto& [win_w, win_h] = Application::GetInstance().GetWindow()->GetSize();
	const auto& [win_x, win_y] = Application::GetInstance().GetWindow()->GetPosition();
	const auto& io = ImGui::GetIO();

	static const auto button_size = ImVec2(350.0f, 45.0f);

	static constexpr const auto spacing = [](ui8 n) noexcept { for (auto i = 0; i < n; i++) ImGui::Spacing(); };

	static const auto container_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground;
	ImGui::Begin("Buttons Container", nullptr, container_flags);
	ImGui::SetWindowSize(ImVec2(static_cast<f32>(win_w), static_cast<f32>(win_h)));
	ImGui::SetWindowPos(ImVec2(static_cast<f32>(win_x), static_cast<f32>(win_y)));
	{
		spacing(15);

		// Enigma Version
		{
			static const ImVec2 label_size = { ImGui::CalcTextSize("Enigma x.y.z").x * m_fonts["Enigma"]->Scale, ImGui::CalcTextSize("Enigma x.y.z").y * m_fonts["Enigma"]->Scale };
			ImGui::SetCursorPosX((io.DisplaySize.x - label_size.x) / 2.0f);
			ImGui::PushFont(m_fonts["Enigma"]);
				ImGui::Text("Enigma %s", ENIGMA_VERSION);
			ImGui::PopFont();
		}

		spacing(9);

		// Buttons
		ImGui::PushFont(m_fonts["NunitoSans-Regular"]);
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
		}

		//Exit Button
		{
			const static ImVec2 exit_btn_size(70.0f, 45.0f);
			ImGui::SetCursorPos(ImVec2(io.DisplaySize.x - exit_btn_size.x, io.DisplaySize.y - exit_btn_size.y));

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.25f, 0.72f, 0.74f, 0.2f));
			ImGui::PushStyleColor(ImGuiCol_Text,  ImVec4(0.10f, 0.56f, 0.58f, 1.0f));
			if (ImGui::Button("Exit", exit_btn_size))
			{
				__super::EndScene();
			}
			ImGui::PopStyleColor(4);
		}
		ImGui::PopFont();

	}
	ImGui::End();

	//Draw Buttons
#if 0
	{
		static bool open = true;
		static const ImVec2 btn_size = ImVec2(150.0f, 45.0f);
		static const ImVec2 container_size = ImVec2(win_w - 100, btn_size.y * 5.5f);
		static const ImVec4 btn_color_active = VEC_RGBA(39, 124, 156, 255);
		static const ImVec4 btn_color_hover = VEC_RGBA(50, 164, 176, 255);
		static const ImVec4 txt_color{ 1.0f, 1.0f, 1.0f, 1.0f };			
		static constexpr const auto win_flags =
			ImGuiWindowFlags_NoBackground |
			ImGuiWindowFlags_NoDecoration |
			ImGuiWindowFlags_NoDocking |
			ImGuiWindowFlags_NoMove;
			/*	
			ImGuiWindowFlags_NoBackground |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoDocking |
			ImGuiWindowFlags_NoNav |
			ImGuiWindowFlags_NoTitleBar | 
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoCollapse;*/	
		//ImGui::SetNextWindowPos(// Set buttons container in the center
		//	ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f)
		//);
		ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
		ImGui::Begin("Buttons Container", &open, win_flags);
		ImGui::SetWindowSize(container_size);
		//ImGui::SetWindowPos(ImVec2(win_x + btn_size.x, win_y));
		{
			//btn styles
			//ImGui::PushStyleColor(ImGuiCol_Button, btn_color_active);
			//ImGui::PushStyleColor(ImGuiCol_ButtonActive, btn_color_active);
			//ImGui::PushStyleColor(ImGuiCol_ButtonHovered, btn_color_hover);
			//text style
			//ImGui::PushStyleColor(ImGuiCol_Text, txt_color);

			// Enigma Text
			{
				ImGui::SetWindowFontScale(2.0f);
				ImGui::Text("Enigma %s", ENIGMA_VERSION);
				ImGui::SetWindowFontScale(1.0f);
			}
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();
			// Encrypt File Button
			{ 
				if (ImGui::Button("Encrypt File", btn_size))
				{
					std::this_thread::sleep_for(std::chrono::seconds(1));
				}
				if (ImGui::Button("Decrypt File", btn_size))
				{
					std::this_thread::sleep_for(std::chrono::seconds(1));
				}
				ImGui::Spacing();
				if (ImGui::Button("Encrypt Text", btn_size))
				{
					std::this_thread::sleep_for(std::chrono::seconds(1));
				}
				if (ImGui::Button("Decrypt Text", btn_size))
				{
					std::this_thread::sleep_for(std::chrono::seconds(1));
				}

			}
			//ImGui::PopStyleColor(3 + 0);
		}
		ImGui::End();
	}
#endif


	// Clear GL buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}


void MainMenuScene::OnEvent(Event& event)
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
		const auto& [win_x, win_y] = __super::GetSceneData().window->GetPosition();

		ENIGMA_TRACE("{0} {1}", win_x, win_y);
	}
	*/
};

void MainMenuScene::OnDestroy()
{
	LOG(ENIGMA_CURRENT_FUNCTION);
	
	m_fonts.clear();
}
