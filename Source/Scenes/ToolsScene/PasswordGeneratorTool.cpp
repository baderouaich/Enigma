#include <pch.hpp>
#include "PasswordGeneratorTool.hpp"
#include <GUI/ImGuiWidgets.hpp>
#include <System/Clipboard/Clipboard.hpp>
#include <Utility/Random.hpp>
#include <cstring>

NS_ENIGMA_BEGIN
void PasswordGeneratorTool::OnCreate() 
{
	ENIGMA_TRACE_CURRENT_FUNCTION();

	m_digits = m_uppercase_alphabets = m_lowercase_alphabets = m_special_characters = true;

#if 0
	// Make Available lengths
	for (i32 i = 2; i <= 4096; i *= 2)
	{
		char* s = new char[10];
		std::sprintf(s, "%d", i); // will add \000 at the end which is required for imgui combo box
		m_lengths.emplace_back(std::move(s));
	}
#endif
	// Make Available lengths
	m_lengths.reserve(4096 - 5);
	for (i32 i = 5; i <= 4096; ++i)
	{
		char* s = new char[10];
		std::sprintf(s, "%d", i); // will add \000 at the end which is required for imgui combo box
		m_lengths.emplace_back(std::move(s));
	}
	m_selected_length_index = 7; // 12
}

void PasswordGeneratorTool::OnDraw(Scene* parent)
{
	const auto& [win_w, win_h] = Application::GetInstance()->GetWindow()->GetSize();
	//const auto& [win_x, win_y] = Application::GetInstance()->GetWindow()->GetPosition();

	//const auto button_size = Vec2f(win_w / 2.6f, 40.0f);

	static constexpr const auto spacing = [](const ui8& n) noexcept { for (ui8 i = 0; i < n; i++) ImGui::Spacing(); };

	static auto& fonts = Application::GetInstance()->GetFonts();
	//static ImFont* const& font_audiowide_regular_45 = fonts.at("Audiowide-Regular-45");
	//static ImFont* const& font_audiowide_regular_30 = fonts.at("Audiowide-Regular-30");
	static ImFont* const& font_audiowide_regular_20 = fonts.at("Audiowide-Regular-20");
	static ImFont* const& font_montserrat_medium_20 = fonts.at("Montserrat-Medium-20");
	static ImFont* const& font_montserrat_medium_18 = fonts.at("Montserrat-Medium-18");
	static ImFont* const& font_montserrat_medium_14 = fonts.at("Montserrat-Medium-14");
	//static ImFont* const& font_montserrat_medium_12 = fonts.at("Montserrat-Medium-12");

	ImGui::PushFont(font_audiowide_regular_20);
	{
		if (ImGui::CollapsingHeader("Password Generator", nullptr, ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen)) 
		//if (ImGui::CollapsingHeader("Password Generator", nullptr)) 
		{
			ImGui::PushFont(font_montserrat_medium_18);
			spacing(3);
			// Copy button, Output Input Text , Generate Button
			// Copy Generated password button 10% of width
			//ImGui::PushFont(font_montserrat_medium_20);
			if (ImGuiWidgets::Button("Copy", Vec2f(win_w * 0.10f, 33.0f), Constants::Colors::BUTTON_COLOR, Constants::Colors::BUTTON_COLOR_HOVER, Constants::Colors::BUTTON_COLOR_ACTIVE))
			{
				this->OnCopyPasswordButtonPressed();
			}
			//
			ImGui::SameLine();

			// Output password input text 65% of width
			ImGuiWidgets::InputTextMultiline("##input_pw", &m_password, Vec2f(win_w * 0.70f, 33.0f));
			//

			ImGui::SameLine();

			// Generate button 25% of width
			if (ImGuiWidgets::Button("Generate", Vec2f(win_w * 0.20f, 33.0f), Constants::Colors::TOOLS_BUTTON_COLOR, Constants::Colors::TOOLS_BUTTON_COLOR_HOVER, Constants::Colors::TOOLS_BUTTON_COLOR_ACTIVE))
			{
				Application::GetInstance()->LaunchWorkerThread(parent, "Generating Password...", [this]()
				{
					this->OnGenerateButtonPressed();
				});

			}

			spacing(2);

			
			//ImGui::PopFont();

			// Password Settings
			// Wrap password settings in columns to order things up
			//ImGui::PushFont(font_montserrat_medium_14);
				ImGui::BeginColumns("Password Settings", 5,  ImGuiColumnsFlags_NoResize);
					// Length Combo Box
					/*
					ImGui::Combo("Length", &m_selected_length_index, m_lengths.data(), m_lengths.size(), -1);
					ImGui::NextColumn();
					ImGui::Checkbox("Digits (0-9)", &m_digits); 
					ImGui::NextColumn();
					ImGui::Checkbox("Uppercase Alphabets (A-Z)", &m_uppercase_alphabets);
					ImGui::NextColumn();
					ImGui::Checkbox("Lowercase Alphabets (a-z)", &m_lowercase_alphabets); 
					ImGui::NextColumn();
					ImGui::Checkbox("Special Characters (+*~.#^)", &m_special_characters);
					*/
					ImGui::Combo("Length", &m_selected_length_index, m_lengths.data(), static_cast<i32>(m_lengths.size()), -1);
					ImGui::NextColumn();
					ImGui::Checkbox("Numbers", &m_digits);
					ImGui::NextColumn();
					ImGui::Checkbox("Uppercase", &m_uppercase_alphabets);
					ImGui::NextColumn();
					ImGui::Checkbox("Lowercase", &m_lowercase_alphabets);
					ImGui::NextColumn();
					ImGui::Checkbox("Symbols", &m_special_characters); 
				ImGui::EndColumns();
			//ImGui::PopFont();
			//

			spacing(2);

			// Remember your password sentence (only when available)
			if (!m_remember_password_sentence.empty())
			{
				ImGui::BulletText("Remember Your Password:");
				if (ImGuiWidgets::Button("Copy ", Vec2f(win_w * 0.10f, 33.0f), Constants::Colors::BUTTON_COLOR, Constants::Colors::BUTTON_COLOR_HOVER, Constants::Colors::BUTTON_COLOR_ACTIVE))
				{
					this->OnCopyRememberPasswordSentenceButtonPressed();
				}
				ImGui::SameLine();
				ImGuiWidgets::InputTextMultiline("##output_rmb_pw", &m_remember_password_sentence, Vec2f(-1.0f, 33.0f));
				ImGui::NewLine();
			}
			//

			ImGui::PopFont();
		}
	}
	ImGui::PopFont();
}

void PasswordGeneratorTool::OnDestroy()
{
	ENIGMA_TRACE_CURRENT_FUNCTION();

	// Delete heap allocated combobox lengths item
	for (auto& length : m_lengths)
	{
		if (length)
		{
			delete[] length;
			length = nullptr;
		}
	}
	m_lengths.clear();
}


void PasswordGeneratorTool::OnGenerateButtonPressed()
{
	if (!m_digits && !m_lowercase_alphabets && !m_uppercase_alphabets && !m_special_characters)
		return;

	// get desired length
	const size_t length = std::stoull(m_lengths[m_selected_length_index]);
	const String special_characters = Constants::Algorithm::SPECIAL_CHARACTERS;

	Random::Reseed();
	m_password.clear();
	while (m_password.size() < length)
	{
		char c = '\000';
	again:
		const ui16 r = Random::Int<ui16>(1, 4);
		switch (r)
		{
		case 1: // m_digits
			if (m_digits)
			{
				c = static_cast<char>(Random::Int<int>('0', '9'));
			}
			else goto again;
			break;
		case 2: // m_uppercase_alphabets
			if (m_uppercase_alphabets)
			{
				c = static_cast<char>(Random::Int<int>('A', 'Z'));
			}
			else goto again;
			break;
		case 3: // m_lowercase_alphabets
			if (m_lowercase_alphabets)
			{
				c = static_cast<char>(Random::Int<int>('a', 'z'));
			}
			else goto again;
			break;
		case 4: // m_special_characters
			if (m_special_characters)
			{
				c = special_characters[Random::Int<size_t>(0, special_characters.size() - 1)];
			}
			else goto again;
			break;
		}
		m_password += c;
	}


	// Generate helpful sentence to remember password with
	m_remember_password_sentence.clear();
	for (const char c : m_password)
	{
		// if char its a digit or special character, place it directly
		if (m_remember_password_char_words.find(c) == m_remember_password_char_words.end())
		{
			m_remember_password_sentence += c;
		}
		else
		{
			m_remember_password_sentence += m_remember_password_char_words.at(c);
		}
		m_remember_password_sentence += ' ';
	}
}

void PasswordGeneratorTool::OnCopyPasswordButtonPressed()
{
	if(!m_password.empty())
		Clipboard::Set(m_password);
}

void PasswordGeneratorTool::OnCopyRememberPasswordSentenceButtonPressed()
{
	if(!m_remember_password_sentence.empty())
		Clipboard::Set(m_remember_password_sentence);
}

NS_ENIGMA_END
