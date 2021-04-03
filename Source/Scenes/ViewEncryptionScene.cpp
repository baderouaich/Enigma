#include <pch.hpp>
#include "ViewEncryptionScene.hpp"

#include <imgui.h>

NS_ENIGMA_BEGIN

ViewEncryptionScene::ViewEncryptionScene(const i64 encryption_id)
	:
	Enigma::Scene()
{
	ENIGMA_TRACE("Gettings encryption's data id {0} from database", encryption_id);
	
	// Get Encryption record with all cipher and is_file properties which matter in this case
	m_encryption = Database::GetEncryptionByID<true, true, true, true, true>(encryption_id);
	if (!m_encryption)
	{
		(void)DialogUtils::Error("Couldn't get encryption record from database");
		Scene::EndScene();
	}
}

void ViewEncryptionScene::OnCreate()
{
	ENIGMA_TRACE_CURRENT_FUNCTION();

	// Set background clear color
	//glAssert(glClearColor(
	//	Constants::Colors::BACKGROUND_COLOR.x,
	//	Constants::Colors::BACKGROUND_COLOR.y,
	//	Constants::Colors::BACKGROUND_COLOR.z,
	//	Constants::Colors::BACKGROUND_COLOR.w
	//));
}

void ViewEncryptionScene::OnUpdate(const f32&)
{}

void ViewEncryptionScene::OnDraw()
{
	// Clear GL buffers
	//glAssert(glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}

void ViewEncryptionScene::OnImGuiDraw()
{
	const auto& [win_w, win_h] = Application::GetInstance()->GetWindow()->GetSize();
	const auto& [win_x, win_y] = Application::GetInstance()->GetWindow()->GetPosition();
	static const auto& io = ImGui::GetIO();

	const auto button_size = Vec2f(win_w / 2.6f, 40.0f);

	static constexpr const auto inline_dummy = [](const f32& x, const f32& y) noexcept {  ImGui::SameLine(); ImGui::Dummy(ImVec2(x, y)); };
	static constexpr const auto spacing = [](const ui8& n) noexcept { for (ui8 i = 0; i < n; i++) ImGui::Spacing(); };

	const auto& fonts = Application::GetInstance()->GetFonts();
	static ImFont* const& font_audiowide_regular_45 = fonts.at("Audiowide-Regular-45");
	static ImFont* const& font_audiowide_regular_20 = fonts.at("Audiowide-Regular-20");
	static ImFont* const& font_montserrat_medium_20 = fonts.at("Montserrat-Medium-20");
	static ImFont* const& font_montserrat_medium_18 = fonts.at("Montserrat-Medium-18");
	static ImFont* const& font_montserrat_medium_12 = fonts.at("Montserrat-Medium-12");

	static constexpr const auto container_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;// | ImGuiWindowFlags_NoBackground;

	// Push window's background color
	ImGui::PushStyleColor(ImGuiCol_WindowBg, Constants::Colors::BACKGROUND_COLOR);

	ImGui::Begin("Container", nullptr, container_flags);
	ImGui::SetWindowSize(ImVec2(static_cast<f32>(win_w), static_cast<f32>(win_h))); // same size as window
	ImGui::SetWindowPos(ImVec2(0.0f, 0.0f)); // top left
	{
		// Scene Title as encryption title
		ImGui::PushFont(font_audiowide_regular_20); // text font
		ImGui::PushStyleColor(ImGuiCol_Text, Constants::Colors::TEXT_COLOR); // text color
		{
			//static constexpr const auto title = "View Encryption #";
			static const ImVec2 title_size(ImGui::CalcTextSize(m_encryption->title.c_str()).x * font_audiowide_regular_45->Scale, ImGui::CalcTextSize(m_encryption->title.c_str()).y * font_audiowide_regular_45->Scale);
			ImGui::SetCursorPosX((io.DisplaySize.x - title_size.x) / 2.0f);
			ImGui::Text(m_encryption->title.c_str());
		}
		ImGui::PopStyleColor(1);
		ImGui::PopFont();

		spacing(2);
		ImGui::Separator();
		spacing(2);

		if (m_encryption->is_file)
		{
			// Cipher is file binary
			ImGui::Text("File");
		}
		else
		{
			// Cipher is text
			ImGui::Text("Text");

		}

	
		spacing(2);
		ImGui::Separator();
		spacing(2);

		// Back Button 
		{
			ImGui::PushFont(font_audiowide_regular_20); // buttons font
			ImGui::SetCursorPosX((io.DisplaySize.x - button_size.x) / 2.0f);
			ImGui::SetCursorPosY((io.DisplaySize.y - button_size.y) - 10.0f);
			if (ImGuiWidgets::Button("Back", button_size, Constants::Colors::BACK_BUTTON_COLOR, Constants::Colors::BACK_BUTTON_COLOR_HOVER, Constants::Colors::BACK_BUTTON_COLOR_ACTIVE))
			{
				this->OnBackButtonPressed();
			}
			ImGui::PopFont();
		}

	}
	ImGui::End();

	// Pop window's background color
	ImGui::PopStyleVar(1);

}

void ViewEncryptionScene::OnEvent(Event&)
{}

void ViewEncryptionScene::OnDestroy()
{
	ENIGMA_TRACE_CURRENT_FUNCTION();
}


void ViewEncryptionScene::OnBackButtonPressed()
{
	Scene::EndScene();
}


NS_ENIGMA_END
