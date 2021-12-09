#include <pch.hpp>
#include "LoadingScene.hpp"
#include <GUI/ImGuiWidgets.hpp>

NS_ENIGMA_BEGIN

LoadingScene::LoadingScene()
	:
	Enigma::Scene()
{

}

void LoadingScene::OnCreate()
{
	ENIGMA_TRACE_CURRENT_FUNCTION();
}

void LoadingScene::OnUpdate(const f32& /*dt*/)
{}

void LoadingScene::OnDraw()
{}

void LoadingScene::OnImGuiDraw()
{
	const auto& [win_w, win_h] = Application::GetInstance()->GetWindow()->GetSize();
	static const auto& io = ImGui::GetIO();

	static constexpr const auto container_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;
	
	// Push window's background color
	ImGui::PushStyleColor(ImGuiCol_WindowBg, Constants::Colors::LOADING_BACKGROUND_COLOR);


	ImGui::Begin("Loading", nullptr, container_flags);
	ImGui::SetWindowSize(ImVec2(static_cast<f32>(win_w), static_cast<f32>(win_h))); // same size as window
	ImGui::SetWindowPos(ImVec2(0.0f, 0.0f)); // top left
	{		
		/*
		*	static constexpr const auto loading_txt = "Loading...";
		*	static const ImVec2 title_size(ImGui::CalcTextSize(loading_txt).x * font_audiowide_regular_45->Scale, ImGui::CalcTextSize(loading_txt).y * font_audiowide_regular_45->Scale);
		*	ImGui::SetCursorPosX((io.DisplaySize.x - title_size.x) / 2.0f);
		*	ImGui::SetCursorPosY((io.DisplaySize.y - title_size.y) / 2.0f);
		*	ImGui::Text(loading_txt);
		*/
		
		const ImVec2 spinner_position = { (io.DisplaySize.x - ((SPINNER_RADIUS * SPINNER_THICKNESS) / 2.0f)) / 2.0f,  (io.DisplaySize.y - ((SPINNER_RADIUS * SPINNER_THICKNESS) / 2.0f)) / 2.0f };
		ImGuiWidgets::LoadingDialog(
			m_loading_text.empty() ? "Loading..." : m_loading_text.data(),
			spinner_position, 
			SPINNER_RADIUS,
			SPINNER_THICKNESS,
			Constants::Colors::LOADING_SPINNER_COLOR,
			static_cast<f32>(win_w), 
			static_cast<f32>(win_h)
		);

		
		/*
		static constexpr const float RADIUS = 45;
		static constexpr const float THICKNESS = 6;
		static const ui32 COLOR = ImGui::GetColorU32(Constants::Colors::LOADING_SPINNER_COLOR);  // 0xrrggbbaa
		ImGui::SetCursorPos({ (io.DisplaySize.x - ((RADIUS * THICKNESS)/2.0f)) / 2.0f,  (io.DisplaySize.y - ((RADIUS * THICKNESS) / 2.0f)) / 2.0f });
		ImGuiWidgets::LoadingSpinner("##spinner", RADIUS, THICKNESS, COLOR);
		//ImGuiWidgets::LoadingBar("##loadingbar", 20.0f, {win_w /2.0f, 15.0f}, ImGui::GetColorU32(Constants::Colors::MENUBAR_BACKGROUND_COLOR), ImGui::GetColorU32(Constants::Colors::BUTTON_COLOR));
		*/
	}
	ImGui::End();

	// Pop window's background color
	ImGui::PopStyleColor(1);
}

void LoadingScene::OnEvent(Event& /*event*/)
{

}

void LoadingScene::OnDestroy()
{
	ENIGMA_TRACE_CURRENT_FUNCTION();
}

NS_ENIGMA_END
