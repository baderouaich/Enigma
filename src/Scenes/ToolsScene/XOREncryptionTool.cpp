#include "pch.hpp"
#include "XOREncryptionTool.hpp"
#include <Application/Application.hpp>
#include <GUI/ImGuiWidgets.hpp>
#include <Utility/DialogUtils.hpp>
#include <System/Clipboard/Clipboard.hpp>
#include <Utility/Base64.hpp>

NS_ENIGMA_BEGIN
void XOREncryptionTool::OnCreate()
{
	m_intent = Algorithm::Intent::Encrypt;
}

void XOREncryptionTool::OnDraw(Scene* parent)
{
	static const auto& io = ImGui::GetIO();

	const auto& [win_w, win_h] = Application::getInstance()->GetWindow()->GetSize();
	//const auto& [win_x, win_y] = Application::getInstance()->GetWindow()->GetPosition();

	//const auto button_size = ImVec2(win_w / 2.6f, 40.0f);

	static constexpr const auto spacing = [](const std::uint8_t& n) noexcept { for (std::uint8_t i = 0; i < n; i++) ImGui::Spacing(); };

	static auto& fonts = Application::getInstance()->GetFonts();
	//static ImFont* const& font_audiowide_regular_45 = fonts.at("Audiowide-Regular-45");
	//static ImFont* const& font_audiowide_regular_30 = fonts.at("Audiowide-Regular-30");
	static ImFont* const& font_audiowide_regular_20 = fonts.at("Audiowide-Regular-20");
	static ImFont* const& font_montserrat_medium_20 = fonts.at("Montserrat-Medium-20");
	static ImFont* const& font_montserrat_medium_18 = fonts.at("Montserrat-Medium-18");
	static ImFont* const& font_montserrat_medium_14 = fonts.at("Montserrat-Medium-14");
	static ImFont* const& font_montserrat_medium_12 = fonts.at("Montserrat-Medium-12");

	ImGui::PushFont(font_audiowide_regular_20);
	{
		if (ImGui::CollapsingHeader(("XOR Encrypt/Decrypt")))
		{
			ImGui::PushFont(font_montserrat_medium_18);
			spacing(3);


			// Opearation Radio buttons
			ImGui::Text("%s:", ("Operation"));
			if (ImGui::RadioButton(("Encrypt"), m_intent == Algorithm::Intent::Encrypt))
			{
				m_intent = Algorithm::Intent::Encrypt;
			}
			ImGui::SameLine();
			if (ImGui::RadioButton(("Decrypt"), m_intent == Algorithm::Intent::Decrypt))
			{
				m_intent = Algorithm::Intent::Decrypt;
			}

			spacing(3);

			// Buffer or cipher to encrypt/decrypt
			ImGui::PushFont(font_montserrat_medium_20);
			{
				// Label
				ImGui::Text("%s:", ("Input"));

				// Input text/cipher
				const ImVec2 input_text_size(static_cast<float>(win_w), ImGui::GetTextLineHeightWithSpacing() * 3.0f);
				ImGuiWidgets::InputTextMultiline("##text1", &m_input, input_text_size);

				// Bytes count
				ImGui::PushFont(font_montserrat_medium_12);
				ImGui::Text("%s", SizeUtils::FriendlySize(m_input.size()).c_str());
				ImGui::PopFont();
			}
			ImGui::PopFont();

			spacing(3);

			// Encryption Password 
			ImGui::PushFont(font_montserrat_medium_20);
			{			
				// Label
				ImGui::Text("%s:", ("Password"));
				// Input text
				ImGuiWidgets::InputText("##text2", &m_password, static_cast<float>(win_w), ImGuiInputTextFlags_::ImGuiInputTextFlags_Password);
			}
			ImGui::PopFont();

			spacing(3);

			// Encrypted/decrypted output
			if (!m_output.empty())
			{
				ImGui::PushFont(font_montserrat_medium_20);
				{
					// Label
					ImGui::Text("%s:", ("Output (in base64)"));
					// Encrypted/Decrypted text
					ImGuiWidgets::InputTextMultiline("##text4", &m_output, ImVec2(win_w * 0.88f, ImGui::GetTextLineHeightWithSpacing() * 3.0f));
					ImGui::PushFont(font_montserrat_medium_14);
					ImGui::SameLine();
					ImGui::PushStyleColor(ImGuiCol_Button, Constants::Colors::BUTTON_COLOR); // buttons color idle
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Constants::Colors::BUTTON_COLOR_HOVER);  // buttons color hover
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, Constants::Colors::BUTTON_COLOR_ACTIVE); // buttons color pressed
					if (ImGui::Button(("Copy"), ImVec2(win_w * 0.10f, 30.0f)))
					{
						Clipboard::Set(m_output);
					}
					//ImGui::NewLine();
					ImGui::PushFont(font_montserrat_medium_12);
					ImGui::Text("%s", SizeUtils::FriendlySize(m_output.size()).c_str());
					ImGui::PopFont();
					ImGui::PopStyleColor(3);
					ImGui::PopFont();
				}
				ImGui::PopFont();
			}

			spacing(3);

			// Encrypt/Decrypt Button
			{
				ImGui::PushFont(font_audiowide_regular_20); // buttons font
				ImGui::PushStyleColor(ImGuiCol_Button, Constants::Colors::BUTTON_COLOR); // buttons color idle
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Constants::Colors::BUTTON_COLOR_HOVER);  // buttons color hover
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, Constants::Colors::BUTTON_COLOR_ACTIVE); // buttons color pressed
				{
					const auto button_size = ImVec2(win_w / 2.9f, 33.0f);
					ImGui::SetCursorPosX((io.DisplaySize.x - button_size.x) / 2.0f);
					//ImGui::SetCursorPosY((io.DisplaySize.y - button_size.y) - 10.0f);
					const auto btn_txt = m_intent == Algorithm::Intent::Encrypt ? "Encrypt" : "Decrypt";
					ImGui::PushID(m_intent == Algorithm::Intent::Encrypt ? 33 : 66);
					if (ImGui::Button((btn_txt), button_size))
					{
						const auto loading_txt = m_intent == Algorithm::Intent::Encrypt ? "Encrypting text..." : "Decrypting text...";
						Application::getInstance()->LaunchWorkerThread(parent, (loading_txt), [this]() -> void
							{
								if(m_intent == Algorithm::Intent::Encrypt)
									this->OnEncryptButtonPressed();
								else
									this->OnDecryptButtonPressed();
							});
					}
					ImGui::PopID();

				}
				ImGui::PopStyleColor(3);
				ImGui::PopFont();

			}

			ImGui::PopFont();
		}
	}
	ImGui::PopFont();
}

void XOREncryptionTool::OnDestroy()
{
	m_password.clear();
	m_input.clear();
	m_output.clear();
}

void XOREncryptionTool::OnEncryptButtonPressed()
{
	ENIGMA_TRACE_CURRENT_FUNCTION();

	// validate fields
	if (m_password.empty())
	{
		(void)DialogUtils::Warn(("Encryption password is empty"));
		return;
	}
	if (m_input.empty())
	{
		(void)DialogUtils::Warn(("Input is empty"));
		return;
	}

	try
	{
		m_output = XOREncrypt(m_password, m_input);
		m_output = Base64::Encode(m_output);
	}
	catch (const std::exception& e)
	{
		ENIGMA_ERROR("Encryption Failure: {0}", e.what());
		(void)DialogUtils::Error(("Encryption Failure"), e.what());
	}
	catch (...)
	{
		const std::string err_msg = ("Encryption Failure: UNKNOWN ERROR");
		ENIGMA_ERROR(err_msg);
		(void)DialogUtils::Error(err_msg);
	}
}

void XOREncryptionTool::OnDecryptButtonPressed()
{
	ENIGMA_TRACE_CURRENT_FUNCTION();
	
	// validate fields
	if (m_password.empty())
	{
		(void)DialogUtils::Warn(("Encryption password is empty"));
		return;
	}
	if (m_input.empty())
	{
		(void)DialogUtils::Warn(("Input is empty"));
		return;
	}

	try
	{
		m_output = XORDecrypt(m_password, Base64::Decode(m_input));
	}
	catch (const std::exception& e)
	{
		ENIGMA_ERROR("Decryption Failure: {0}", e.what());
		(void)DialogUtils::Error(("Encryption Failure"), e.what());
	}
	catch (...)
	{
		const std::string err_msg = ("Decryption Failure: UNKNOWN ERROR");
		ENIGMA_ERROR(err_msg);
		(void)DialogUtils::Error(err_msg);
	}
}
 
NS_ENIGMA_END
