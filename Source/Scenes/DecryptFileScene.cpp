#include "pch.hpp"
#include "DecryptFileScene.hpp"
#include <GUI/ImGuiWidgets.hpp>
#include <Utility/DialogUtils.hpp>
#include <System/Dialogs/MessageBox.hpp>
#include <System/Dialogs/OpenFileDialog.hpp>
#include <System/Dialogs/SaveFileDialog.hpp>
#include <System/Dialogs/SelectFolderDialog.hpp>

NS_ENIGMA_BEGIN

DecryptFileScene::DecryptFileScene(const std::unordered_map<std::string_view, ImFont*>& fonts)
	:
	Enigma::Scene(),
	m_fonts(fonts),
	m_type(Algorithm::Type::AES)
{
}

void DecryptFileScene::OnCreate()
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
	//m_in_filename = fs::current_path().string();

}

void DecryptFileScene::OnUpdate(const f32& dt)
{}

void DecryptFileScene::OnDraw()
{
	// Clear GL buffers
	glAssert(glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}

void DecryptFileScene::OnImGuiDraw()
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
	static ImFont* const& font_montserrat_medium_18 = m_fonts.at("Montserrat-Medium-18");
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
			static constexpr const auto title = "Decrypt File";
			static const ImVec2 title_size(ImGui::CalcTextSize(title).x * font_audiowide_regular_45->Scale, ImGui::CalcTextSize(title).y * font_audiowide_regular_45->Scale);
			ImGui::SetCursorPosX((io.DisplaySize.x - title_size.x) / 2.0f);
			ImGui::Text(title);
		}
		ImGui::PopStyleColor(1);
		ImGui::PopFont();

		spacing(3);
		ImGui::Separator();
		spacing(3);

		// Algorithm used for encryption
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
				if (ImGui::RadioButton(algo_name.c_str(), m_type == algo_type))
				{
					m_type = algo_type;
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

		// In File to Decrypt
		ImGui::PushFont(font_montserrat_medium_20);
		{
			// Label
			ImGui::Text("File To Decrypt:");
			// Encrypted text
			static const ImVec2 browse_button_size(45.0f, 25.0f);
			ImGuiWidgets::InputText("##text1", &m_in_filename, win_w - (browse_button_size.x * 1.5f));
			ImGui::PushFont(font_montserrat_medium_12);
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Button, Constants::Colors::BUTTON_COLOR); // buttons color idle
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Constants::Colors::BUTTON_COLOR_HOVER);  // buttons color hover
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, Constants::Colors::BUTTON_COLOR_ACTIVE); // buttons color pressed
			if (ImGui::Button("Browse", browse_button_size))
			{
				this->OnBrowseInFileButtonPressed();
			}
			ImGui::PopStyleColor(3);
			ImGui::PopFont();

			spacing(1);

			// Decompression (if used in encryption)
			ImGui::PushFont(font_montserrat_medium_18);
			{
				// Label
				ImGui::Text("Decompress file with gzip after decrypting:");
				ImGui::SameLine();
				ImGui::Checkbox(" ", &m_decompress);
			}
			ImGui::PopFont();

		}
		ImGui::PopFont();

		spacing(2);

		// Out File Decrypted
		if (!m_in_filename.empty())
		{
			ImGui::PushFont(font_montserrat_medium_20);
			{
				// Label
				ImGui::Text("Decrypted File Location:");
				// Encrypted text
				static const ImVec2 browse_button_size(45.0f, 25.0f);
				ImGuiWidgets::InputText("##text2", &m_out_filename, win_w - (browse_button_size.x * 1.5f));
				ImGui::PushFont(font_montserrat_medium_12);
				ImGui::SameLine();
				ImGui::PushStyleColor(ImGuiCol_Button, Constants::Colors::BUTTON_COLOR); // buttons color idle
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Constants::Colors::BUTTON_COLOR_HOVER);  // buttons color hover
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, Constants::Colors::BUTTON_COLOR_ACTIVE); // buttons color pressed
				if (ImGui::Button("Browse ", browse_button_size))
				{
					this->OnBrowseOutFileButtonPressed();
				}
				ImGui::PopStyleColor(3);
				ImGui::PopFont();
			}
			ImGui::PopFont();
		}


		spacing(3);
		ImGui::Separator();
		spacing(3);

		// Password used for encryption
		ImGui::PushFont(font_montserrat_medium_20);
		{
			// Label
			ImGui::Text("Password:");

			// Input text
			ImGuiWidgets::InputText("##text3", &m_password, static_cast<f32>(win_w), ImGuiInputTextFlags_::ImGuiInputTextFlags_Password);

			// Bytes count
			ImGui::PushFont(font_montserrat_medium_12);
			ImGui::Text("%zu bytes", m_password.size());
			ImGui::PopFont();
		}
		ImGui::PopFont();

		spacing(3);
		ImGui::Separator();
		spacing(3);

		// Decrypt & Back Button 
		{
			ImGui::PushFont(font_audiowide_regular_20); // buttons font
			ImGui::PushStyleColor(ImGuiCol_Button, Constants::Colors::BUTTON_COLOR); // buttons color idle
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Constants::Colors::BUTTON_COLOR_HOVER);  // buttons color hover
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, Constants::Colors::BUTTON_COLOR_ACTIVE); // buttons color pressed
			{
				ImGui::SetCursorPosX((io.DisplaySize.x - button_size.x * 2) / 2.0f);
				ImGui::SetCursorPosY((io.DisplaySize.y - button_size.y) - 10.0f);
				if (ImGuiWidgets::Button("Back", button_size, Constants::Colors::BACK_BUTTON_COLOR, Constants::Colors::BACK_BUTTON_COLOR_HOVER, Constants::Colors::BACK_BUTTON_COLOR_ACTIVE))
				{
					this->OnBackButtonPressed();
				}


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

void DecryptFileScene::OnEvent(Event& event)
{}

void DecryptFileScene::OnDestroy()
{
	ENIGMA_TRACE(ENIGMA_CURRENT_FUNCTION);

}

void DecryptFileScene::OnAutoDetectAlgorithmButtonPressed()
{
	if (m_in_filename.empty())
	{
		(void)DialogUtils::Warn("Cannot auto-detect algorithm without file to decrypt");
		return;
	}
	if (!fs::exists(m_in_filename))
	{
		(void)DialogUtils::Warn(m_in_filename + " does not exist");
		return;
	}
	if (!fs::is_regular_file(m_in_filename))
	{
		(void)DialogUtils::Warn(m_in_filename + " is not a regular file");
		return;
	}
	if (fs::is_empty(m_in_filename))
	{
		(void)DialogUtils::Warn(m_in_filename + " is empty");
		return;
	}
	

	try
	{
		// Auto detect algorithm used for encrypting
		byte cipher_first_byte{ (byte)Algorithm::Type::Last + 1 };
		ENIGMA_TRACE("auto-detecting algorithm used for encryption...");
		// extract first byte from infile cipher which must be the mode type used in encryption
		if (std::ifstream ifs{ m_in_filename, std::ios_base::binary | std::ios_base::in })
		{
			ifs >> cipher_first_byte;
			ifs.close();
		}
		
		// Check if detected mode is valid
		if (!ENIGMA_IS_BETWEEN(cipher_first_byte, (byte)Algorithm::Type::First, (byte)Algorithm::Type::Last))
		{
			(void)DialogUtils::Error("Could not auto-detect algorithm mode used for encryption");
			return;
		}
		// if alles gut, set type
		m_type = static_cast<Algorithm::Type>(cipher_first_byte);
		// little happy info dialog
		ENIGMA_INFO("Successfully detected algorithm used for encryption which is: {0}", Algorithm::AlgoTypeEnumToStr(m_type));
		(void)DialogUtils::Info("Successfully detected algorithm used for encryption which is: " + Algorithm::AlgoTypeEnumToStr(m_type));

	}
	catch (const CryptoPP::Exception& e)
	{
		const String err_msg = CryptoPPUtils::GetFullErrorMessage(e);
		ENIGMA_ERROR("Auto-detect algorithm Failure: {0}", err_msg);
		(void)DialogUtils::Error("Decryption Failure", err_msg);
	}
	catch (const std::exception& e)
	{
		ENIGMA_ERROR("Auto-detect algorithm Failure: {0}", e.what());
		(void)DialogUtils::Error("Auto-detect algorithm Failure", e.what());
	}
	catch (...)
	{
		const String err_msg = "Auto-detect algorithm Failure: UNKNOWN ERROR";
		ENIGMA_ERROR(err_msg);
		(void)DialogUtils::Error(err_msg);
	}
}


void DecryptFileScene::OnBrowseInFileButtonPressed()
{
	const auto ofd = std::make_unique<Enigma::OpenFileDialog>(
		"Select A File To Decrypt",
		m_in_filename, // initial path
		false // disable multi-select
		);

	ENIGMA_TRACE("Selecting a file to decrypt...");
	const std::vector<String> selected_file_paths = ofd->Show();
	if (selected_file_paths.empty())
	{
		ENIGMA_TRACE("Nothing is selected.");
		return;
	}
	m_in_filename = *selected_file_paths.begin();

}

void DecryptFileScene::OnBrowseOutFileButtonPressed()
{
	const auto ofd = std::make_unique<Enigma::SelectFolderDialog>(
		"Select A Location To Save Decrypted File To",
		m_in_filename, // initial path
		false // disable multi-select
		);

	ENIGMA_TRACE("Selecting a location to save decrypted file to...");
	const String selected_location = ofd->Show();
	if (selected_location.empty())
	{
		ENIGMA_TRACE("Nothing is selected.");
		return;
	}

	if (fs::path(m_in_filename).extension().string() == ".enigma")
		m_out_filename = selected_location + '/' + fs::path(m_in_filename).replace_extension("").filename().string();
	else
		m_out_filename = selected_location;

}

void DecryptFileScene::OnDecryptButtonPressed()
{
	// Validate fields 
	if (m_in_filename.empty())
	{
		(void)DialogUtils::Warn("In File to decrypt is empty");
	}
	else if (m_out_filename.empty())
	{
		(void)DialogUtils::Warn("Out File location is empty");
	}
	else if (!fs::exists(m_in_filename) || !fs::is_regular_file(m_in_filename))
	{
		(void)DialogUtils::Warn("In File does not exist");
	}
	else if (m_password.empty())
	{
		(void)DialogUtils::Warn("Encryption Password is empty");
	}
	else // Alles gut
	{
		try
		{
			// Create decryptor based on selected algorithm type
			const auto algorithm = Algorithm::CreateFromType(m_type, Algorithm::Intent::Decrypt);
			ENIGMA_ASSERT_OR_THROW(algorithm, "Failed to create algorithm from type");

			// Read in file cipher
			String cipher{};
			const bool file_read_success = FileUtils::Read(m_in_filename, cipher);
			ENIGMA_ASSERT_OR_THROW(file_read_success, "Failed to read cipher from file " + m_in_filename);
			ENIGMA_ASSERT_OR_THROW(!cipher.empty(), "File " + m_in_filename + " is empty");

			// Decrypt file cipher
			String buffer = algorithm->Decrypt(m_password, cipher);
			ENIGMA_ASSERT_OR_THROW(!buffer.empty(), "Failed to decrypt file cipher");

			// Decompression (if used in encryption)
			size_t old_buffer_size{ 0 }, new_buffer_size{ 0 }, increased_bytes{ 0 };
			if (m_decompress)
			{
				ENIGMA_TRACE("Decompressing file buffer {0} ...", m_in_filename);
				old_buffer_size = buffer.size();
				buffer = GZip::Decompress(buffer);
				new_buffer_size = buffer.size();
				increased_bytes = old_buffer_size < new_buffer_size ? (new_buffer_size - old_buffer_size) : 0;
				ENIGMA_TRACE("File size increased by {0:0.3f} MB", ENIGMA_BYTES_TO_MB(increased_bytes));
			}

			// Save buffer to out file decrypted
			ENIGMA_ASSERT_OR_THROW(!m_out_filename.empty(), "Invalid output file name");
			// Write buffer to out file
			const bool file_written_success = FileUtils::Write(m_out_filename, buffer);
			ENIGMA_ASSERT_OR_THROW(file_written_success, "Failed to write buffer to file " + m_out_filename);

			// Alert user that encryption was successfull
			(void)DialogUtils::Info("Decrypted " + m_in_filename + " => " + m_out_filename + " Successfully!\n" +
				(m_decompress ? (increased_bytes ? ("Decompression Status: File size increased by " +
					std::to_string(ENIGMA_BYTES_TO_MB(increased_bytes)) + " MB") : "") : ""));

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
			const String err_msg = "Decryption Failure: UNKNOWN ERROR";
			ENIGMA_ERROR(err_msg);
			(void)DialogUtils::Error(err_msg);
		}
	}

}

void DecryptFileScene::OnBackButtonPressed()
{
	if (!m_in_filename.empty() || !m_out_filename.empty() || !m_password.empty())
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


NS_ENIGMA_END
