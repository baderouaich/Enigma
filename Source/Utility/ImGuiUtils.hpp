#pragma once
#ifndef ENIGMA_IMGUI_UTILS_H
#define ENIGMA_IMGUI_UTILS_H

#include <Core/Core.hpp>
#include <imgui.h>

NS_ENIGMA_BEGIN
class ENIGMA_API ImGuiUtils
{
public:
	/*
	*	InputTextMultiline with container std::string instread of char* 
	*	https://github.com/ocornut/imgui/issues/2035
	*/
	static int InputTextMultilineCallback(ImGuiInputTextCallbackData* data)
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
		return ImGui::InputTextMultiline(label, (char*)str->c_str(), str->capacity() + 1, size, flags, InputTextMultilineCallback, (void*)str);
	}

};
NS_ENIGMA_END

#endif // !ENIGMA_IMGUI_UTILS_H
