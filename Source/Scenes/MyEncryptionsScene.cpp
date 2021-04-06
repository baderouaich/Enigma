#include <pch.hpp>
#include "MyEncryptionsScene.hpp"
#include "ViewEncryptionScene.hpp"
#include <Utility/DialogUtils.hpp>

#include <imgui.h>


NS_ENIGMA_BEGIN

MyEncryptionsScene::MyEncryptionsScene()
	:
	Enigma::Scene()
{
}

void MyEncryptionsScene::OnCreate()
{
	ENIGMA_TRACE_CURRENT_FUNCTION();

	// Explicit OpenGL old method to et background clear color
	//glAssert(glClearColor(
	//	Constants::Colors::BACKGROUND_COLOR.x,
	//	Constants::Colors::BACKGROUND_COLOR.y,
	//	Constants::Colors::BACKGROUND_COLOR.z,
	//	Constants::Colors::BACKGROUND_COLOR.w
	//));

	// Get all Encryptions from database
	this->GetAllEncryptions();

}

void MyEncryptionsScene::OnUpdate(const f32&)
{}

void MyEncryptionsScene::OnDraw()
{
	// Clear GL buffers
	//glAssert(glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}

void MyEncryptionsScene::OnImGuiDraw()
{
	const auto& [win_w, win_h] = Application::GetInstance()->GetWindow()->GetSize();
	const auto& [win_x, win_y] = Application::GetInstance()->GetWindow()->GetPosition();

	static const auto& io = ImGui::GetIO();

	const auto button_size = Vec2f(win_w / 2.6f, 40.0f);

	static constexpr const auto inline_dummy = [](const f32& x, const f32& y) noexcept {  ImGui::SameLine(); ImGui::Dummy(ImVec2(x, y)); };
	static constexpr const auto spacing = [](const ui8& n) noexcept { for (ui8 i = 0; i < n; i++) ImGui::Spacing(); };

	static auto& fonts = Application::GetInstance()->GetFonts();
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
			static constexpr const auto title = "My Encryptions";
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

		// Search Query
		ImGui::PushFont(font_montserrat_medium_18); // text font
		ImGui::PushStyleColor(ImGuiCol_Button, Constants::Colors::BUTTON_COLOR); // buttons color idle
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Constants::Colors::BUTTON_COLOR_HOVER);  // buttons color hover
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, Constants::Colors::BUTTON_COLOR_ACTIVE); // buttons color pressed
		{
			//ImGui::LabelText("##label", "Search by title...");
			if (ImGuiWidgets::InputTextWithHint("##inputtext", "Search an encryption by title...", &m_query, win_w / 2.25f, ImGuiInputTextFlags_CallbackEdit)) // ImGuiInputTextFlags_CallbackEdit to return true only on edit so we don't exhaust database 
			{
				// Enable searching
				m_isSearching = true;

				if (!m_query.empty())
				{
					this->OnSearchEncryptionsByTitle();
				}
			}
			else // InputText is not being edited now
			{
				m_isSearching = false;
			}
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, Constants::Colors::BUTTON_COLOR); // buttons color idle
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Constants::Colors::BUTTON_COLOR_HOVER);  // buttons color hover
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, Constants::Colors::BUTTON_COLOR_ACTIVE); // buttons color pressed
			if (ImGui::Button("Reset"))
			{
				m_isSearching = false;
				m_query.clear();
				this->GetAllEncryptions();
			}
			ImGui::PopStyleColor(3);
		}
		ImGui::PopStyleColor(3);
		ImGui::PopFont();

		spacing(2);


		if (!m_encryptions.empty())
		{
			// Order By
			ImGui::PushFont(font_montserrat_medium_18); // text font
			ImGui::Text("Order By "); ImGui::SameLine();
			if (ImGui::RadioButton("ID", m_order_by == Database::OrderBy::ID))
			{
				if (m_order_by != Database::OrderBy::ID) // avoid exhausting database
				{
					m_order_by = Database::OrderBy::ID;
					this->GetAllEncryptions();
				}

			}
			ImGui::SameLine();
			if (ImGui::RadioButton("Title", m_order_by == Database::OrderBy::Title))
			{
				if (m_order_by != Database::OrderBy::Title) // avoid exhausting database
				{
					m_order_by = Database::OrderBy::Title;
					this->GetAllEncryptions();
				}
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("Date Time", m_order_by == Database::OrderBy::DateTime))
			{
				if (m_order_by != Database::OrderBy::DateTime) // avoid exhausting database
				{
					m_order_by = Database::OrderBy::DateTime;
					this->GetAllEncryptions();
				}
			}
			// Order ASC DESC
			ImGui::Text("Order "); ImGui::SameLine();
			if (ImGui::RadioButton("Ascending", m_order == Database::Order::Ascending))
			{
				if (m_order != Database::Order::Ascending) // avoid exhausting database
				{
					m_order = Database::Order::Ascending;
					this->GetAllEncryptions();
				}
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("Descending", m_order == Database::Order::Descending))
			{
				if (m_order != Database::Order::Descending) // avoid exhausting database
				{
					m_order = Database::Order::Descending;
					this->GetAllEncryptions();
				}
			}
			ImGui::PopFont();

			spacing(2);
			ImGui::Separator();
			spacing(2);


			// Encryptions records Table 
			// https://github.com/ocornut/imgui/blob/master/imgui_demo.cpp#L5024
			{
				ImGui::PushFont(font_montserrat_medium_18); // text font
			// settings inspired from https://github.com/ocornut/imgui/issues/2957 # https://user-images.githubusercontent.com/8225057/71590710-dd057f80-2b29-11ea-8c1f-d827c008e050.png

				static constexpr const auto table_flags =
					ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable |
					ImGuiTableFlags_RowBg | // i % 2 ? dark row : light row
					ImGuiTableFlags_Hideable | ImGuiTableFlags_BordersV |
					ImGuiTableFlags_BordersOuterV | ImGuiTableFlags_BordersInnerV |
					ImGuiTableFlags_BordersH | ImGuiTableFlags_BordersOuterH |
					ImGuiTableFlags_BordersInnerH |
					ImGuiTableFlags_SizingStretchSame |
					ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY;
				//ScrollFreezeTopRow
				/*
				static constexpr const auto table_flags =
					ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable |
					ImGuiTableFlags_Hideable | ImGuiTableFlags_Borders |
					ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoSavedSettings |
					ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY |
					ImGuiTableFlags_RowBg | // i % 2 ? dark row : light row
					;
				*/
				static constexpr const auto NUM_COLUMNS = 5 + 1; // id, title... + 1 field has 2 buttons Operation(View|Delete)
				//if (ImGui::BeginTable("Encryptions", NUM_COLUMNS, table_flags, ImVec2(win_w - 20.0f, win_h / 1.6f)))
				if (ImGui::BeginTable("Encryptions", NUM_COLUMNS, table_flags))
				{
					ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible

					// Columns header
					/*
					static constexpr const auto header_columns_flags =
						ImGuiTableColumnFlags_DefaultSort | //  // Default as a sorting column
						ImGuiTableColumnFlags_NoHide | // Disable ability to hide / disable this column
						ImGuiTableColumnFlags_WidthStretch; // Column will stretch
					*/
					static constexpr const auto header_columns_flags =
						ImGuiTableColumnFlags_NoSort |    // disable sorting (right click on header column)
						ImGuiTableColumnFlags_NoHide |    // disable hiding (right click to select which column to hide/show)
						ImGuiTableColumnFlags_NoReorder | // disable column reordering
						ImGuiTableColumnFlags_NoResize    // disable column resize
						;

					ImGui::TableSetupColumn("ID", header_columns_flags | ImGuiTableColumnFlags_WidthFixed);
					ImGui::TableSetupColumn("Title", header_columns_flags | ImGuiTableColumnFlags_WidthStretch); // no need to specify title width fixed size, since table can can scroll x and y
					ImGui::TableSetupColumn("Date Time", header_columns_flags | ImGuiTableColumnFlags_WidthFixed);
					ImGui::TableSetupColumn("Size", header_columns_flags | ImGuiTableColumnFlags_WidthFixed);
					ImGui::TableSetupColumn("Type", header_columns_flags | ImGuiTableColumnFlags_WidthFixed);
					ImGui::TableSetupColumn("Operation", header_columns_flags | ImGuiTableColumnFlags_WidthFixed);
					ImGui::TableHeadersRow(); // show headers

					// Rows
					//for (const auto& enc_ptr : m_isSearching ? m_search_encryptions : m_encryptions)
					for (const auto& enc_ptr : m_encryptions)
					{
						const auto& [id, title, _, date_time, size, is_file] = *enc_ptr;

						// id, title, date_time, size, is_file
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);
						ImGui::Text("%zd", id);
						ImGui::TableSetColumnIndex(1);
						ImGui::TextWrapped("%s", title.c_str());
						ImGui::TableSetColumnIndex(2);
						ImGui::Text("%s", date_time.c_str());
						ImGui::TableSetColumnIndex(3);
						ImGui::Text("%s", ENIGMA_FRIENDLY_BYTES_SIZE(size).c_str());
						ImGui::TableSetColumnIndex(4);
						ImGui::Text("%s", is_file ? "File" : "Text");
						// Operation (View|delete...)
						ImGui::TableSetColumnIndex(5);
						{
							ImGui::PushID(static_cast<i32>(id)); // Special id for button
							if (ImGuiWidgets::Button("View"))
							{
								this->OnViewEncryptionButtonPressed(id);
							}
							ImGui::PopID();

							ImGui::SameLine();

							ImGui::PushID(static_cast<i32>(id)); // Special id for button
							if (ImGuiWidgets::Button("Delete", ImVec2(), Constants::Colors::BACK_BUTTON_COLOR, Constants::Colors::BACK_BUTTON_COLOR_HOVER, Constants::Colors::BACK_BUTTON_COLOR_ACTIVE))
							{
								// if item is deleted, vector size is changed so break loop.
								if (this->OnDeleteEncryptionButtonPressed(id))
								{
									ImGui::PopID(); // unregister button id after deletion
									break;
								}
							}
							ImGui::PopID();

						}
					}
					ImGui::EndTable();
				}
			}
			ImGui::PopFont();
		}
		else // No Encryption records saved yet.
		{
			// Centre text no saved encryption records
			ImGui::PushFont(font_montserrat_medium_18); // text font
			ImGui::PushStyleColor(ImGuiCol_Text, Constants::Colors::TEXT_COLOR); // text color
			{
				static const auto text = m_isSearching ? "No Encryptions found." : "No Encryptions saved yet.";
				static const ImVec2 text_size(ImGui::CalcTextSize(text).x * font_montserrat_medium_18->Scale, ImGui::CalcTextSize(text).y * font_montserrat_medium_18->Scale);
				ImGui::SetCursorPosX((io.DisplaySize.x - text_size.x) / 2.0f);
				ImGui::Text(text);
			}
			ImGui::PopStyleColor(1);
			ImGui::PopFont();
		}

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


	}
	ImGui::End();


	// Pop window's background color
	ImGui::PopStyleColor(1);
}

void MyEncryptionsScene::OnEvent(Event& /*event*/)
{}

void MyEncryptionsScene::OnDestroy()
{
	ENIGMA_TRACE_CURRENT_FUNCTION();

	// Cleanup
	m_encryptions.clear();
}

void MyEncryptionsScene::GetAllEncryptions()
{
	ENIGMA_TRACE_CURRENT_FUNCTION();

	m_encryptions.clear();

	ENIGMA_INFO("Getting all encryptions from database...");
	m_encryptions = Database::GetAllEncryptions<true, false, true, true, true>(m_order_by, m_order);
	ENIGMA_INFO("Got {0} Encryption records.", m_encryptions.size());

}


void MyEncryptionsScene::OnBackButtonPressed()
{
	Scene::EndScene();
}

void MyEncryptionsScene::OnViewEncryptionButtonPressed(const i64 ide)
{
	ENIGMA_TRACE_CURRENT_FUNCTION();

	ENIGMA_TRACE("View {0}", ide);

	Application::GetInstance()->PushScene(std::make_unique<ViewEncryptionScene>(ide));
}

// returns true if item deleted successfully to notify draw loop that vector range changed
bool MyEncryptionsScene::OnDeleteEncryptionButtonPressed(const i64 ide)
{
	ENIGMA_TRACE_CURRENT_FUNCTION();

	ENIGMA_TRACE("Delete {0}", ide);
	ENIGMA_TRACE("Confirming deletion of encryption with id {0}", ide);

	const auto action = DialogUtils::Warn("Are you sure you want to delete encryption?", MessageBox::Choice::Yes_No_Cancel);
	if (action == MessageBox::Action::Yes)
	{
		ENIGMA_TRACE("Deleting encryption with id {0} from database", ide);
		// Remove from database
		const bool deleted = Database::DeleteEncryption(ide);
		if (deleted)
		{
			ENIGMA_TRACE("Deleting encryption with id {0} from vector", ide);
			// Remove from m_encryptions vector
			for (auto it = m_encryptions.begin(); it != m_encryptions.end(); ++it)
			{
				if ((*it)->ide == ide)
				{
					m_encryptions.erase(it);
					break;
				}
			}
			return true;
		}
		else
		{
			const String msg = "Couldn't delete encryption record with id " + std::to_string(ide) + " from database";
			ENIGMA_ERROR(msg);
			(void)DialogUtils::Error(msg);
			return false;
		}
	}
	return false;
}

void MyEncryptionsScene::OnSearchEncryptionsByTitle()
{
	ENIGMA_TRACE_CURRENT_FUNCTION();

	m_encryptions.clear();
	m_encryptions = Database::SearchEncryptionsByTitle<true, false, true, true, true>(m_query);

}


NS_ENIGMA_END
