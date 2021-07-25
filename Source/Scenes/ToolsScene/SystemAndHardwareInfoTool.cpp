#include <pch.hpp>
#include "SystemAndHardwareInfoTool.hpp"
#include <Utility/SizeUtils.hpp>

NS_ENIGMA_BEGIN

void SystemAndHardwareInfoTool::OnCreate()
{
	ENIGMA_TRACE_CURRENT_FUNCTION();

	m_cpu_info = std::make_unique<CPUInformation>();
}

void SystemAndHardwareInfoTool::OnDraw(Scene*)
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
		if (ImGui::CollapsingHeader("System & Hardware Information", nullptr, ImGuiTreeNodeFlags_Framed))
		{
			ImGui::PushFont(font_montserrat_medium_18);
			spacing(3);

			/// CPU Information ///
			if (m_cpu_info)
			{
				if (ImGui::TreeNode("CPU"))
				{
					// Architecture, Frequency, Model name, Vendor ID
					ImGui::BulletText("Architecture: %s", m_cpu_info->architecture.c_str());
					ImGui::BulletText("Frequency: %lu Hz", m_cpu_info->frequency);
					ImGui::BulletText("Endianness: %s", m_cpu_info->endianness.c_str());
					ImGui::BulletText("Model name: %s", m_cpu_info->model_name.c_str());
					ImGui::BulletText("Vendor ID: %s", m_cpu_info->vendor_id.c_str());

					spacing(2);

					// Quantities
					if (ImGui::TreeNode("Quantities"))
					{
						ImGui::BulletText("Logical CPUs: %u",   m_cpu_info->quantities.logical);
						ImGui::BulletText("Physical CPUs: %u",  m_cpu_info->quantities.physical);
						ImGui::BulletText("CPU packages: %u",   m_cpu_info->quantities.packages);

						ImGui::TreePop();
					}

					spacing(2);

					// Caches
					if (ImGui::TreeNode("Caches"))
					{
						for (size_t i = 0; i < m_cpu_info->caches.size(); ++i)
						{
							if (ImGui::TreeNode(std::to_string(i + 1).c_str()))
							{
								ImGui::BulletText("Size: %s", SizeUtils::FriendlySize(m_cpu_info->caches[i].size).c_str());
								ImGui::BulletText("Line Size: %s", SizeUtils::FriendlySize(m_cpu_info->caches[i].line_size).c_str());
								ImGui::BulletText("Associativity: %d", static_cast<i32>(m_cpu_info->caches[i].associativity));
								ImGui::BulletText("Type: %s", m_cpu_info->GetCacheTypeStr(m_cpu_info->caches[i].type).c_str());

								ImGui::TreePop();
							}
						}

						ImGui::TreePop();
					}
					

					spacing(2);

					// Supported Instruction Set
					if (ImGui::TreeNode("Supported Instruction Set"))
					{
						for (const auto& is : m_cpu_info->supported_instructions_set)
						{
							ImGui::BulletText("%s", is.data());
						}
						ImGui::TreePop();
					}


					ImGui::TreePop();
				}
			}
			///

			ImGui::PopFont();
		}
	}
	ImGui::PopFont();
}

void SystemAndHardwareInfoTool::OnDestroy()
{
	ENIGMA_TRACE_CURRENT_FUNCTION();

}
NS_ENIGMA_END
