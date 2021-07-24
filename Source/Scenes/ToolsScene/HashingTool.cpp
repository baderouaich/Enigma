#include <pch.hpp>
#include "HashingTool.hpp"

NS_ENIGMA_BEGIN
void HashingTool::OnCreate()
{
	ENIGMA_TRACE_CURRENT_FUNCTION();

	// dm Register Available Hashing Algorithms
	//m_hash_algorithms[HashAlgorithm::MD5] = std::make_unique<CryptoPP::MD5>();
	m_selected_hash = HashAlgorithm::MD5; // MD5 selected by default
}

void HashingTool::OnDraw(Scene* parent)
{
	const auto& [win_w, win_h] = Application::GetInstance()->GetWindow()->GetSize();

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
		
		if (ImGui::CollapsingHeader("Hashing", nullptr, ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::PushFont(font_montserrat_medium_18);
			spacing(3);

			ImGui::BulletText("Algorithms: ");
			for (i32 i = static_cast<i32>(HashAlgorithm::BEGIN); i <= static_cast<i32>(HashAlgorithm::END); ++i)
			{
				if (ImGui::RadioButton(*static_cast<HashAlgorithm>(i), m_selected_hash == static_cast<HashAlgorithm>(i)))
				{
					m_selected_hash = static_cast<HashAlgorithm>(i);
				}
				ImGui::SameLine();
			}

			// TODO: add another 2 radio () Text () File
			// if text, show input to enter text with right button [hash]
			// if file, show input text with [browse] left button, and hash right button
			// see how u can do to effect only the browse button on radio state and check if(hash_text) else readfile...

			ImGui::PopFont();
		}
	}
	ImGui::PopFont();
}

void HashingTool::OnDestroy()
{
	ENIGMA_TRACE_CURRENT_FUNCTION();
}

NS_ENIGMA_END
