#include "pch.hpp"
#include "MainMenuScene.hpp"

MainMenuScene::MainMenuScene()
	:
	Enigma::Scene()
{
}

void MainMenuScene::OnCreate()
{
	LOG(ENIGMA_CURRENT_FUNCTION);

	// Set background clear color
	glClearColor(BACKGROUND_COLOR.x, BACKGROUND_COLOR.y, BACKGROUND_COLOR.z, BACKGROUND_COLOR.w);
}

void MainMenuScene::OnUpdate(const f32& dt)
{

}

void MainMenuScene::OnDraw()
{
}

#define VEC_RGBA(r, g, b, a) ImVec4(r/255.0f, g/255.0f, b/255.0f, a/255.0f)
void MainMenuScene::OnImGuiDraw()
{
	const auto& [win_w, win_h] = __super::GetSceneData().window->GetSize();
	const auto& io = ImGui::GetIO();

	//Draw Buttons
	{
		static bool open = true;
		static const ImVec2 btn_size = ImVec2(300.0f, 70.0f);
		static const ImVec2 win_size = ImVec2(btn_size.x * 2.0f, btn_size.y * 2.5f);
		static const ImVec4 btn_color_active = VEC_RGBA(39, 124, 156, 255);
		static const ImVec4 btn_color_hover = VEC_RGBA(50, 164, 176, 255);
		static const ImVec4 txt_color{ 1.0f, 1.0f, 1.0f, 1.0f };
		/*static constexpr const auto win_flags =
			ImGuiWindowFlags_NoBackground |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoDocking |
			ImGuiWindowFlags_NoNav |
			ImGuiWindowFlags_NoTitleBar | 
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoCollapse;*/		
		static constexpr const auto win_flags =
			ImGuiWindowFlags_NoBackground |
			ImGuiWindowFlags_NoDecoration |
			ImGuiWindowFlags_NoDocking |
			ImGuiWindowFlags_NoMove;
		ImGui::SetNextWindowPos(// Set buttons container in the center
			ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f)
		);
		ImGui::Begin("Buttons Container", &open, win_flags);
		ImGui::SetWindowSize(win_size);
		{
			//btn styles
			//ImGui::PushStyleColor(ImGuiCol_Button, btn_color_active);
			//ImGui::PushStyleColor(ImGuiCol_ButtonActive, btn_color_active);
			//ImGui::PushStyleColor(ImGuiCol_ButtonHovered, btn_color_hover);
			//text style
			//ImGui::PushStyleColor(ImGuiCol_Text, txt_color);

			// Encrypt File Button
			{ 
				if (ImGui::Button("Encrypt File", btn_size))
				{
					std::this_thread::sleep_for(std::chrono::seconds(1));
				}
				ImGui::SameLine();
				if (ImGui::Button("Decrypt File", btn_size))
				{
					std::this_thread::sleep_for(std::chrono::seconds(1));
				}
				ImGui::Spacing();
				ImGui::NewLine();
				if (ImGui::Button("Encrypt Text", btn_size))
				{
					std::this_thread::sleep_for(std::chrono::seconds(1));
				}
				ImGui::SameLine();
				if (ImGui::Button("Decrypt Text", btn_size))
				{
					std::this_thread::sleep_for(std::chrono::seconds(1));
				}

			}
			//ImGui::PopStyleColor(3 + 0);
		}
		ImGui::End();
	}




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

};

void MainMenuScene::OnDestroy()
{
	LOG(ENIGMA_CURRENT_FUNCTION);
}
