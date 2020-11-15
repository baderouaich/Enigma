#include "pch.hpp"
#include "MainMenuScene.hpp"

MainMenuScene::MainMenuScene()
	:
	Enigma::Scene()
{
}


void MainMenuScene::LoadImGuiFont()
{
	m_pieces_of_eight_font = ImGui::GetIO().Fonts->AddFontFromFileTTF(Constants::Fonts::PIECES_OF_EIGHT_FONT_PATH, 45.0f);
	m_enigma_font = ImGui::GetIO().Fonts->AddFontFromFileTTF(Constants::Fonts::ENIGMA_FONT_PATH, 20.0f);
	if (!m_enigma_font || !m_pieces_of_eight_font)
	{
		ENIGMA_ERROR("Could not load fonts");
		Application::GetInstance().EndApplication();
	}
	
}

void MainMenuScene::OnCreate()
{
	LOG(ENIGMA_CURRENT_FUNCTION);

	// Set background clear color
	glClearColor(BACKGROUND_COLOR.x, BACKGROUND_COLOR.y, BACKGROUND_COLOR.z, BACKGROUND_COLOR.w);


	this->LoadImGuiFont();
}

void MainMenuScene::OnUpdate(const f32& dt)
{

}

void MainMenuScene::OnDraw()
{
}

void MainMenuScene::OnImGuiDraw()
{
	const auto& [win_w, win_h] = __super::GetSceneData().window->GetSize();
	const auto& [win_x, win_y] = __super::GetSceneData().window->GetPosition();
	const auto& io = ImGui::GetIO();

	static const auto button_size = ImVec2(350.0f, 45.0f);

	static const auto spacing = [](ui8 n) {
		for (auto i = 0; i < n; i++)
			ImGui::Spacing();
	};

	static const auto container_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground;
	ImGui::Begin("Buttons Container", nullptr, container_flags);
	//ImGui::SetWindowPos(ImVec2(win_x, win_y));
	ImGui::SetWindowSize(ImVec2(win_w, win_h));
	{
		spacing(12);

		// Enigma Version
		{
			ImGui::SetCursorPosX((io.DisplaySize.x - 160.0f) / 2.0f);
			ImGui::PushFont(m_pieces_of_eight_font);
				ImGui::Text("Enigma %s", ENIGMA_VERSION);
			ImGui::PopFont();
		}
		spacing(6);

		// Buttons
		ImGui::PushFont(m_enigma_font);
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
			}
			ImGui::SetCursorPosX((io.DisplaySize.x - button_size.x) / 2.0f);
			if (ImGui::Button("Decrypt Text", button_size))
			{
			}
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

	if (Input::IsKeyPressed(Enigma::Key::Escape))
	{
		__super::EndScene();
	}


	if (Input::IsKeyPressed(Enigma::Key::S))
	{
		const auto& [win_x, win_y] = __super::GetSceneData().window->GetPosition();

		ENIGMA_TRACE("{0} {1}", win_x, win_y);
	}
};

void MainMenuScene::OnDestroy()
{
	LOG(ENIGMA_CURRENT_FUNCTION);

}
