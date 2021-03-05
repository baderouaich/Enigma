#pragma once
#ifndef ENIGMA_IMGUI_WIDGETS_H
#define ENIGMA_IMGUI_WIDGETS_H

#include <Core/Core.hpp>
#include <imgui.h>

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
		return ImGui::InputTextMultiline(label, (char*)str->c_str(), str->capacity() + 1, size, flags, InputTextCallback, (void*)str);
	}
	static bool InputText(const char* label, std::string* str, const float& width, ImGuiInputTextFlags flags = 0)
	{
		ImGui::PushItemWidth(width);
		flags |= ImGuiInputTextFlags_CallbackResize;
		bool ret = ImGui::InputText(label, (char*)str->c_str(), str->capacity() + 1, flags, InputTextCallback, (void*)str);
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

}

NS_ENIGMA_END

#endif // !ENIGMA_IMGUI_WIDGETS_H
