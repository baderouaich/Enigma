#include "pch.hpp"
#include "RSAScene.hpp"
#include <Utility/DialogUtils.hpp>
#include <GUI/ImGuiWidgets.hpp>
#include <Utility/GZip.hpp>
#include <System/Clipboard/Clipboard.hpp>

NS_ENIGMA_BEGIN

RSAScene::RSAScene()
	:
	Enigma::Scene()
{
}

void RSAScene::OnCreate()
{
	ENIGMA_TRACE_CURRENT_FUNCTION();

	// Explicit OpenGL old method to et background clear color
	//glAssert(glClearColor(
	//	Constants::Colors::BACKGROUND_COLOR.x,
	//	Constants::Colors::BACKGROUND_COLOR.y,
	//	Constants::Colors::BACKGROUND_COLOR.z,
	//	Constants::Colors::BACKGROUND_COLOR.w
	//));

}

void RSAScene::OnUpdate(const f32& /*dt*/)
{}

void RSAScene::OnDraw()
{
	// Clear GL buffers
	//glAssert(glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}

void RSAScene::OnImGuiDraw()
{
	const auto& [win_w, win_h] = Application::GetInstance()->GetWindow()->GetSize();
	const auto& [win_x, win_y] = Application::GetInstance()->GetWindow()->GetPosition();
	static const auto& io = ImGui::GetIO();

	const auto button_size = Vec2f(win_w / 2.5f, 40.0f);

	static constexpr const auto dummy = [](const f32& x, const f32& y) noexcept { ImGui::Dummy(ImVec2(x, y)); };
	static constexpr const auto inline_dummy = [](const f32& x, const f32& y) noexcept {  ImGui::SameLine(); ImGui::Dummy(ImVec2(x, y)); };
	static constexpr const auto spacing = [](const ui8& n) noexcept { for (ui8 i = 0; i < n; i++) ImGui::Spacing(); };
	static constexpr const auto inline_spacing = [](const ui8& n) noexcept { for (ui8 i = 0; i < n; i++) { ImGui::SameLine(); ImGui::Spacing(); } };

	static const auto& fonts = Application::GetInstance()->GetFonts();
	static ImFont* const& font_audiowide_regular_45 = fonts.at("Audiowide-Regular-45");
	static ImFont* const& font_audiowide_regular_30 = fonts.at("Audiowide-Regular-30");
	static ImFont* const& font_audiowide_regular_20 = fonts.at("Audiowide-Regular-20");
	static ImFont* const& font_montserrat_medium_20 = fonts.at("Montserrat-Medium-20");
	static ImFont* const& font_montserrat_medium_18 = fonts.at("Montserrat-Medium-18");
	static ImFont* const& font_montserrat_medium_16 = fonts.at("Montserrat-Medium-16");
	static ImFont* const& font_montserrat_medium_14 = fonts.at("Montserrat-Medium-14");
	static ImFont* const& font_montserrat_medium_12 = fonts.at("Montserrat-Medium-12");

	static constexpr const auto container_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;// | ImGuiWindowFlags_NoBackground;

	// Push window's background color
	ImGui::PushStyleColor(ImGuiCol_WindowBg, Constants::Colors::BACKGROUND_COLOR);

	ImGui::Begin("Container", nullptr, container_flags);
	ImGui::SetWindowSize(ImVec2(static_cast<f32>(win_w), static_cast<f32>(win_h))); // same size as window
	ImGui::SetWindowPos(ImVec2(0.0f, 0.0f)); // top left
	{
#pragma region Back Button [<] & Scene title
		// Back button [<] & Title
		{
			static const auto& title_font = font_audiowide_regular_30;
			static const auto title = "RSA";
			static const ImVec2 title_size((ImGui::CalcTextSize(title).x * title_font->Scale) - 45.0f, ImGui::CalcTextSize(title).y * title_font->Scale);
			static const ImVec2 back_button_size(45.0f, title_size.y);

			// Back Button [<]
			{
				ImGui::PushFont(font_montserrat_medium_18); // < arrow is a text too
				if (ImGuiWidgets::BackButton("##back", back_button_size))
				{
					Scene::EndScene();
				}
				ImGui::PopFont();
			}

			ImGui::SameLine();

			// Scene Title
			{
				ImGui::PushFont(title_font); // text font
				ImGui::PushStyleColor(ImGuiCol_Text, Constants::Colors::TEXT_COLOR); // text color
				ImGui::PushStyleColor(ImGuiCol_Button, Constants::Colors::SCENE_TITLE_BACKGROUND_COLOR); // Scene title back color
				{
					(void)ImGui::ButtonEx(title, ImVec2(static_cast<f32>(win_w), title_size.y), ImGuiButtonFlags_Disabled);
				}
				ImGui::PopStyleColor(2);
				ImGui::PopFont();
			}
		}
#pragma endregion

		spacing(3);
		ImGui::Separator();
		spacing(3);

#if 0
		// Save to database widget
		ImGui::PushFont(font_montserrat_medium_16);
		{
			ImGui::PushFont(font_audiowide_regular_20);
			ImGui::Text("%s:", ENIGMA_TRANSLATE_CSTR("Save to database"));
			ImGui::PopFont();
			inline_dummy(6.0f, 0.0f);
			ImGui::SameLine();
			ImGui::Checkbox("##checkbox", &m_save_to_database);

			if (m_save_to_database)
			{
				ImGui::Text("%s:", ENIGMA_TRANSLATE_CSTR("Encryption Title"));
				ImGuiWidgets::InputTextWithHint("##idb", ENIGMA_TRANSLATE_CSTR("(example > An important message) helps with searching through encryption records in the future"), &m_db_title, win_w / 1.3f);
			}
		}
		ImGui::PopFont();

		spacing(2);
		ImGui::Separator();
		spacing(2);

		// Text to encrypt
		ImGui::PushFont(font_montserrat_medium_20);
		{
			// Label
			ImGui::Text("%s:", ENIGMA_TRANSLATE_CSTR("Text"));

			// Input text
			const ImVec2 input_text_size(static_cast<f32>(win_w), ImGui::GetTextLineHeightWithSpacing() * 5);
			ImGuiWidgets::InputTextMultiline("##text1", &m_text, input_text_size);

			// Bytes count
			ImGui::PushFont(font_montserrat_medium_12);
			ImGui::Text("%s", SizeUtils::FriendlySize(m_text.size()).c_str());
			ImGui::PopFont();
		}
		ImGui::PopFont();

		spacing(3);
		ImGui::Separator();
		spacing(3);

		// Encrypt Button
		{
			ImGui::PushFont(font_audiowide_regular_20); // buttons font
			ImGui::PushStyleColor(ImGuiCol_Button, Constants::Colors::BUTTON_COLOR); // buttons color idle
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Constants::Colors::BUTTON_COLOR_HOVER);  // buttons color hover
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, Constants::Colors::BUTTON_COLOR_ACTIVE); // buttons color pressed
			{
				ImGui::SetCursorPosX((io.DisplaySize.x - button_size.x) / 2.0f);
				//ImGui::SetCursorPosY((io.DisplaySize.y - button_size.y) - 10.0f);
				if (ImGui::Button(ENIGMA_TRANSLATE_CSTR("Encrypt"), button_size))
				{
					Application::GetInstance()->LaunchWorkerThread(this, ENIGMA_TRANSLATE_CSTR("Encrypting text..."), [this]() -> void
						{
							this->OnEncryptButtonPressed();
						});
				}

			}
			ImGui::PopStyleColor(3);
			ImGui::PopFont();

		}

#endif

	}
	ImGui::End();

	// Pop window's background color
	ImGui::PopStyleColor(1);

}

void RSAScene::OnEvent(Event&)
{
	//// Go back to main menu scene
	//if (Input::IsKeyPressed(Enigma::Key::Escape))
	//{
	//	// Confirm encryption abortion
	//	if (this->UserWantsToCancel())
	//	{
	//		this->EndScene();
	//	}
	//}
};

void RSAScene::OnDestroy()
{
	ENIGMA_TRACE_CURRENT_FUNCTION();

	
}

NS_ENIGMA_END
