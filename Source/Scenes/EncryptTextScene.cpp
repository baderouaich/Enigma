#include "pch.hpp"
#include "EncryptTextScene.hpp"
#include <Utility/DialogUtils.hpp>
#include <Utility/ImGuiUtils.hpp>
#include <System/Clipboard/Clipboard.hpp>

EncryptTextScene::EncryptTextScene(const std::unordered_map<std::string_view, ImFont*>& fonts)
	:
	Enigma::Scene(),
	m_fonts(fonts),
	// AES as default algorithm
	m_algorithm(Algorithm::CreateFromType(Algorithm::Type::AES, Algorithm::Intent::Encrypt))
{

}

void EncryptTextScene::OnCreate()
{
	ENIGMA_TRACE(ENIGMA_CURRENT_FUNCTION);

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
	const auto& [win_w, win_h] = Application::GetInstance()->GetWindow()->GetSize();
	const auto& [win_x, win_y] = Application::GetInstance()->GetWindow()->GetPosition();
	static const auto& io = ImGui::GetIO();

	const auto button_size = Vec2f(win_w / 2.5f, 40.0f);

	static constexpr const auto dummy = [](const f32& x, const f32& y) noexcept { ImGui::Dummy(ImVec2(x, y)); };
	static constexpr const auto inline_dummy = [](const f32& x, const f32& y) noexcept {  ImGui::SameLine(); ImGui::Dummy(ImVec2(x, y)); };
	static constexpr const auto spacing = [](const ui8& n) noexcept { for (ui8 i = 0; i < n; i++) ImGui::Spacing(); };
	static constexpr const auto inline_spacing = [](const ui8& n) noexcept { for (ui8 i = 0; i < n; i++) { ImGui::SameLine(); ImGui::Spacing(); } };

	static ImFont* const& font_audiowide_regular_45 = m_fonts.at("Audiowide-Regular-45");
	static ImFont* const& font_audiowide_regular_20 = m_fonts.at("Audiowide-Regular-20");
	static ImFont* const& font_montserrat_medium_20 = m_fonts.at("Montserrat-Medium-20");
	static ImFont* const& font_montserrat_medium_12 = m_fonts.at("Montserrat-Medium-12");

	static constexpr const auto container_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground;
	//static constexpr const auto container_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground;
	ImGui::Begin("Container", nullptr, container_flags);
	ImGui::SetWindowSize(ImVec2(static_cast<f32>(win_w), static_cast<f32>(win_h))); // same size as window
	ImGui::SetWindowPos(ImVec2(0.0f, 0.0f)); // top left
	{
		// Scene Title
		ImGui::PushFont(font_audiowide_regular_45); // text font
		ImGui::PushStyleColor(ImGuiCol_Text, Constants::Colors::TEXT_COLOR); // text color
		{
			static constexpr const auto title = "Encrypt Text";
			static const ImVec2 title_size(ImGui::CalcTextSize(title).x * font_audiowide_regular_45->Scale,ImGui::CalcTextSize(title).y * font_audiowide_regular_45->Scale);
			ImGui::SetCursorPosX((io.DisplaySize.x - title_size.x) / 2.0f);
			ImGui::Text(title);
		}
		ImGui::PopStyleColor(1);
		ImGui::PopFont();
		
		spacing(3);

		// Algorithm To encrypt text with
		ImGui::PushFont(font_audiowide_regular_20);
		{
			// Label
			ImGui::Text("Algorithm:");

			// Algo types radio buttons
			const static auto supported_algorithms = Algorithm::GetSupportedAlgorithms();
			for (const auto& [algo_name, algo_type] : supported_algorithms)
			{
				inline_dummy(6.0f, 0.0f);
				ImGui::SameLine();
				if (ImGui::RadioButton(algo_name.c_str(), m_algorithm->GetType() == algo_type))
				{
					m_algorithm->SetType(algo_type);
				}
			}
			
			/*
			Hardcoded Arghh!
			if (ImGui::RadioButton("AES", m_algorithm == Algorithm::Type::AES))
			{
				m_algorithm = Algorithm::Type::AES;
			}
			inline_dummy(2.0f, 0.0f);
			ImGui::SameLine();
			if (ImGui::RadioButton("ChaCha20", m_algorithm == Algorithm::Type::ChaCha20))
			{
				m_algorithm = Algorithm::Type::ChaCha20;
			}
			inline_dummy(2.0f, 0.0f);
			ImGui::SameLine();
			if (ImGui::RadioButton("TripleDES", m_algorithm == Algorithm::Type::TripleDES))
			{
				m_algorithm = Algorithm::Type::TripleDES;
			}			
			inline_dummy(2.0f, 0.0f);
			ImGui::SameLine();
			if (ImGui::RadioButton("Twofish", m_algorithm == Algorithm::Type::Twofish))
			{
				m_algorithm = Algorithm::Type::Twofish;
			}*/
		}
		ImGui::PopFont();

		spacing(3);

		// Text to encrypt
		ImGui::PushFont(font_montserrat_medium_20);
		{
			// Label
			ImGui::Text("Text to Encrypt:");

			// Input text
			const ImVec2 input_text_size(static_cast<f32>(win_w) - 10.0f, 120.0f);
			ImGuiUtils::InputTextMultiline("##text1", &m_text, input_text_size);

			// Remaining characters
			ImGui::PushFont(font_montserrat_medium_12);
			ImGui::Text("%llu bytes", m_text.size());
			ImGui::PopFont();
		}
		ImGui::PopFont();

		spacing(3);

		// Encryption Password & Confirm password
		// password text coloring for each state
		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, (m_password.empty() && m_confirm_password.empty()) ? ImVec4(1.0f, 1.0f, 1.0f, 1.0f) : m_password == m_confirm_password ? ImVec4(0.2f, 1.0f, 0.0f, 0.6f) : ImVec4(1.0f, 0.2f, 0.0f, 0.6f));
		ImGui::PushFont(font_montserrat_medium_20);
		{
			// Label
			ImGui::Text("Password:");
			// Input text
			const ImVec2 input_text_size(static_cast<f32>(win_w) - 10.0f, 33.0f);
			ImGuiUtils::InputTextMultiline("##text2", &m_password, input_text_size, ImGuiInputTextFlags_::ImGuiInputTextFlags_Password);
			ImGui::Text("Confirm Password:");
			ImGuiUtils::InputTextMultiline("##text3", &m_confirm_password, input_text_size, ImGuiInputTextFlags_::ImGuiInputTextFlags_Password);
			ImGui::PopStyleColor();
			// Remaining characters
			ImGui::PushFont(font_montserrat_medium_12);
			ImGui::Text("%llu bytes", m_password.size());
		}
		ImGui::PopFont();
		ImGui::PopFont();

		spacing(3);

		// Encrypted text in Base64 output
		if (!m_encrypted_text.empty())
		{
			ImGui::PushFont(font_montserrat_medium_20);
			{
				// Label
				ImGui::Text("Encrypted Text (encoded in base64)");
				// Encrypted text
				const ImVec2 copy_button_size(45.0f, 25.0f);
				const ImVec2 input_text_size(static_cast<f32>(win_w) - 20.0f - copy_button_size.x, 33.0f);
				ImGuiUtils::InputTextMultiline("##text4", &m_encrypted_text_base64, input_text_size);
				ImGui::PushFont(font_montserrat_medium_12);
					ImGui::SameLine();
					if (ImGui::Button("Copy", copy_button_size))
					{
						this->OnCopyEncryptedBase64TextButtonPressed();
					}
					//ImGui::NewLine();
					ImGui::Text("%llu bytes", m_encrypted_text_base64.size());
				ImGui::PopFont();
			}
			ImGui::PopFont();
		}



		spacing(9);
		
		// Encrypt Button
		{
			ImGui::PushFont(font_audiowide_regular_20); // buttons font
			ImGui::PushStyleColor(ImGuiCol_Button, Constants::Colors::BUTTON_COLOR); // buttons color idle
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Constants::Colors::BUTTON_COLOR_HOVER);  // buttons color hover
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, Constants::Colors::BUTTON_COLOR_ACTIVE); // buttons color pressed
			{
				ImGui::SetCursorPosX((io.DisplaySize.x - button_size.x * 2) / 2.0f);
				if (ImGui::Button("Back", button_size))
				{
					this->OnBackButtonPressed();
				}
				ImGui::SameLine();
				if (ImGui::Button("Encrypt", button_size))
				{
					this->OnEncryptButtonPressed();
				}

			}
			ImGui::PopStyleColor(3);
			ImGui::PopFont();
		}
		

	}
	ImGui::End();
}

void EncryptTextScene::OnEvent(Event& event)
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

void EncryptTextScene::OnDestroy()
{
	ENIGMA_TRACE(ENIGMA_CURRENT_FUNCTION);

	m_text.clear();
	m_password.clear();
	m_encrypted_text.clear();
	m_encrypted_text_base64.clear();
}

void EncryptTextScene::OnEncryptButtonPressed()
{
	// Validate fields 
	if (m_text.empty())
	{
		(void)DialogUtils::Warn("Text to encrypt is empty");
	}
	else if (m_password.empty() || m_confirm_password.empty())
	{
		(void)DialogUtils::Warn("Encryption Password is empty");
	}
	else if (m_password.size() < Constants::Algorithm::MINIMUM_PASSWORD_LENGTH)
	{
		(void)DialogUtils::Warn(Constants::ErrorMessages::WEAK_PASSWORD_ERROR_MESSAGE);
	}
	else if (m_password != m_confirm_password)
	{
		(void)DialogUtils::Warn("Password doesn't match");
	}
	else // Alles gut
	{ 
		try
		{
			// Create encryptor based on selected algorithm type
			m_algorithm = Algorithm::CreateFromType(m_algorithm->GetType(), Algorithm::Intent::Encrypt);
			ENIGMA_ASSERT_OR_THROW(m_algorithm, "Failed to create algorithm from type");

			// Encrypt text
			m_encrypted_text = m_algorithm->Encrypt(m_password, m_text);
			ENIGMA_ASSERT_OR_THROW(!m_encrypted_text.empty(), "Failed to encrypt text");

			// Encode cipher to Base64
			m_encrypted_text_base64 = Base64::Encode(m_encrypted_text); 
			ENIGMA_ASSERT_OR_THROW(!m_encrypted_text_base64.empty(), "Failed to encode cipher text to Base64");
		}
		catch (const std::exception& e)
		{
			ENIGMA_ERROR(e.what());
			(void)DialogUtils::Error(e.what());
		}
	}
}

void EncryptTextScene::OnBackButtonPressed()
{
	if ((!m_text.empty() || !m_password.empty() || !m_confirm_password.empty()))
	{	// Show alert dialog to user asking whether the operation should be aborted
		const auto action = DialogUtils::Question("Are you sure you want to cancel the entire operation?");
		if (action == Enigma::MessageBox::Action::Yes)
		{
			this->EndScene();
		}
	}
	else
		this->EndScene();
}

void EncryptTextScene::OnCopyEncryptedBase64TextButtonPressed()
{
	Enigma::Clipboard::Set(m_encrypted_text_base64);
}

