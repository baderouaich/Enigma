#include "pch.hpp"
#include "EncryptFileScene.hpp"
#include <Utility/ImGuiUtils.hpp>
#include <Utility/DialogUtils.hpp>
#include <System/Dialogs/MessageBox.hpp>
#include <System/Dialogs/OpenFileDialog.hpp>
#include <System/Dialogs/SaveFileDialog.hpp>
#include <System/Dialogs/SelectFolderDialog.hpp>

EncryptFileScene::EncryptFileScene(const std::unordered_map<std::string_view, ImFont*>& fonts)
	:
	Enigma::Scene(),
	m_fonts(fonts),
	//AES will be first selected in Radio buttons as default, must be initialized for apply algo->GetType()
	m_algorithm(Algorithm::CreateFromType(Algorithm::Type::AES, Algorithm::Intent::Encrypt))
{
}

void EncryptFileScene::OnCreate()
{
	ENIGMA_TRACE(ENIGMA_CURRENT_FUNCTION);

	// Set background clear color
	glAssert(glClearColor(
		Constants::Colors::BACKGROUND_COLOR.x,
		Constants::Colors::BACKGROUND_COLOR.y,
		Constants::Colors::BACKGROUND_COLOR.z,
		Constants::Colors::BACKGROUND_COLOR.w
	));

	// Set default m_in_filename as enigma exe path
	m_in_filename = fs::current_path().string();

}

void EncryptFileScene::OnUpdate(const f32& dt)
{}

void EncryptFileScene::OnDraw()
{
	// Clear GL buffers
	glAssert(glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}

void EncryptFileScene::OnImGuiDraw()
{
	const auto& [win_w, win_h] = Application::GetInstance()->GetWindow()->GetSize();
	const auto& [win_x, win_y] = Application::GetInstance()->GetWindow()->GetPosition();
	static const auto& io = ImGui::GetIO();

	const auto button_size = Vec2f(win_w / 2.5f, 40.0f);

	static constexpr const auto inline_dummy = [](const f32& x, const f32& y) noexcept {  ImGui::SameLine(); ImGui::Dummy(ImVec2(x, y)); };
	static constexpr const auto spacing = [](const ui8& n) noexcept { for (ui8 i = 0; i < n; i++) ImGui::Spacing(); };

	static ImFont* const& font_audiowide_regular_45 = m_fonts.at("Audiowide-Regular-45");
	static ImFont* const& font_audiowide_regular_20 = m_fonts.at("Audiowide-Regular-20");
	static ImFont* const& font_montserrat_medium_20 = m_fonts.at("Montserrat-Medium-20");
	static ImFont* const& font_montserrat_medium_12 = m_fonts.at("Montserrat-Medium-12");

	static constexpr const auto container_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground;

	ImGui::Begin("Container", nullptr, container_flags);
	ImGui::SetWindowSize(ImVec2(static_cast<f32>(win_w), static_cast<f32>(win_h))); // same size as window
	ImGui::SetWindowPos(ImVec2(0.0f, 0.0f)); // top left
	{
		// Scene Title
		ImGui::PushFont(font_audiowide_regular_45); // text font
		ImGui::PushStyleColor(ImGuiCol_Text, Constants::Colors::TEXT_COLOR); // text color
		{
			static constexpr const auto title = "Encrypt File";
			static const ImVec2 title_size(ImGui::CalcTextSize(title).x * font_audiowide_regular_45->Scale, ImGui::CalcTextSize(title).y * font_audiowide_regular_45->Scale);
			ImGui::SetCursorPosX((io.DisplaySize.x - title_size.x) / 2.0f);
			ImGui::Text(title);
		}
		ImGui::PopStyleColor(1);
		ImGui::PopFont();

		spacing(3);
		ImGui::Separator();
		spacing(3);

		// Algorithm To encrypt File with
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
		}
		ImGui::PopFont();

		spacing(2);
		ImGui::Separator();
		spacing(2);

		// In File to Encrypt
		ImGui::PushFont(font_montserrat_medium_20);
		{
			// Label
			ImGui::Text("File To Encrypt:");
			// Encrypted text
			static const ImVec2 browse_button_size(45.0f, 25.0f);
			ImGuiUtils::InputText("##text1", &m_in_filename, win_w - (browse_button_size.x * 1.5f));
			ImGui::PushFont(font_montserrat_medium_12);
				ImGui::SameLine();
				if (ImGui::Button("Browse", browse_button_size))
				{
					this->OnBrowseInFileButtonPressed();
				}
			ImGui::PopFont();
		}
		ImGui::PopFont();
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
			ImGuiUtils::InputText("##text2", &m_password, static_cast<f32>(win_w), ImGuiInputTextFlags_::ImGuiInputTextFlags_Password);
			ImGui::Text("Confirm Password:");
			ImGuiUtils::InputText("##text3", &m_confirm_password, static_cast<f32>(win_w), ImGuiInputTextFlags_::ImGuiInputTextFlags_Password);
			ImGui::PopStyleColor();
			// Bytes count
			ImGui::PushFont(font_montserrat_medium_12);
			ImGui::Text("%llu bytes", m_password.size());
			ImGui::PopFont();
		}
		ImGui::PopFont();

		spacing(2);
		ImGui::Separator();
		spacing(2);

		// Encrypt & Back Button 
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
		// Out File Encrypted
		/*ImGui::PushFont(font_montserrat_medium_20);
		{
			// Label
			ImGui::Text("Save Encrypted File To:");
			// Encrypted text
			static const ImVec2 browse_button_size(45.0f, 25.0f);
			ImGuiUtils::InputText("##text2", &m_out_file_path, win_w - (browse_button_size.x * 1.5f));
			ImGui::PushFont(font_montserrat_medium_12);
			ImGui::SameLine();
			if (ImGui::Button("Browse ", browse_button_size))
			{
				this->OnBrowseOutFileButtonClicked();
			}
			ImGui::PopFont();
		}
		ImGui::PopFont();*/



	}
	ImGui::End();
}

void EncryptFileScene::OnEvent(Event& event)
{}

void EncryptFileScene::OnDestroy()
{
	ENIGMA_LOG(ENIGMA_CURRENT_FUNCTION);

}

void EncryptFileScene::OnBrowseInFileButtonPressed()
{
	const auto ofd = std::make_unique<Enigma::OpenFileDialog>(
		"Select A File To Encrypt",
		m_in_filename, // initial path
		false // disable multi-select
	);
	
	ENIGMA_TRACE("Selecting a file to encrypt...");
	const std::vector<String> selected_file_paths = ofd->Show();
	if (selected_file_paths.empty())
	{
		ENIGMA_TRACE("Nothing is selected.");
		return;
	}
	m_in_filename = *selected_file_paths.begin();

	//if (fs::exists(*selected_file_paths.begin()) && fs::is_regular_file(*selected_file_paths.begin()))
	//{
	//	ENIGMA_TRACE("Selected file: {0}", m_in_filename);
	//}
	//else
	//{
	//	ENIGMA_WARN("File {0} does not exist", *selected_file_paths.begin());
	//	(void)DialogUtils::Warn("File " + *selected_file_paths.begin() + " does not exist");
	//}
}

void EncryptFileScene::OnEncryptButtonPressed()
{
	// Validate fields 
	if (m_in_filename.empty())
	{
		(void)DialogUtils::Warn("In File to encrypt is empty");
	}
	else if (!fs::exists(m_in_filename) || !fs::is_regular_file(m_in_filename))
	{
		(void)DialogUtils::Warn("In File does not exist");
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

			// Read in file buffer
			String buffer{};
			const bool file_read_success = FileUtils::Read(m_in_filename, buffer);
			ENIGMA_ASSERT_OR_THROW(file_read_success, "Failed to read buffer from file " + m_in_filename);
			ENIGMA_ASSERT_OR_THROW(!buffer.empty(), "File " + m_in_filename + " is empty");
			
			// Encrypt file buffer
			String cipher = m_algorithm->Encrypt(m_password, buffer);
			ENIGMA_ASSERT_OR_THROW(!cipher.empty(), "Failed to encrypt file buffer");

			// Save cipher to out file encrypted
			// Ask user where should we save it
			const auto sfd = std::make_unique<Enigma::SaveFileDialog>(
				"Select Where To Save Encrypted File",
				m_in_filename + ".enigma", // in_file.txt.enigma
				true // force overwrite
				);
			const String m_out_filename = sfd->Show();
			//TODO
			ENIGMA_ASSERT_OR_THROW(!m_out_filename.empty(), "Invalid output file name");

			// Write cipher to out file
			const bool file_written_success = FileUtils::Write(m_out_filename, cipher);
			ENIGMA_ASSERT_OR_THROW(file_written_success, "Failed to write cipher to file " + m_out_filename);


			// Alert user that encryption was successfull
			(void)Enigma::MessageBox{ "Enigma", "Encrypted" + m_in_filename + " => " + m_out_filename + " Successfully!", Enigma::MessageBox::Icon::Info, Enigma::MessageBox::Choice::Ok }.Show();

		}
		catch (const CryptoPP::Exception& e)
		{
			const String err_msg = CryptoPPUtils::GetFullErrorMessage(e);
			ENIGMA_ERROR("Encryption Failure: {}", err_msg);
			(void)DialogUtils::Error("Encryption Failure", err_msg);
		}
		catch (const std::exception& e)
		{
			ENIGMA_ERROR("Encryption Failure: {}", e.what());
			(void)DialogUtils::Error("Encryption Failure", e.what());
		}
		catch (...)
		{
			const String err_msg = "Encryption Failure: Unknown Error";
			ENIGMA_ERROR("Encryption Failure: Unknown Error");
			(void)DialogUtils::Error(err_msg);
		}
	}

}

void EncryptFileScene::OnBackButtonPressed()
{
	this->EndScene();
}

/*
void EncryptFileScene::OnBrowseOutFileButtonClicked()
{
	const auto sfd = std::make_unique<Enigma::SelectFolderDialog>(
		"Select Where To Save Encrypted File To",
		m_out_file_path, // initial path
		true // force overwrite
		);

	ENIGMA_TRACE("Selecting an output encyrpted file...");
	m_out_file_path = sfd->Show();
	if (fs::is_directory(m_out_file_path))
	{
		fs::path p = m_out_file_path;
		p /=  fs::path(m_in_filename).filename().string() + ".enigma";
		m_out_file_path = p.string();
		ENIGMA_TRACE("Selected Dir: {0}", m_out_file_path);
	}
	else
	{
		ENIGMA_WARN("Path {0} is not valid", m_out_file_path);
		(void)DialogUtils::Warn("Path " + m_out_file_path + " is invalid");
	}

}
*/
