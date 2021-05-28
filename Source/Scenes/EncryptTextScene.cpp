#include "pch.hpp"
#include "EncryptTextScene.hpp"
#include <Utility/DialogUtils.hpp>
#include <GUI/ImGuiWidgets.hpp>
#include <Utility/GZip.hpp>
#include <System/Clipboard/Clipboard.hpp>

NS_ENIGMA_BEGIN

EncryptTextScene::EncryptTextScene()
	:
	Enigma::Scene(),
	m_type(Algorithm::Type::AES) // default
{
}

void EncryptTextScene::OnCreate()
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

void EncryptTextScene::OnUpdate(const f32& /*dt*/)
{}

void EncryptTextScene::OnDraw()
{
	// Clear GL buffers
	//glAssert(glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
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

	static const auto& fonts = Application::GetInstance()->GetFonts();
	static ImFont* const& font_audiowide_regular_45 = fonts.at("Audiowide-Regular-45");
	static ImFont* const& font_audiowide_regular_30 = fonts.at("Audiowide-Regular-30");
	static ImFont* const& font_audiowide_regular_20 = fonts.at("Audiowide-Regular-20");
	static ImFont* const& font_montserrat_medium_20 = fonts.at("Montserrat-Medium-20");
	static ImFont* const& font_montserrat_medium_18 = fonts.at("Montserrat-Medium-18");
	static ImFont* const& font_montserrat_medium_16 = fonts.at("Montserrat-Medium-16");
	static ImFont* const& font_montserrat_medium_12 = fonts.at("Montserrat-Medium-12");

	static constexpr const auto container_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;// | ImGuiWindowFlags_NoBackground;

	// Push window's background color
	ImGui::PushStyleColor(ImGuiCol_WindowBg, Constants::Colors::BACKGROUND_COLOR);

	ImGui::Begin("Container", nullptr, container_flags);
	ImGui::SetWindowSize(ImVec2(static_cast<f32>(win_w), static_cast<f32>(win_h))); // same size as window
	ImGui::SetWindowPos(ImVec2(0.0f, 0.0f)); // top left
	{
		// Back button [<] & Title
		{
			static const auto& title_font = font_audiowide_regular_30;
			static constexpr const auto title = "Encrypt Text";
			static const ImVec2 title_size((ImGui::CalcTextSize(title).x * title_font->Scale) - 45.0f, ImGui::CalcTextSize(title).y * title_font->Scale);
			static const ImVec2 back_button_size(45.0f, title_size.y);

			// Back Button [<]
			{
				ImGui::PushFont(font_montserrat_medium_18); // < arrow is a text too
				if (ImGuiWidgets::BackButton("##back", back_button_size))
				{
					this->OnBackButtonPressed();
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

		spacing(3);
		ImGui::Separator();
		spacing(3);

		// Algorithm To encrypt text with
		ImGui::PushFont(font_audiowide_regular_20);
		{
			// Label
			//ImGui::LabelText("##label", "Algorithm:");
			ImGui::Text("Algorithm:");
			ImGui::NewLine();

			// Algo types radio buttons
			const static auto supported_algorithms = Algorithm::GetSupportedAlgorithms();
			for (const auto& [algo_name, algo_type] : supported_algorithms)
			{
				inline_dummy(1.0f, 0.0f);
				ImGui::SameLine();
				if (ImGui::RadioButton(algo_name.c_str(), m_type == algo_type))
				{
					m_type = algo_type;
				}
			}
			/*const static auto supported_algorithms = Algorithm::GetSupportedAlgorithms();
			for (const auto& [algo_name, algo_type] : supported_algorithms)
			{
				inline_dummy(6.0f, 0.0f);
				ImGui::SameLine();
				if (ImGui::RadioButton(algo_name.c_str(), m_algorithm->GetType() == algo_type))
				{
					m_algorithm->SetType(algo_type);
				}
			}*/
		}
		ImGui::PopFont();

		spacing(2);

		// Save to database widget
		ImGui::PushFont(font_montserrat_medium_16);
		{
			ImGui::PushFont(font_audiowide_regular_20);
				ImGui::Text("Save to database:");
			ImGui::PopFont();
			inline_dummy(6.0f, 0.0f);
			ImGui::SameLine();
			ImGui::Checkbox("##checkbox", &m_save_to_database);

			if (m_save_to_database)
			{
				ImGui::Text("Encryption Title:");
				ImGuiWidgets::InputTextWithHint("##idb", "(e.g: An important message) helps with searching through encryption records in the future", &m_db_title, win_w / 1.3f);
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
			ImGui::Text("Text:");

			// Input text
			const ImVec2 input_text_size(static_cast<f32>(win_w), ImGui::GetTextLineHeightWithSpacing() * 5);
			ImGuiWidgets::InputTextMultiline("##text1", &m_text, input_text_size);

			// Bytes count
			ImGui::PushFont(font_montserrat_medium_12);
			ImGui::Text("%s", SizeUtils::FriendlySize( m_text.size() ).c_str());
			ImGui::PopFont();
		}
		ImGui::PopFont();

		//spacing(3);

		spacing(3);
		ImGui::Separator();
		spacing(3);
			
		// Encryption Password & Confirm password
		ImGui::PushFont(font_montserrat_medium_20);
		{		
			// password text coloring for each state
			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text,
				(m_password.empty() && m_confirm_password.empty()) ? // if password or confirm password is empty...
				Constants::Colors::TEXT_COLOR :  // ... set color to white
				m_password == m_confirm_password ?  // else if password matches confim password...
				Constants::Colors::PASSWORD_MATCH_TEXT_COLOR : // ... set color to green
				Constants::Colors::ERROR_TEXT_COLOR // else set color to red.
			); 
			// Label
			ImGui::Text("Password:");
			// Input text
			ImGuiWidgets::InputText("##text2", &m_password, static_cast<f32>(win_w), ImGuiInputTextFlags_::ImGuiInputTextFlags_Password);
			ImGui::Text("Confirm Password:");
			ImGuiWidgets::InputText("##text3", &m_confirm_password, static_cast<f32>(win_w), ImGuiInputTextFlags_::ImGuiInputTextFlags_Password);
			ImGui::PopStyleColor();
			// Bytes count
			ImGui::PushFont(font_montserrat_medium_12);
				//ImGui::Text("%zu bytes", m_password.size());
				ImGui::Text("%s", SizeUtils::FriendlySize( m_password.size() ).c_str());
			ImGui::PopFont();
		}
		ImGui::PopFont();



		// Encrypted text in Base64 output
		if (!m_cipher.empty())
		{
			spacing(3);
			ImGui::Separator();
			spacing(3);

			ImGui::PushFont(font_montserrat_medium_20);
			{
				// Label
				ImGui::Text("Cipher (in base64):");
				// Encrypted text
				static const ImVec2 copy_button_size(45.0f, 25.0f);
				ImGuiWidgets::InputText("##text4", &m_cipher_base64, win_w - (copy_button_size.x * 2.0f));
				ImGui::PushFont(font_montserrat_medium_12);
					ImGui::SameLine();
					ImGui::PushStyleColor(ImGuiCol_Button, Constants::Colors::BUTTON_COLOR); // buttons color idle
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Constants::Colors::BUTTON_COLOR_HOVER);  // buttons color hover
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, Constants::Colors::BUTTON_COLOR_ACTIVE); // buttons color pressed
					if (ImGui::Button("Copy", copy_button_size))
					{
						this->OnCopyEncryptedBase64TextButtonPressed();
					}
					//ImGui::NewLine();
					ImGui::Text("%s (Please save cipher base64 text above in a safe place)", SizeUtils::FriendlySize(m_cipher_base64.size()).c_str());
					ImGui::PopStyleColor(3);
				ImGui::PopFont();
			}
			ImGui::PopFont();
		}

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
				if (ImGui::Button("Encrypt", button_size))
				{
					Application::GetInstance()->LaunchWorkerThread(this, "Encrypting text...", [this]() -> void
						{
							this->OnEncryptButtonPressed();
						});
				}

			}
			ImGui::PopStyleColor(3);
			ImGui::PopFont();
#if 0
			ImGui::PushFont(font_audiowide_regular_20); // buttons font
			ImGui::SetCursorPosX((io.DisplaySize.x - button_size.x * 2) / 2.0f);
			ImGui::SetCursorPosY((io.DisplaySize.y - button_size.y) - 10.0f);
			if (ImGuiWidgets::Button("Back", button_size, Constants::Colors::BACK_BUTTON_COLOR, Constants::Colors::BACK_BUTTON_COLOR_HOVER, Constants::Colors::BACK_BUTTON_COLOR_ACTIVE))
			{
				this->OnBackButtonPressed();
			}
			ImGui::PopFont();


			ImGui::PushFont(font_audiowide_regular_20); // buttons font
			ImGui::PushStyleColor(ImGuiCol_Button, Constants::Colors::BUTTON_COLOR); // buttons color idle
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Constants::Colors::BUTTON_COLOR_HOVER);  // buttons color hover
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, Constants::Colors::BUTTON_COLOR_ACTIVE); // buttons color pressed
			{
				ImGui::SameLine();
				if (ImGui::Button("Encrypt", button_size))
				{
					Application::GetInstance()->LaunchWorkerThread(this, [this]() -> void
					{
						this->OnEncryptButtonPressed();
					});

					/*std::thread working_thread([this]() -> void
						{
							std::scoped_lock<std::mutex> guard{ Scene::GetMutex() };

							ENIGMA_LOG("Launching worker thread id #{0}", std::this_thread::get_id());

							Scene::SetLoading(true);
								this->OnEncryptButtonPressed();
							Scene::SetLoading(false);

							ENIGMA_LOG("Finished worker thread id #{0}", std::this_thread::get_id());
						});
					working_thread.detach();*/
					/*Application::GetInstance()->LaunchWorkerThread(this,
					[this]() -> void
					{
						this->OnEncryptButtonPressed();
					});*/

					//this->OnEncryptButtonPressed();
				}
			}
			ImGui::PopStyleColor(3);
#endif
		}
		

	}
	ImGui::End();

	// Pop window's background color
	ImGui::PopStyleColor(1);

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
	ENIGMA_TRACE_CURRENT_FUNCTION();

	m_password.clear();
	m_confirm_password.clear();
	m_cipher.clear();
	m_cipher_base64.clear();
	m_text.clear();
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
		(void)DialogUtils::Warn(Constants::ErrorMessages::WEAK_PASSWORD);
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
			const auto algorithm = Algorithm::CreateFromType(m_type, Algorithm::Intent::Encrypt);
			ENIGMA_ASSERT_OR_THROW(algorithm, "Failed to create algorithm from type");


			/*
			Note: You should compress before encrypting. Encryption turns your data into high-entropy data,
				 usually indistinguishable from a random stream. Compression relies on patterns in order to gain
				 any size reduction. Since encryption destroys such patterns, the compression algorithm would be
				 unable to give you much (if any) reduction in size if you apply it to encrypted data.
			*/

			// Compress text before encrypting
			const String compressed_text = GZip::Compress(m_text);
			ENIGMA_ASSERT_OR_THROW(!compressed_text.empty(), "Failed to compress text");

			// Encrypt text
			m_cipher = algorithm->Encrypt(m_password, compressed_text);
			ENIGMA_ASSERT_OR_THROW(!m_cipher.empty(), "Failed to encrypt text");

			// Encode cipher to Base64
			m_cipher_base64 = Base64::Encode(m_cipher);
			ENIGMA_ASSERT_OR_THROW(!m_cipher_base64.empty(), "Failed to encode cipher text to Base64");

			// Save to database
			if (m_save_to_database)
			{
				ENIGMA_ASSERT_OR_THROW(ENIGMA_IS_BETWEEN(m_db_title.size(), 3, 255), "Encryption title is too long or short, must be between 3 and 255 characters");
				auto e = std::make_unique<Encryption>();
				e->title = m_db_title;
				e->is_file = false;
				e->cipher.data = m_cipher;
				e->size = e->cipher.data.size();
				ENIGMA_ASSERT_OR_THROW(Database::AddEncryption(e), "Failed to save encryption record to database");
			}

			// Spawn notification alert if window is not focused
			//if (!Application::GetInstance()->GetWindow()->IsFocused())
			//{
			//	Notification{ "Enigma", "Successfully Encrypted Text" }.Show();
			//}


#if 0
			// Create encryptor based on selected algorithm type
			m_algorithm = Algorithm::CreateFromType(m_algorithm->GetType(), Algorithm::Intent::Encrypt);
			ENIGMA_ASSERT_OR_THROW(m_algorithm, "Failed to create algorithm from type");

			// Encrypt text
			m_cipher = m_algorithm->Encrypt(m_password, m_text);
			ENIGMA_ASSERT_OR_THROW(!m_cipher.empty(), "Failed to encrypt text");

			// Encode cipher to Base64
			m_cipher_base64 = Base64::Encode(m_cipher); 
			ENIGMA_ASSERT_OR_THROW(!m_cipher_base64.empty(), "Failed to encode cipher text to Base64");

			// Spawn notification alert if window is not focused
			//if (! Application::GetInstance()->GetWindow()->IsFocused())
			//{
			//	Notification{ "Enigma", "Successfully Encrypted Text" }.Show();
			//}
#endif 
		}
		catch (const CryptoPP::Exception& e)
		{
			const String err_msg = CryptoPPUtils::GetFullErrorMessage(e);
			ENIGMA_ERROR("Encryption Failure: {0}", err_msg);
			(void)DialogUtils::Error("Encryption Failure", err_msg);
		}
		catch (const std::exception& e)
		{
			ENIGMA_ERROR("Encryption Failure: {0}", e.what());
			(void)DialogUtils::Error("Encryption Failure", e.what());
		}
		catch (...)
		{
			const String err_msg = "Encryption Failure: Unknown Error";
			ENIGMA_ERROR(err_msg);
			(void)DialogUtils::Error(err_msg);
		}
	}
}

void EncryptTextScene::OnBackButtonPressed()
{
	//if (!m_text.empty() || !m_password.empty() || !m_confirm_password.empty())
	//{	// Show alert dialog to user asking whether the operation should be aborted
	//	const auto action = DialogUtils::Question("Are you sure you want to cancel the entire operation?");
	//	if (action == Enigma::MessageBox::Action::Yes)
	//	{
	//		Scene::EndScene();
	//	}
	//}
	//else
		Scene::EndScene();
}

void EncryptTextScene::OnCopyEncryptedBase64TextButtonPressed()
{
	Clipboard::Set(m_cipher_base64);
}


NS_ENIGMA_END
