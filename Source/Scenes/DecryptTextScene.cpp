#include "pch.hpp"
#include "DecryptTextScene.hpp"

DecryptTextScene::DecryptTextScene(const std::unordered_map<String, ImFont*>& fonts)
	:
	Enigma::Scene(),
	m_fonts(fonts)
{
}

void DecryptTextScene::OnCreate()
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

void DecryptTextScene::OnUpdate(const f32& dt)
{}

void DecryptTextScene::OnDraw()
{
	// Clear GL buffers
	glAssert(glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}

void DecryptTextScene::OnImGuiDraw()
{
	const auto& [win_w, win_h] = Application::GetInstance().GetWindow()->GetSize();
	const auto& [win_x, win_y] = Application::GetInstance().GetWindow()->GetPosition();
	const auto& io = ImGui::GetIO();

	static const auto button_size = ImVec2(win_w / 3.0f, 45.0f);

	static const auto spacing = [](ui8 n) { for (auto i = 0; i < n; i++) ImGui::Spacing(); };

	static const auto container_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground;
	ImGui::Begin("Buttons Container", nullptr, container_flags);
	//ImGui::SetWindowPos(ImVec2(win_x, win_y));
	ImGui::SetWindowSize(ImVec2(static_cast<f32>(win_w), static_cast<f32>(win_h)));
	{
		ImGui::PushFont(io.FontDefault);
		{
			spacing(15);
			{
				ImGui::Text("Enigma Decrypt Text Scene");
			}
			if (ImGui::Button("Close Scene"))
			{
				__super::EndScene();
			}
			spacing(9);

		}
		ImGui::PopFont();		
	}
	ImGui::End();

}


void DecryptTextScene::OnEvent(Event& event)
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
		const auto& [win_x, win_y] = Application::GetInstance().GetWindow()->GetPosition();

		ENIGMA_TRACE("{0} {1}", win_x, win_y);
	}
};

void DecryptTextScene::OnDestroy()
{
	LOG(ENIGMA_CURRENT_FUNCTION);
}
