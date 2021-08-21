#include "pch.hpp"
#include "DecryptFileScene.hpp"
#include <GUI/ImGuiWidgets.hpp>
#include <Utility/DialogUtils.hpp>
#include <System/Dialogs/MessageBox.hpp>
#include <System/Dialogs/OpenFileDialog.hpp>
#include <System/Dialogs/SaveFileDialog.hpp>
#include <System/Dialogs/SelectFolderDialog.hpp>
//#include <Scenes/RSAScene.hpp>

NS_ENIGMA_BEGIN

DecryptFileScene::DecryptFileScene()
	:
	Enigma::Scene(),
	m_type(Algorithm::Type::AES)
{
}

void DecryptFileScene::OnCreate()
{
	ENIGMA_TRACE_CURRENT_FUNCTION();

	// Explicit OpenGL old method to et background clear color
	//glAssert(glClearColor(
	//	Constants::Colors::BACKGROUND_COLOR.x,
	//	Constants::Colors::BACKGROUND_COLOR.y,
	//	Constants::Colors::BACKGROUND_COLOR.z,
	//	Constants::Colors::BACKGROUND_COLOR.w
	//));

	// Set default m_in_filename as enigma exe path
	//m_in_filename = fs::current_path().string();

}

void DecryptFileScene::OnUpdate(const f32&)
{}

void DecryptFileScene::OnDraw()
{
	// Clear GL buffers
	//glAssert(glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}

void DecryptFileScene::OnImGuiDraw()
{
	const auto& [win_w, win_h] = Application::GetInstance()->GetWindow()->GetSize();
	const auto& [win_x, win_y] = Application::GetInstance()->GetWindow()->GetPosition();
	static const auto& io = ImGui::GetIO();

	const auto button_size = Vec2f(win_w / 2.5f, 40.0f);

	static constexpr const auto inline_dummy = [](const f32& x, const f32& y) noexcept {  ImGui::SameLine(); ImGui::Dummy(ImVec2(x, y)); };
	static constexpr const auto spacing = [](const ui8& n) noexcept { for (ui8 i = 0; i < n; i++) ImGui::Spacing(); };

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
#pragma region Back button [<] & Scene Title
		static const auto& title_font = font_audiowide_regular_30;
		static const String title = ("Decrypt File");
		static const ImVec2 title_size((ImGui::CalcTextSize(title.c_str()).x * title_font->Scale) - 45.0f, ImGui::CalcTextSize(title.c_str()).y * title_font->Scale);
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
				(void)ImGui::ButtonEx(title.c_str(), ImVec2(static_cast<f32>(win_w), title_size.y), ImGuiButtonFlags_Disabled);
			}
			ImGui::PopStyleColor(2);
			ImGui::PopFont();
		}
#pragma endregion

		spacing(3);
		ImGui::Separator();
		spacing(3);

		// Algorithm used for encryption
		ImGui::PushFont(font_audiowide_regular_20);
		{
			// Label
			ImGui::Text("%s:", ("Algorithm"));
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
		}
		ImGui::PopFont();



#if 0
		spacing(2);

		// Decompression widget
		ImGui::PushFont(font_montserrat_medium_16);
		{
			ImGui::PushFont(font_audiowide_regular_20);
			ImGui::Text("Decompress file with gzip after decrypting:");
			ImGui::PopFont();
			inline_dummy(6.0f, 0.0f);
			ImGui::SameLine();
			ImGui::Checkbox("##decompress_checkbox", &m_decompress);
		}
		ImGui::PopFont();
#endif

		spacing(3);
		ImGui::Separator();
		spacing(3);

		// In File to Decrypt
		ImGui::PushFont(font_montserrat_medium_20);
		{
			// Label
			ImGui::Text("%s:", ("File To Decrypt"));
			// Encrypted text
			static const ImVec2 browse_button_size(45.0f, 25.0f);
			ImGuiWidgets::InputText("##text1", &m_in_filename, win_w - (browse_button_size.x * 1.5f));
			ImGui::PushFont(font_montserrat_medium_12);
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Button, Constants::Colors::BUTTON_COLOR); // buttons color idle
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Constants::Colors::BUTTON_COLOR_HOVER);  // buttons color hover
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, Constants::Colors::BUTTON_COLOR_ACTIVE); // buttons color pressed
			ImGui::PushID("Browse1");
				if (ImGui::Button("Browse", browse_button_size))
				{
					Application::GetInstance()->LaunchWorkerThread(this, "Browsing input file...", [this]() -> void
					{
						this->OnBrowseInFileButtonPressed();
					});
				}
			ImGui::PopID();
			ImGui::PopStyleColor(3);
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
				ImGui::Text("%s:", ("Decrypted File Location"));
				// Encrypted text
				static const ImVec2 browse_button_size(45.0f, 25.0f);
				ImGuiWidgets::InputText("##text2", &m_out_filename, win_w - (browse_button_size.x * 1.5f));
				ImGui::PushFont(font_montserrat_medium_12);
				ImGui::SameLine();
				ImGui::PushStyleColor(ImGuiCol_Button, Constants::Colors::BUTTON_COLOR); // buttons color idle
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Constants::Colors::BUTTON_COLOR_HOVER);  // buttons color hover
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, Constants::Colors::BUTTON_COLOR_ACTIVE); // buttons color pressed
				ImGui::PushID("Browse2");
					if (ImGui::Button("Browse", browse_button_size))
					{
						Application::GetInstance()->LaunchWorkerThread(this, "Browsing output file location...", [this]() -> void
						{
							this->OnBrowseOutFileLocationButtonPressed();
						});
					}
				ImGui::PopID();
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
			ImGui::Text("%s:", ("Password"));

			// Input text
			ImGuiWidgets::InputText("##text3", &m_password, static_cast<f32>(win_w), ImGuiInputTextFlags_::ImGuiInputTextFlags_Password);

			// Bytes count
			ImGui::PushFont(font_montserrat_medium_12);
			//ImGui::Text("%zu bytes", m_password.size());
			ImGui::Text("%s", SizeUtils::FriendlySize( m_password.size() ).c_str());
			ImGui::PopFont();
		}
		ImGui::PopFont();

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
				if (ImGui::Button(("Decrypt"), button_size))
				{
					Application::GetInstance()->LaunchWorkerThread(this, ("Decrypting file..."), [this]() -> void
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

void DecryptFileScene::OnEvent(Event& event)
{
	// Listen to Window File Drop Event for input filename
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<WindowFileDropEvent>([this](const WindowFileDropEvent& e) -> bool
		{
			// set dropped file into in filename
			if (const std::vector<fs::path>& filenames = e.GetFilenames();
				!filenames.empty() &&
				fs::exists(filenames[0]) &&
				fs::is_regular_file(filenames[0]))
			{
				this->m_in_filename = filenames[0].string();
			}

			return true; // handled, return false if other scenes may use the event
		});
}

void DecryptFileScene::OnDestroy()
{
	ENIGMA_TRACE_CURRENT_FUNCTION();

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

void DecryptFileScene::OnBrowseOutFileLocationButtonPressed()
{
	const auto ofd = std::make_unique<Enigma::SelectFolderDialog>(
		("Select A Location To Save Decrypted File To"),
		fs::path(m_in_filename).has_parent_path() ? fs::path(m_in_filename).parent_path().string() : m_in_filename, // in file location as an initial path
		false // disable multi-select
		);

	ENIGMA_TRACE("Selecting a location to save decrypted file to...");
	const String selected_location = ofd->Show();
	if (selected_location.empty())
	{
		ENIGMA_TRACE("Nothing is selected.");
		return;
	}

	// check if file ends with .enigma extension, if so, remove that extention to recover default file's one.
	if (StringUtils::LowerCopy(fs::path(m_in_filename).extension().string()) == ".enigma")
	{ 
		ENIGMA_TRACE("in file ends with .enigma ext. removing it...");
		// file ends with .enigma ext, remove it.
		m_out_filename = (fs::path(selected_location) / fs::path(m_in_filename).replace_extension("").filename()).string();
	}
	else
	{
		// file doesnt end with .enigma ext, assume it is its original ext.
		m_out_filename = (fs::path(selected_location) / fs::path(m_in_filename)).string();
	}

#if 0
	if (fs::path(m_in_filename).extension().string() == ".enigma")
		m_out_filename = selected_location + '/' + fs::path(m_in_filename).replace_extension("").filename().string();
	else
		m_out_filename = selected_location;
#endif

}

void DecryptFileScene::OnDecryptButtonPressed()
{
	// Validate fields 
	if (m_in_filename.empty())
	{
		(void)DialogUtils::Warn(("Input File to decrypt is empty"));
	}
	else if (m_out_filename.empty())
	{
		(void)DialogUtils::Warn(("Out File location is empty"));
	}
	else if (!fs::exists(m_in_filename) || !fs::is_regular_file(m_in_filename))
	{
		(void)DialogUtils::Warn(("Input File does not exist"));
	}
	else if (m_password.empty())
	{
		(void)DialogUtils::Warn(("Encryption password is empty"));
	}
	else // Alles gut
	{
		try
		{
			// Auto detect encryption algorithm
			m_type = Algorithm::DetectFromFile(m_in_filename);
			ENIGMA_INFO("Successfully auto-detected algorithm used for encryption which is {0}", Algorithm::AlgoTypeEnumToStr(m_type));

			// Create decryptor based on selected algorithm type
			const auto algorithm = Algorithm::CreateFromType(m_type, Algorithm::Intent::Decrypt);
			ENIGMA_ASSERT_OR_THROW(algorithm, ("Failed to create algorithm from type"));

			// Read in file cipher
			String cipher{};
			const bool file_read_success = FileUtils::Read(m_in_filename, cipher);
			ENIGMA_ASSERT_OR_THROW(file_read_success, ("Failed to read cipher from file {}", m_in_filename));
			ENIGMA_ASSERT_OR_THROW(!cipher.empty(), ("File {} is empty", m_in_filename));

			// Decrypt file cipher
			String buffer = algorithm->Decrypt(m_password, cipher);
			ENIGMA_ASSERT_OR_THROW(!buffer.empty(), ("Failed to decrypt file cipher"));

			/*
			Note: You should compress before encrypting. Encryption turns your data into high-entropy data,
				 usually indistinguishable from a random stream. Compression relies on patterns in order to gain
				 any size reduction. Since encryption destroys such patterns, the compression algorithm would be
				 unable to give you much (if any) reduction in size if you apply it to encrypted data.
			*/

			// Decompression 
			ENIGMA_TRACE("Decompressing file buffer {0} ...", m_in_filename);
			auto old_buffer_size = buffer.size();
			buffer = GZip::Decompress(buffer);
			auto new_buffer_size = buffer.size();
			auto increased_bytes = old_buffer_size < new_buffer_size ? (new_buffer_size - old_buffer_size) : 0;
			ENIGMA_TRACE("File size increased by {0}", SizeUtils::FriendlySize(increased_bytes));
			

#if 0
			// Decompression (if used in encryption)
			size_t increased_bytes{ 0 };
			if (m_decompress)
			{
				ENIGMA_TRACE("Decompressing file buffer {0} ...", m_in_filename);
				auto old_buffer_size = buffer.size();
				buffer = GZip::Decompress(buffer);
				auto new_buffer_size = buffer.size();
				increased_bytes = old_buffer_size < new_buffer_size ? (new_buffer_size - old_buffer_size) : 0;
				//ENIGMA_TRACE("File size increased by {0:0.3f} MB", ENIGMA_BYTES_TO_MB(increased_bytes));
				ENIGMA_TRACE("File size increased by {0}", SizeUtils::FriendlySize(increased_bytes));
			}
#endif

			// Save buffer to out file decrypted
			ENIGMA_ASSERT_OR_THROW(!m_out_filename.empty(), ("Invalid output file name"));
			// Write buffer to out file
			const bool file_written_success = FileUtils::Write(m_out_filename, buffer);
			ENIGMA_ASSERT_OR_THROW(file_written_success, ("Failed to write buffer to file {}", m_out_filename));

			// Alert user that decryption was successfull
			const String msg = ("Decrypted {} to {} successfully, decompression increased file size by {}",
				fs::path(m_in_filename).filename(),
				fs::path(m_out_filename).filename(),
				SizeUtils::FriendlySize(increased_bytes));
			ENIGMA_INFO(msg);
			(void)DialogUtils::Info(msg);
		}
		catch (const CryptoPP::Exception& e)
		{
			const String err_msg = CryptoPPUtils::GetFullErrorMessage(e);
			ENIGMA_ERROR("Decryption Failure: {0}", err_msg);
			(void)DialogUtils::Error(("Decryption Failure"), err_msg);
		}
		catch (const std::exception& e)
		{
			ENIGMA_ERROR("Decryption Failure: {0}", e.what());
			(void)DialogUtils::Error(("Decryption Failure"), e.what());
		}
		catch (...)
		{
			const String err_msg = ("Decryption Failure UNKNOWN ERROR");
			ENIGMA_ERROR(err_msg);
			(void)DialogUtils::Error(err_msg);
		}
	}

}

void DecryptFileScene::OnBackButtonPressed()
{
	//if (!m_in_filename.empty() || !m_out_filename.empty() || !m_password.empty())
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


NS_ENIGMA_END
