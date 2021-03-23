#pragma once
#ifndef ENIGMA_IMGUI_WIDGETS_H
#define ENIGMA_IMGUI_WIDGETS_H

#include <Core/Core.hpp>
#include <imgui.h>
#include <imgui_internal.h>

NS_ENIGMA_BEGIN

namespace ImGuiWidgets
{
	/*
	*	InputTextMultiline with container std::string instread of char*
	*	https://github.com/ocornut/imgui/issues/2035
	*/
	static int InputTextCallback(ImGuiInputTextCallbackData* data)
	{
		if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
		{
			// Resize string callback
			std::string* str = (std::string*)data->UserData;
			IM_ASSERT(data->Buf == str->c_str());
			str->resize(data->BufTextLen);
			data->Buf = (char*)str->c_str();
		}
		return 0;
	}
	static bool InputTextMultiline(const char* label, std::string* str, const ImVec2& size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0)
	{
		flags |= ImGuiInputTextFlags_CallbackResize;
		return ImGui::InputTextMultiline(label, (char*)str->c_str(), str->capacity() + 1, size, flags, ImGuiWidgets::InputTextCallback, (void*)str);
	}
	static bool InputText(const char* label, std::string* str, const float& width, ImGuiInputTextFlags flags = 0)
	{
		ImGui::PushItemWidth(width);
		flags |= ImGuiInputTextFlags_CallbackResize;
		bool ret = ImGui::InputText(label, (char*)str->c_str(), str->capacity() + 1, flags, ImGuiWidgets::InputTextCallback, (void*)str);
		ImGui::PopItemWidth();
		return ret;
	}
	static bool InputTextWithHint(const char* label, const char* hint, std::string* str, const float& width, ImGuiInputTextFlags flags = 0)
	{
		ImGui::PushItemWidth(width);
		flags |= ImGuiInputTextFlags_CallbackResize;
		bool ret = ImGui::InputTextWithHint(label, hint, (char*)str->c_str(), str->capacity() + 1, flags, ImGuiWidgets::InputTextCallback, (void*)str);
		ImGui::PopItemWidth();
		return ret;

	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	static bool Button(const char* text,
		const ImVec2& size = ImVec2(0.0f, 0.0f),
		const ImVec4& color = Constants::Colors::BUTTON_COLOR,
		const ImVec4& color_hover = Constants::Colors::BUTTON_COLOR_HOVER,
		const ImVec4& color_active = Constants::Colors::BUTTON_COLOR_ACTIVE)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, color); // buttons color idle
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color_hover);  // buttons color hover
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, color_active); // buttons color pressed
			bool pressed_or_selected = ImGui::Button(text, size);
		ImGui::PopStyleColor(3);

		return pressed_or_selected;
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Loading spinner https://github.com/ocornut/imgui/issues/1901
	static bool LoadingSpinner(const char* label, float radius, int thickness, const ImU32& color) {
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);

		ImVec2 pos = window->DC.CursorPos;
		ImVec2 size((radius) * 2, (radius + style.FramePadding.y) * 2);

		const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
		ImGui::ItemSize(bb, style.FramePadding.y);
		if (!ImGui::ItemAdd(bb, id))
			return false;

		// Render
		window->DrawList->PathClear();

		int num_segments = 30;
		int start = (int)std::abs(sinf(g.Time * 1.8f) * (num_segments - 5));

		const float a_min = IM_PI * 2.0f * ((float)start) / (float)num_segments;
		const float a_max = IM_PI * 2.0f * ((float)num_segments - 3) / (float)num_segments;

		const ImVec2 centre = ImVec2(pos.x + radius, pos.y + radius + style.FramePadding.y);

		for (int i = 0; i < num_segments; i++) {
			const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
			window->DrawList->PathLineTo(ImVec2(centre.x + cosf(a + g.Time * 8.0f) * radius,
				centre.y + sinf(a + g.Time * 8.0f) * radius));
		}

		window->DrawList->PathStroke(color, false, thickness);

		return true;

	}

	// Loading bar https://github.com/ocornut/imgui/issues/1901
	static bool LoadingBar(const char* label, float value, const ImVec2& size_arg, const ImU32& bg_col, const ImU32& fg_col) {
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);

		ImVec2 pos = window->DC.CursorPos;
		ImVec2 size = size_arg;
		size.x -= style.FramePadding.x * 2;

		const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
		ImGui::ItemSize(bb, style.FramePadding.y);
		if (!ImGui::ItemAdd(bb, id))
			return false;

		// Render
		const float circleStart = size.x * 0.7f;
		const float circleEnd = size.x;
		const float circleWidth = circleEnd - circleStart;

		window->DrawList->AddRectFilled(bb.Min, ImVec2(pos.x + circleStart, bb.Max.y), bg_col);
		window->DrawList->AddRectFilled(bb.Min, ImVec2(pos.x + circleStart * value, bb.Max.y), fg_col);

		const float t = g.Time;
		const float r = size.y / 2;
		const float speed = 1.5f;

		const float a = speed * 0;
		const float b = speed * 0.333f;
		const float c = speed * 0.666f;

		const float o1 = (circleWidth + r) * (t + a - speed * (int)((t + a) / speed)) / speed;
		const float o2 = (circleWidth + r) * (t + b - speed * (int)((t + b) / speed)) / speed;
		const float o3 = (circleWidth + r) * (t + c - speed * (int)((t + c) / speed)) / speed;

		window->DrawList->AddCircleFilled(ImVec2(pos.x + circleEnd - o1, bb.Min.y + r), r, bg_col);
		window->DrawList->AddCircleFilled(ImVec2(pos.x + circleEnd - o2, bb.Min.y + r), r, bg_col);
		window->DrawList->AddCircleFilled(ImVec2(pos.x + circleEnd - o3, bb.Min.y + r), r, bg_col);

		return true;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//https://github.com/ocornut/imgui/issues/249
	// Popup dialog has spinner loading inside it
	static void LoadingDialog(const ImVec2& spinner_position, const float spinner_radius, const int spinner_thickness, const ImVec4& spinner_color, 
		const float container_width = (float)Application::GetInstance()->GetWindow()->GetSize().first,
		const float container_height = (float)Application::GetInstance()->GetWindow()->GetSize().second)
	{
		static constexpr auto popup_id = "loadingDialog";

		ImGui::OpenPopup(popup_id, ImGuiPopupFlags_AnyPopup);
		static constexpr const auto popup_window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground;
		if (ImGui::BeginPopupModal(popup_id, nullptr, popup_window_flags))
		{
			ImGui::SetWindowSize(ImVec2(container_width, container_height)); // set spinner container's size same size as window (to centerize it)
			ImGui::SetWindowPos(ImVec2(0.0f, 0.0f)); // top left

			static const ui32 COLOR = ImGui::GetColorU32(spinner_color);  // 0xrrggbbaa
			ImGui::SetCursorPos(spinner_position);
			ImGuiWidgets::LoadingSpinner("##spinner", spinner_radius, spinner_thickness, COLOR);
			ImGui::EndPopup();

		}
	
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


}

NS_ENIGMA_END

#endif // !ENIGMA_IMGUI_WIDGETS_H
