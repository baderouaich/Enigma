#include "pch.hpp"
#include "EncryptTextScene.hpp"

EncryptTextScene::EncryptTextScene(const std::unordered_map<String, ImFont*>& fonts)
	:
	Enigma::Scene(),
	m_fonts(fonts)
{
}

void EncryptTextScene::OnCreate()
{
	LOG(ENIGMA_CURRENT_FUNCTION);

	// Set background clear color
	glAssert(glClearColor(
		Constants::Colors::BACKGROUND_COLOR.x,
		Constants::Colors::BACKGROUND_COLOR.y,
		Constants::Colors::BACKGROUND_COLOR.z,
		Constants::Colors::BACKGROUND_COLOR.w
	));
}

void EncryptTextScene::OnUpdate(const f32& dt)
{}

void EncryptTextScene::OnDraw()
{
	// Clear GL buffers
	glAssert(glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}

void EncryptTextScene::OnImGuiDraw()
{
	const auto& [win_w, win_h] = Application::GetInstance().GetWindow()->GetSize();
	const auto& [win_x, win_y] = Application::GetInstance().GetWindow()->GetPosition();
	static const auto& io = ImGui::GetIO();

	static const auto button_size = ImVec2(350.0f, 45.0f);
	static const auto spacing = [](ui8 n) { for (auto i = 0; i < n; i++) ImGui::Spacing(); };
	static const auto container_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground;

	static ImFont* const& font_audiowide_regular = m_fonts.at("Audiowide-Regular-45");
	static ImFont* const& font_audiowide_regular_20 = m_fonts.at("Audiowide-Regular-20");

	ImGui::Begin("ETS Container", nullptr, container_flags);
	ImGui::SetWindowSize(ImVec2(static_cast<f32>(win_w), static_cast<f32>(win_h)));
	ImGui::SetWindowPos(ImVec2(static_cast<f32>(win_x), static_cast<f32>(win_y)));
	{
		// Text to encrypt input text
		ImGui::PushFont(font_audiowide_regular);
		{
			static char buffer[1024 * 1024 * 1]{ '\000' }; // 1mb max buffer
			// Label
			ImGui::Text("Text to Encrypt: %d / %d (1mb max)", strlen(buffer), IM_ARRAYSIZE(buffer)); spacing(1);
			// Input text
			ImGui::PushItemWidth(win_w - 80.0f);
			ImGui::InputText("##text", buffer, IM_ARRAYSIZE(buffer));
			ImGui::SameLine(); 
			// Clear button
			if (ImGui::Button("Clear"))
			{
				std::memset(buffer, 0, IM_ARRAYSIZE(buffer));
			}
			ImGui::NewLine();

			spacing(3);

			//Choose encryption algorithm type


		}
		ImGui::PopFont();





		// Back To Main Menu Scene Button
		ImGui::PushFont(font_audiowide_regular_20);
		{
			const static ImVec2 back_btn_size = { ImGui::CalcTextSize("Back to Menu").x + 10.0f, 45.0f };
			ImGui::SetCursorPos(ImVec2(io.DisplaySize.x - back_btn_size.x, io.DisplaySize.y - back_btn_size.y));

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.25f, 0.72f, 0.74f, 0.2f));
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.10f, 0.56f, 0.58f, 1.0f));
			if (ImGui::Button("Back to Menu", back_btn_size))
			{
				__super::EndScene();
			}
			ImGui::PopStyleColor(4);
		}
		ImGui::PopFont();


		

	}
	ImGui::End();
}


void EncryptTextScene::OnEvent(Event& event)
{
	// Go back to main menu scene
	if (Input::IsKeyPressed(Enigma::Key::Escape))
	{
		__super::EndScene();
	}
};

void EncryptTextScene::OnDestroy()
{
	LOG(ENIGMA_CURRENT_FUNCTION);
}
