#include <pch.hpp>
#include "MyEncryptionsScene.hpp"

#include <imgui.h>

NS_ENIGMA_BEGIN

MyEncryptionsScene::MyEncryptionsScene(const std::unordered_map<std::string_view, ImFont*>& fonts)
	:
	Enigma::Scene(),
	m_fonts(fonts)
{
}

void MyEncryptionsScene::OnCreate()
{
	ENIGMA_TRACE(ENIGMA_CURRENT_FUNCTION);

	// Set background clear color
	glAssert(glClearColor(
		Constants::Colors::BACKGROUND_COLOR.x,
		Constants::Colors::BACKGROUND_COLOR.y,
		Constants::Colors::BACKGROUND_COLOR.z,
		Constants::Colors::BACKGROUND_COLOR.w
	));

	// Get all Encryptions from database
	this->GetAllEncryptions();
}

void MyEncryptionsScene::OnUpdate(const f32& dt)
{}

void MyEncryptionsScene::OnDraw()
{
	// Clear GL buffers
	glAssert(glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}

void MyEncryptionsScene::OnImGuiDraw()
{
	const auto& [win_w, win_h] = Application::GetInstance()->GetWindow()->GetSize();
	const auto& [win_x, win_y] = Application::GetInstance()->GetWindow()->GetPosition();
	static const auto& io = ImGui::GetIO();

	const auto button_size = Vec2f(win_w / 2.6f, 40.0f);

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
			static constexpr const auto title = "My Encryptions";
			static const ImVec2 title_size(ImGui::CalcTextSize(title).x * font_audiowide_regular_45->Scale, ImGui::CalcTextSize(title).y * font_audiowide_regular_45->Scale);
			ImGui::SetCursorPosX((io.DisplaySize.x - title_size.x) / 2.0f);
			ImGui::Text(title);
		}
		ImGui::PopStyleColor(1);
		ImGui::PopFont();

		spacing(2);
		ImGui::Separator();
		spacing(2);

		// Order By 
		ImGui::PushFont(font_montserrat_medium_18); // text font
		ImGui::Text("Order By "); ImGui::SameLine();
		if (ImGui::RadioButton("ID", m_order_by == OrderBy::ID))
		{
			m_order_by = OrderBy::ID;
			this->GetAllEncryptions();
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("Title", m_order_by == OrderBy::Title))
		{
			m_order_by = OrderBy::Title;
			this->GetAllEncryptions();
		}	
		ImGui::SameLine();
		if (ImGui::RadioButton("Date Time", m_order_by == OrderBy::DateTime))
		{
			m_order_by = OrderBy::DateTime;
			this->GetAllEncryptions();
		}		
		// Order ASC DESC
		ImGui::Text("Order "); ImGui::SameLine();
		if (ImGui::RadioButton("Ascending", m_order == Order::Ascending))
		{
			m_order = Order::Ascending;
			this->GetAllEncryptions();
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("Descending", m_order == Order::Descending))
		{
			m_order = Order::Descending;
			this->GetAllEncryptions();
		}
		ImGui::PopFont();

		spacing(2);
		ImGui::Separator();
		spacing(2);

		// Encryptions records Table 
		// https://github.com/ocornut/imgui/blob/master/imgui_demo.cpp#L5024
		ImGui::PushFont(font_montserrat_medium_18); // text font
		static constexpr const auto table_flags = 
			ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable |
			ImGuiTableFlags_Hideable | ImGuiTableFlags_Borders | 
			ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoSavedSettings |
		    ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY;
		static constexpr const auto NUM_COLUMNS = 4 + 1; // id, title... + 1 field has 2 buttons Operation(View|Delete)
		if (ImGui::BeginTable("Encryptions", NUM_COLUMNS, table_flags, ImVec2(win_w - 20.0f, win_h / 1.60f)))
		{
			// Columns header
			static constexpr const auto header_columns_flags = ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoHide;
			ImGui::TableSetupColumn("ID", header_columns_flags);
			ImGui::TableSetupColumn("Title", header_columns_flags); // no need to specify title width fixed size, since table can can scroll x and y
			ImGui::TableSetupColumn("Date Time", header_columns_flags);
			ImGui::TableSetupColumn("Type", header_columns_flags);
			ImGui::TableSetupColumn("Operation", header_columns_flags);
			ImGui::TableHeadersRow(); // show headers

			// Rows
			for (const auto& enc_ptr : m_encryptions)
			{
				const auto& [id, title, _, date_time, is_file] = *enc_ptr;

				// id, title, date_time, is_file
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("%d", id);
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%s", title.c_str());
				ImGui::TableSetColumnIndex(2);
				ImGui::Text("%s", date_time.c_str());
				ImGui::TableSetColumnIndex(3);
				ImGui::Text("%s", is_file ? "File" : "Text");
				// Operation (View|delete...)
				ImGui::TableSetColumnIndex(4);
				{
					ImGui::PushStyleColor(ImGuiCol_Button, Constants::Colors::BUTTON_COLOR); // buttons color idle
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Constants::Colors::BUTTON_COLOR_HOVER);  // buttons color hover
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, Constants::Colors::BUTTON_COLOR_ACTIVE); // buttons color pressed
					
					ImGui::PushID(id); // Special id for button
					if (ImGui::Button("View"))
					{
						this->OnViewEncryptionButtonPressed(id);
					}
					ImGui::PopID();

					ImGui::PopStyleColor(3);

					ImGui::SameLine();

					ImGui::PushStyleColor(ImGuiCol_Button, Constants::Colors::BACK_BUTTON_COLOR); // buttons color idle
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Constants::Colors::BACK_BUTTON_COLOR_HOVER);  // buttons color hover
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, Constants::Colors::BACK_BUTTON_COLOR_ACTIVE); // buttons color pressed
					
					ImGui::PushID(id); // Special id for button
					if (ImGui::Button("Delete"))
					{
						this->OnDeleteEncryptionButtonPressed(id);
					}
					ImGui::PopID();
					
					ImGui::PopStyleColor(3);
				}
			}
			ImGui::EndTable();
		}
		ImGui::PopFont();

#if 0
		if (ImGui::BeginTable("Encryptions", NUM_COLUMNS, table_flags))
		{ 
			// Columns header
			static constexpr const auto header_columns_flags = ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoHide;
		    ImGui::TableSetupColumn("ID", header_columns_flags);
			ImGui::TableSetupColumn("Title", header_columns_flags, win_w / 3.5f); // fixed width title
			ImGui::TableSetupColumn("Date Time", header_columns_flags);
			ImGui::TableSetupColumn("Is File?", header_columns_flags);
			ImGui::TableSetupColumn("Operation", header_columns_flags);
			ImGui::TableHeadersRow(); // show headers

			// Rows
			for (const auto& enc_ptr : m_encryptions)
			{
				const auto& [id, title, cipher, date_time, is_file] = *enc_ptr;


				// id, title, date_time, is_file
				ImGui::TableNextColumn(); 
				ImGui::Text("%d", id);
				ImGui::TableNextColumn();
				ImGui::Text("%s", title.c_str());
				ImGui::TableNextColumn();
				ImGui::Text("%s", date_time.c_str());
				ImGui::TableNextColumn();
				ImGui::Text("%s", is_file ? "Yes" : "No");
				// Operation (View|delete...)
				ImGui::TableNextColumn();
				{
					ImGui::PushStyleColor(ImGuiCol_Button, Constants::Colors::BUTTON_COLOR); // buttons color idle
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Constants::Colors::BUTTON_COLOR_HOVER);  // buttons color hover
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, Constants::Colors::BUTTON_COLOR_ACTIVE); // buttons color pressed
					if (ImGui::Button("View"))
					{
						//OnViewEncryptionButtonPressed(row, col)
					}
					ImGui::PopStyleColor(3);

					ImGui::SameLine();

					ImGui::PushStyleColor(ImGuiCol_Button, Constants::Colors::BACK_BUTTON_COLOR); // buttons color idle
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Constants::Colors::BACK_BUTTON_COLOR_HOVER);  // buttons color hover
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, Constants::Colors::BACK_BUTTON_COLOR_ACTIVE); // buttons color pressed
					if (ImGui::Button("Delete"))
					{
						//OnDeleteEncryptionButtonPressed(row, col)
					}
					ImGui::PopStyleColor(3);
				}
			}
			ImGui::EndTable();
		}
		ImGui::PopFont();
#endif

		spacing(2);
		ImGui::Separator();
		spacing(2);

		// Back Button 
		{
			ImGui::PushFont(font_audiowide_regular_20); // buttons font
			ImGui::PushStyleColor(ImGuiCol_Button, Constants::Colors::BUTTON_COLOR); // buttons color idle
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Constants::Colors::BUTTON_COLOR_HOVER);  // buttons color hover
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, Constants::Colors::BUTTON_COLOR_ACTIVE); // buttons color pressed
			{
				ImGui::PushStyleColor(ImGuiCol_Button, Constants::Colors::BACK_BUTTON_COLOR); // buttons color idle
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Constants::Colors::BACK_BUTTON_COLOR_HOVER);  // buttons color hover
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, Constants::Colors::BACK_BUTTON_COLOR_ACTIVE); // buttons color pressed
				ImGui::SetCursorPosX((io.DisplaySize.x - button_size.x) / 2.0f);
				if (ImGui::Button("Back", button_size))
				{
					this->OnBackButtonPressed();
				}
				ImGui::PopStyleColor(3);
			}
			ImGui::PopStyleColor(3);
			ImGui::PopFont();
		}

	}
	ImGui::End();
}

void MyEncryptionsScene::OnEvent(Event& event)
{}

void MyEncryptionsScene::OnDestroy()
{
	ENIGMA_TRACE(ENIGMA_CURRENT_FUNCTION);
	
	// Cleanup
	m_encryptions.clear();
}

void MyEncryptionsScene::GetAllEncryptions()
{
	m_encryptions.clear();

	ENIGMA_INFO("Getting all encryptions from database...");
	
	// Get only title, date_time (no cipher for optimization)
	std::ostringstream oss{};
	{
		//CONSTRUCT ORDER BY & ORDER
		switch (m_order_by)
		{
			case OrderBy::ID: oss << "id"; break;
			case OrderBy::Title: oss << "title"; break;
			case OrderBy::DateTime: oss << "date_time"; break;
		}
		switch (m_order)
		{
			case Order::Ascending: oss << " ASC"; break;
			case Order::Descending: oss << " DESC"; break;
		}
	}
	
	m_encryptions = Database::GetAllEncryptions<true, false, true, true>(oss.str());
	ENIGMA_INFO("Got {0} Encryption records.", m_encryptions.size());
}


void MyEncryptionsScene::OnBackButtonPressed()
{
	this->EndScene();
}

void MyEncryptionsScene::OnViewEncryptionButtonPressed(const size_t id)
{
	ENIGMA_TRACE("View {0}", id);
}

void MyEncryptionsScene::OnDeleteEncryptionButtonPressed(const size_t id)
{
	ENIGMA_TRACE("Delete {0}", id);
}


NS_ENIGMA_END
