#include "pch.hpp"
#include "EncryptTextScene.hpp"
#include <Utility/DialogUtils.hpp>
#include <System/Clipboard/Clipboard.hpp>

EncryptTextScene::EncryptTextScene(const std::unordered_map<std::string_view, ImFont*>& fonts)
	:
	Enigma::Scene(),
	m_fonts(fonts),
	// AES as default algorithm
	m_algorithm(Algorithm::Type::AES),
	// Max text length to encrypt 1mb
	m_text(1024 * 1024 * 1, '\000'),
	// Max encryption password length 1024 characters
	m_password(1024, '\000')
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
	const auto& [win_w, win_h] = Enigma::Application::GetInstance().GetWindow()->GetSize();
	const auto& [win_x, win_y] = Enigma::Application::GetInstance().GetWindow()->GetPosition();
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
			if (ImGui::RadioButton("AES", m_algorithm == Algorithm::Type::AES))
			{
				m_algorithm = Algorithm::Type::AES;
			}
			inline_dummy(0.0f, 1.0f);
			ImGui::SameLine();
			if (ImGui::RadioButton("CHACHA", m_algorithm == Algorithm::Type::ChaCha))
			{
				m_algorithm = Algorithm::Type::ChaCha;
			}
		}
		ImGui::PopFont();

		spacing(3);

		// Text to encrypt
		ImGui::PushFont(font_montserrat_medium_20);
		{
			// Label
			ImGui::Text("Text to Encrypt:");
			// Input text
			/*ImGui::InputTextMultiline("##text1", m_text.data(), m_text.size(),
				input_text_size,
				ImGuiInputTextFlags_CallbackResize | ImGuiInputTextFlags_CallbackCharFilter,
				[](ImGuiTextEditCallbackData* data)->int
				{
					String& mystr = *static_cast<String*>(data->UserData);
					mystr.resize(data->BufSize);
					mystr = data->Buf;
				//	data->EventChar = *static_cast<const char*>(data->UserData);
					return mystr.size();
				}, 
				&m_text
			);
			*/
			const ImVec2 input_text_size(static_cast<f32>(win_w) - 10.0f, 60.0f);
			ImGui::InputTextMultiline("##text1", m_text.data(), m_text.size(), input_text_size);
			// Remaining characters
			ImGui::PushFont(font_montserrat_medium_12);
			ImGui::Text("%llu of %llu characters - 1MB MAX", std::strlen(m_text.data()), m_text.size());
			ImGui::PopFont();
		}
		ImGui::PopFont();

		spacing(3);

		// Encryption Password
		ImGui::PushFont(font_montserrat_medium_20);
		{
			// Label
			ImGui::Text("Password:");
			// Input text
			const ImVec2 input_text_size(static_cast<f32>(win_w) - 10.0f, 30.0f);
			ImGui::InputTextMultiline("##text2", m_password.data(), m_password.size(), input_text_size);
			// Remaining characters
			ImGui::PushFont(font_montserrat_medium_12);
			ImGui::Text("%llu of %llu characters - 1KB MAX", std::strlen(m_password.data()), m_password.size());
			ImGui::PopFont();
		}
		ImGui::PopFont();

		spacing(3);

		// Encrypted text in Base64 output
		if (!m_encrypted_text.empty())
		{
			ImGui::PushFont(font_montserrat_medium_20);
			{
				// Label
				ImGui::Text("Encrypted Text (in Base64):");
				// Encrypted text
				const ImVec2 copy_button_size(45.0f, 30.0f);
				const ImVec2 input_text_size(static_cast<f32>(win_w) - 10.0f - copy_button_size.x, 60.0f);
				ImGui::InputTextMultiline("##text3", m_encrypted_text_base64.data(), m_encrypted_text_base64.size(), input_text_size);
				ImGui::Text("%llu characters", m_encrypted_text_base64.size());
				ImGui::SameLine();
				if (ImGui::Button("Copy", copy_button_size))
				{
					this->OnCopyEncryptedBase64TextButtonPressed();
				}
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
				if (ImGui::Button("Cancel", button_size))
				{
					this->OnCancelButtonPressed();
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
	LOG(ENIGMA_CURRENT_FUNCTION);

	m_text.clear();
	m_password.clear();
	m_encrypted_text.clear();
	m_encrypted_text_base64.clear();
}

void EncryptTextScene::OnEncryptButtonPressed()
{
	// Validate fields 
	{
		//todo, use std::strlen()
		if (StringUtils::IsAll(m_text, '\000'))
		{
			ENIGMA_TRACE("m_text is empty");
			[[maybe_unused]] auto _ = DialogUtils::Warn("Text to encrypt is empty");
		}
		else if (StringUtils::IsAll(m_password, '\000'))
		{
			ENIGMA_TRACE("m_password is empty");
			[[maybe_unused]] auto _ = DialogUtils::Warn("Encryption Password is empty");
		}
		else
		{
			// TODO MORE WORK ON \000 headaches //

			// Create encryptor based on selected algorithm type
			std::unique_ptr<Enigma::Algorithm> algo_encryptor = nullptr;
			switch (m_algorithm)
			{
			case Enigma::Algorithm::Type::AES:
				algo_encryptor.reset(new Enigma::AES(Enigma::Algorithm::Intent::Encrypt));
				break;
			case Enigma::Algorithm::Type::ChaCha:
				algo_encryptor.reset(new Enigma::ChaCha(Enigma::Algorithm::Intent::Encrypt));
				break;
			}
			// Encrypt text
			m_encrypted_text = algo_encryptor->Encrypt(m_password, m_text);
			ENIGMA_ASSERT(!m_encrypted_text.empty(), "Failed to encrypt text");
			//m_encrypted_text.resize(std::strlen(m_encrypted_text.data()) + 1); // remove \000
			m_encrypted_text_base64 = Base64::Encode(String(m_encrypted_text.begin(), m_encrypted_text.begin() + m_encrypted_text.find_first_of('\000'))); // remove \000
			ENIGMA_ASSERT(!m_encrypted_text_base64.empty(), "Failed to encode encrypted text to Base64");

			ENIGMA_INFO("Encrypted");
		}
	}

}

void EncryptTextScene::OnCancelButtonPressed()
{
	// Show alert dialog to user asking whether the operation should be aborted
	const auto action = DialogUtils::Question("Are you sure you want to cancel the entire operation?"); 
	if (action == Enigma::MessageBox::Action::Yes)
	{
		this->EndScene();
	}
}

void EncryptTextScene::OnCopyEncryptedBase64TextButtonPressed()
{
	Enigma::Clipboard::Set(m_encrypted_text_base64);
}

