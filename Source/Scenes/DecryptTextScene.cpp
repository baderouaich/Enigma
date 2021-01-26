#include "pch.hpp"
#include "DecryptTextScene.hpp"
#include <Utility/ImGuiUtils.hpp>
#include <Utility/DialogUtils.hpp>

NS_ENIGMA_BEGIN

DecryptTextScene::DecryptTextScene(const std::unordered_map<std::string_view, ImFont*>& fonts)
	:
	Enigma::Scene(),
	m_fonts(fonts),
	//AES will be first selected in Radio buttons as default, must be initialized for apply algo->GetType()
	m_algorithm(Algorithm::CreateFromType(Algorithm::Type::AES, Algorithm::Intent::Decrypt))
{
}

void DecryptTextScene::OnCreate()
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

void DecryptTextScene::OnUpdate(const f32& dt)
{}

void DecryptTextScene::OnDraw()
{
	// Clear GL buffers
	glAssert(glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}

void DecryptTextScene::OnImGuiDraw()
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
			static constexpr const auto title = "Decrypt Text";
			static const ImVec2 title_size(ImGui::CalcTextSize(title).x * font_audiowide_regular_45->Scale, ImGui::CalcTextSize(title).y * font_audiowide_regular_45->Scale);
			ImGui::SetCursorPosX((io.DisplaySize.x - title_size.x) / 2.0f);
			ImGui::Text(title);
		}
		ImGui::PopStyleColor(1);
		ImGui::PopFont();

		spacing(3);
		ImGui::Separator();
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
			ImGui::NewLine();

			ImGui::PushFont(font_audiowide_regular_20); // buttons font
			ImGui::PushStyleColor(ImGuiCol_Button, Constants::Colors::BUTTON_COLOR); // buttons color idle
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Constants::Colors::BUTTON_COLOR_HOVER);  // buttons color hover
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, Constants::Colors::BUTTON_COLOR_ACTIVE); // buttons color pressed
			ImGui::Text("Forgot which Algorithm used in encryption ?"); inline_dummy(6.0f, 0.0f); ImGui::SameLine();
			if (ImGui::Button("Auto-Detect Algorithm"))
			{
				this->OnAutoDetectAlgorithmButtonPressed();
			}
			ImGui::PopStyleColor(3);
			ImGui::PopFont();

		}
		ImGui::PopFont();

		spacing(3);
		ImGui::Separator();
		spacing(3);

		// Encrypted Text in Base64 to Decrypt
		ImGui::PushFont(font_montserrat_medium_20);
		{
			// Label
			ImGui::Text("Cipher (in base64):");

			// Input text
			const ImVec2 input_text_size(static_cast<f32>(win_w), ImGui::GetTextLineHeightWithSpacing() * 2.0f);
			ImGuiUtils::InputTextMultiline("##text1", &m_cipher_base64, input_text_size);

			// Bytes count
			ImGui::PushFont(font_montserrat_medium_12);
			ImGui::Text("%llu bytes", m_cipher_base64.size());
			ImGui::PopFont();
		}
		ImGui::PopFont();

		spacing(3);
		ImGui::Separator();
		spacing(3);

		// Password used for encryption
		ImGui::PushFont(font_montserrat_medium_20);
		{
			// Label
			ImGui::Text("Password:");

			// Input text
			ImGuiUtils::InputText("##text2", &m_password, static_cast<f32>(win_w), ImGuiInputTextFlags_::ImGuiInputTextFlags_Password);

			// Bytes count
			ImGui::PushFont(font_montserrat_medium_12);
			ImGui::Text("%llu bytes", m_password.size());
			ImGui::PopFont();
		}
		ImGui::PopFont();


		// Decrypted/Recovered Text
		if (!m_recovered_text.empty())
		{
			spacing(3);
			ImGui::Separator();
			spacing(3);

			ImGui::PushFont(font_montserrat_medium_20);
			{
				// Label
				ImGui::Text("Recovered Text:");

				// Input text
				const ImVec2 input_text_size(static_cast<f32>(win_w), ImGui::GetTextLineHeightWithSpacing() * 3.0f);
				ImGuiUtils::InputTextMultiline("##text3", &m_recovered_text, input_text_size);

				// Bytes count
				ImGui::PushFont(font_montserrat_medium_12);
				ImGui::Text("%llu bytes", m_recovered_text.size());
				ImGui::PopFont();

			}
			ImGui::PopFont();
		}


		spacing(3);
		ImGui::Separator();
		spacing(3);

		// Decrypt Button
		{
			ImGui::PushFont(font_audiowide_regular_20); // buttons font
			ImGui::PushStyleColor(ImGuiCol_Button, Constants::Colors::BUTTON_COLOR); // buttons color idle
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Constants::Colors::BUTTON_COLOR_HOVER);  // buttons color hover
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, Constants::Colors::BUTTON_COLOR_ACTIVE); // buttons color pressed
			{
				ImGui::PushStyleColor(ImGuiCol_Button, Constants::Colors::BACK_BUTTON_COLOR); // buttons color idle
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Constants::Colors::BACK_BUTTON_COLOR_HOVER);  // buttons color hover
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, Constants::Colors::BACK_BUTTON_COLOR_ACTIVE); // buttons color pressed
				ImGui::SetCursorPosX((io.DisplaySize.x - button_size.x * 2) / 2.0f);
				if (ImGui::Button("Back", button_size))
				{
					this->OnBackButtonPressed();
				}
				ImGui::PopStyleColor(3);

				ImGui::SameLine();
				if (ImGui::Button("Decrypt", button_size))
				{
					this->OnDecryptButtonPressed();
				}

			}
			ImGui::PopStyleColor(3);
			ImGui::PopFont();
		}

	}
	ImGui::End();
}

void DecryptTextScene::OnEvent(Event& event)
{
	//if (event.IsInCategory(EventCategory::EC_KEYBOARD))
	//{
	//	if (Input::IsKeyPressed(Enigma::Key::Escape))
	//	{
	//		this->EndScene();
	//	}
	//}
}

void DecryptTextScene::OnDestroy()
{
	ENIGMA_TRACE(ENIGMA_CURRENT_FUNCTION);

	m_recovered_text.clear(); 
	m_cipher.clear();
	m_cipher_base64.clear();
	m_password.clear();
}


void DecryptTextScene::OnAutoDetectAlgorithmButtonPressed()
{
	if (m_cipher_base64.empty())
	{
		(void)DialogUtils::Warn("Cannot auto-detect algorithm without cipher base64 text");
		return;
	}

	// Decode base64 to cipher
	m_cipher = Base64::Decode(m_cipher_base64);
	// check if successfully decoded
	if (m_cipher.empty())
	{
		(void)DialogUtils::Error("Failed to decode cipher base64! please make sure you have the exact cipher text you received on encryption");
		return;
	}

	// extract first byte from cipher which must be the mode type used in encryption
	const byte& cipher_first_byte = *m_cipher.begin();
	// Check if detected mode is valid
	if (!ENIGMA_IS_BETWEEN(cipher_first_byte, (byte)Algorithm::Type::First, (byte)Algorithm::Type::Last))
	{
		(void)DialogUtils::Error("Could not auto-detect algorithm mode used for encryption");
		return;
	}
	// if alles gut, create polymorphic algorithm decryptor
	m_algorithm = Algorithm::CreateFromType(static_cast<Algorithm::Type>(cipher_first_byte), Algorithm::Intent::Decrypt);
	// little happy info dialog
	(void)DialogUtils::Info("Successfully detected algorithm used for encryption which is: " + m_algorithm->GetTypeString());
}

void DecryptTextScene::OnBackButtonPressed()
{
	if (!m_cipher_base64.empty() || !m_password.empty() || !m_recovered_text.empty())
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

void DecryptTextScene::OnDecryptButtonPressed()
{
	if (m_cipher_base64.empty())
	{
		(void)DialogUtils::Warn("Cipher Base64 is empty");
		return;
	}
	if (m_password.empty())
	{
		(void)DialogUtils::Warn("Password is empty");
		return;
	}

	try
	{
		m_cipher = Base64::Decode(m_cipher_base64);
		ENIGMA_ASSERT_OR_THROW(!m_cipher.empty(), "Failed to decode cipher base64! please make sure you have the exact cipher text you received on encryption");
	
		// Create encryptor based on selected algorithm type
		m_algorithm = Algorithm::CreateFromType(m_algorithm->GetType(), Algorithm::Intent::Decrypt);
		ENIGMA_ASSERT_OR_THROW(m_algorithm, "Failed to create algorithm from type");
		
		m_recovered_text = m_algorithm->Decrypt(m_password, m_cipher);
		ENIGMA_ASSERT_OR_THROW(!m_recovered_text.empty(), "Failed to recover encrypted text");

		// Spawn notification alert if window is not focused
		if (!Application::GetInstance()->GetWindow()->IsFocused())
		{
			Notification{ "Enigma", "Successfully Decrypted Text" }.Show();
		}
	}
	catch (const CryptoPP::Exception& e)
	{
		const String err_msg = CryptoPPUtils::GetFullErrorMessage(e);
		ENIGMA_ERROR("Decryption Failure: {}", err_msg);
		(void)DialogUtils::Error("Decryption Failure", err_msg);
	}
	catch (const std::exception& e)
	{
		ENIGMA_ERROR("Decryption Failure: {}", e.what());
		(void)DialogUtils::Error("Decryption Failure", e.what());
	}
	catch (...)
	{
		const String err_msg = "Decryption Failure: Unknown Error";
		ENIGMA_ERROR("Decryption Failure: Unknown Error");
		(void)DialogUtils::Error(err_msg);
	}
	
	//TODO: handle decryption failure in a better way.
}

NS_ENIGMA_END
