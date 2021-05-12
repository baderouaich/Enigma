#include <pch.hpp>
#include "ViewEncryptionScene.hpp"

#include <imgui.h>

NS_ENIGMA_BEGIN

ViewEncryptionScene::ViewEncryptionScene(const i64 encryption_id)
	:
	Enigma::Scene()
{
	ENIGMA_TRACE("Gettings encryption#{0}'s data from database...", encryption_id);
	
	// Get Encryption record from database (minus cipher to save up memory)
	m_encryption = Database::GetEncryptionByID<true, false, true, true, true>(encryption_id);
	if (!m_encryption)
	{
		(void)DialogUtils::Error("Couldn't get encryption record from database");
		Scene::EndScene();
	}
	std::cout << m_encryption->title;
}

void ViewEncryptionScene::OnCreate()
{
	ENIGMA_TRACE_CURRENT_FUNCTION();
}

void ViewEncryptionScene::OnUpdate(const f32&)
{
}

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

	static constexpr const auto inline_dummy = [](const f32& x, const f32& y) noexcept {  ImGui::SameLine(); ImGui::Dummy(ImVec2(x, y)); ImGui::SameLine(); };
	static constexpr const auto spacing = [](const ui8& n) noexcept { for (ui8 i = 0; i < n; i++) ImGui::Spacing(); };

	static const auto& fonts = Application::GetInstance()->GetFonts();
	static ImFont* const& font_audiowide_regular_45 = fonts.at("Audiowide-Regular-45");
	static ImFont* const& font_audiowide_regular_30 = fonts.at("Audiowide-Regular-30");
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
		// Back button [<] & Title
		{
			static const auto& title_font = font_audiowide_regular_30;
			static constexpr const auto title = "View Encryption";
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


		spacing(2);
		ImGui::Separator();
		spacing(2);

		// Encryption info (title \n Date time  - size)
		{
			// title
			ImGui::PushFont(font_audiowide_regular_20); 
			ImGui::TextWrapped("%s", m_encryption->title.c_str()); // we had an issue here, abort was called because the random generated string contains % confuses imgui which thinks it needs format string, solved by adding %s then text
			ImGui::PopFont();  

			spacing(2);

			// Date time  - size
			ImGui::PushFont(font_montserrat_medium_18);
			{
				static const String format = String("Format: ") + (m_encryption->is_file ? "File" : "Text");
				ImGui::ButtonEx(format.c_str(), { 0.0f, 0.0f }, ImGuiButtonFlags_Disabled);
				inline_dummy(6.0f, 0.0f);

				static const String date_time = String("Date Time: ") + m_encryption->date_time;
				ImGui::ButtonEx(date_time.c_str(), { 0.0f, 0.0f }, ImGuiButtonFlags_Disabled);
				inline_dummy(6.0f, 0.0f);

				static const String size = SizeUtils::FriendlySize(m_encryption->size);
				ImGui::ButtonEx(size.c_str(), { 0.0f, 0.0f }, ImGuiButtonFlags_Disabled);

			}
			//const auto text_size = ImGui::CalcTextSize(text.c_str());
			//ImGui::SetCursorPosX((win_w - (text_size.x)) / 2.0f);
			//ImGui::TextWrapped(text.c_str());
			//ImGui::TextWrapped("%s\t-\t%s", m_encryption->date_time.c_str(), SizeUtils::FriendlySize(m_encryption->size).c_str());
			ImGui::PopFont();
		}

		spacing(2);
		ImGui::Separator();
		spacing(2);

			
		// Password used for encryption
		ImGui::PushFont(font_montserrat_medium_20);
		{
			// Label
			ImGui::Text("Password:");

			// Input text
			ImGuiWidgets::InputText("##password", &m_password, static_cast<f32>(win_w), ImGuiInputTextFlags_::ImGuiInputTextFlags_Password);

			// Bytes count
			ImGui::PushFont(font_montserrat_medium_12);
			//ImGui::Text("%zu bytes", m_password.size());
			ImGui::Text("%s", SizeUtils::FriendlySize(m_password.size()).c_str());
			ImGui::PopFont();
		}
		ImGui::PopFont();


		// Decrypted/Recovered Text
		if (!m_recovered_text.empty() && !m_encryption->is_file)
		{
			spacing(3);
			ImGui::Separator();
			spacing(3);


			ImGui::PushFont(font_montserrat_medium_20);
			{
				// Label
				ImGui::Text("Recovered Text: ");
				// Encrypted text
				static const ImVec2 copy_button_size(45.0f, 25.0f);
				ImGuiWidgets::InputTextMultiline("##enc_txt", &m_recovered_text, ImVec2(win_w - (copy_button_size.x * 1.5f), 63.0f));
				ImGui::PushFont(font_montserrat_medium_12);
				ImGui::SameLine();
				ImGui::PushStyleColor(ImGuiCol_Button, Constants::Colors::BUTTON_COLOR); // buttons color idle
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Constants::Colors::BUTTON_COLOR_HOVER);  // buttons color hover
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, Constants::Colors::BUTTON_COLOR_ACTIVE); // buttons color pressed
				if (ImGui::Button("Copy", copy_button_size))
				{
					this->OnCopyRecoveredTextButtonPressed();
				}
				ImGui::PopStyleColor(3);
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
				ImGui::SetCursorPosX((io.DisplaySize.x - button_size.x) / 2.0f);
				//ImGui::SetCursorPosY((io.DisplaySize.y - button_size.y) - 10.0f);
				if (ImGui::Button("Decrypt", button_size))
				{
					Application::GetInstance()->LaunchWorkerThread(this, std::string_view{ m_encryption->is_file ? "Decrypting file..." : "Decrypting text..." },
						[this]() -> void
						{
							this->OnDecryptButtonPressed();
						});

				}

			}
			ImGui::PopStyleColor(3);
			ImGui::PopFont();
		}

	}
	ImGui::End();

	// Pop window's background color
	ImGui::PopStyleColor(1);

}

void ViewEncryptionScene::OnEvent(Event& /*event*/)
{}

void ViewEncryptionScene::OnDestroy()
{
	ENIGMA_TRACE_CURRENT_FUNCTION();
}


void ViewEncryptionScene::OnBackButtonPressed()
{
	Scene::EndScene();
}


void ViewEncryptionScene::OnDecryptButtonPressed()
{
	if (m_password.empty())
	{
		(void)DialogUtils::Warn("Password is empty");
		return;
	}


	try
	{
		// Get encryption cipher from database, we havent got it at the beginning to save up memory
		auto cipher = Database::GetCipherByEncryptionID(m_encryption->ide);
		ENIGMA_ASSERT_OR_THROW(cipher, "Failed to get cipher by encryption id");

		// Auto detect algorithm used for encryption
		Algorithm::Type algo_type = Algorithm::DetectFromCipher(cipher->data);
		ENIGMA_INFO("Detected algorithm is {0}", Algorithm::AlgoTypeEnumToStr(algo_type));

		// Create encryptor based on selected algorithm type
		const auto algorithm = Algorithm::CreateFromType(algo_type, Algorithm::Intent::Decrypt);
		ENIGMA_ASSERT_OR_THROW(algorithm, "Failed to create algorithm from type");
		

		// Decrypt either file or text ...
		// ... case file
		if (m_encryption->is_file) 
		{
			// Decrypt cipher
			String buffer = algorithm->Decrypt(m_password, cipher->data);
			ENIGMA_ASSERT_OR_THROW(!buffer.empty(), "Failed to decrypt file");

			// Decompress buffer
			/*
			Note: You should compress before encrypting. Encryption turns your data into high-entropy data,
					usually indistinguishable from a random stream. Compression relies on patterns in order to gain
					any size reduction. Since encryption destroys such patterns, the compression algorithm would be
					unable to give you much (if any) reduction in size if you apply it to encrypted data.
			*/
			buffer = GZip::Decompress(buffer);
			ENIGMA_ASSERT_OR_THROW(!buffer.empty(), "Failed to decompress file buffer");

			// Ask where to save decrypted file ?
			// Get path to where decrypted file should be saved
			if (const String output_filename = SaveFileDialog{"Save Decrypted File To"}.Show(); !output_filename.empty())
			{
				// Write file recovered content to output file
				const bool saved = FileUtils::Write(output_filename, buffer);
				ENIGMA_ASSERT_OR_THROW(saved, "Failed to save decrypted file");

				// little happy msg
				DialogUtils::Info("Successfully decrypted file to " + output_filename);
			}

		}
		// ... case text
		else
		{
			// Decrypt cipher text
			m_recovered_text = algorithm->Decrypt(m_password, cipher->data);
			ENIGMA_ASSERT_OR_THROW(!m_recovered_text.empty(), "Failed to decrypt text");

			// Decompress recovered text
			/*
			Note: You should compress before encrypting. Encryption turns your data into high-entropy data,
					usually indistinguishable from a random stream. Compression relies on patterns in order to gain
					any size reduction. Since encryption destroys such patterns, the compression algorithm would be
					unable to give you much (if any) reduction in size if you apply it to encrypted data.
			*/
			//ENIGMA_LOG("m_recovered_text size before {0}", m_recovered_text.size());
			m_recovered_text = GZip::Decompress(m_recovered_text);
			//ENIGMA_LOG("m_recovered_text size after {0}", m_recovered_text.size());
			ENIGMA_ASSERT_OR_THROW(!m_recovered_text.empty(), "Failed to decompress recovered text");
		}

	}
	catch (const CryptoPP::Exception& e)
	{
		const String err_msg = CryptoPPUtils::GetFullErrorMessage(e);
		ENIGMA_ERROR("Decryption Failure: {0}", err_msg);
		(void)DialogUtils::Error("Decryption Failure", err_msg);
	}
	catch (const std::exception& e)
	{
		ENIGMA_ERROR("Decryption Failure: {0}", e.what());
		(void)DialogUtils::Error("Decryption Failure", e.what());
	}
	catch (...)
	{
		const String err_msg = "Decryption Failure: Unknown Error";
		ENIGMA_ERROR("Decryption Failure: Unknown Error");
		(void)DialogUtils::Error(err_msg);
	}
}

void ViewEncryptionScene::OnCopyRecoveredTextButtonPressed()
{
	Clipboard::Set(m_recovered_text);
}
NS_ENIGMA_END

