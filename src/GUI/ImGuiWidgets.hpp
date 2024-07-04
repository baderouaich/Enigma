#pragma once
#ifndef ENIGMA_IMGUI_WIDGETS_H
#define ENIGMA_IMGUI_WIDGETS_H

#include <Application/Application.hpp>
#include <Core/Core.hpp>
#include <Utility/OpenGLUtils.hpp>
#include <Utility/SizeUtils.hpp>
#include <imgui.h>
#include <imgui_internal.h>

NS_ENIGMA_BEGIN

class ImGuiWidgets final {
    ENIGMA_STATIC_CLASS(ImGuiWidgets);

  public:
    /**
	*	InputTextMultiline with stl container std::string instread of char* static size
	*	
	*	@note: https://github.com/ocornut/imgui/issues/2035, https://github.com/ocornut/imgui/blob/master/misc/cpp/imgui_stdlib.cpp
	*/
    struct InputTextCallback_UserData {
        std::string *Str;
        ImGuiInputTextCallback ChainCallback;
        void *ChainCallbackUserData;
    };
    static int InputTextCallback(ImGuiInputTextCallbackData *data) {
      InputTextCallback_UserData *user_data = static_cast<InputTextCallback_UserData *>(data->UserData);
      if (data->EventFlag == static_cast<decltype(data->EventFlag)>(ImGuiInputTextFlags_CallbackResize)) {
        // Resize string callback
        // If for some reason we refuse the new length (BufTextLen) and/or capacity (BufSize) we need to set them back to what we want.
        std::string *str = user_data->Str;
        IM_ASSERT(data->Buf == str->c_str());
        str->resize(data->BufTextLen);
        data->Buf = (char *) str->c_str();
      } else if (user_data->ChainCallback) {
        // Forward to user callback, if any
        data->UserData = user_data->ChainCallbackUserData;
        return user_data->ChainCallback(data);
      }
      return 0;
    }

    /** ImGui InputText with dynamic std::string */
    static bool InputText(const char *label, std::string *str, const float width, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = nullptr, void *user_data = nullptr) {
      IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
      flags |= ImGuiInputTextFlags_CallbackResize;

      InputTextCallback_UserData cb_user_data;
      cb_user_data.Str = str;
      cb_user_data.ChainCallback = callback;
      cb_user_data.ChainCallbackUserData = user_data;
      ImGui::PushItemWidth(width);
      return ImGui::InputText(label, (char *) str->c_str(), str->capacity() + 1, flags, InputTextCallback, &cb_user_data);
    }

    /** ImGui InputTextMultiline with dynamic std::string */
    static bool InputTextMultiline(const char *label, std::string *str, const ImVec2& size, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = nullptr, void *user_data = nullptr) {
      IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
      flags |= ImGuiInputTextFlags_CallbackResize;

      InputTextCallback_UserData cb_user_data;
      cb_user_data.Str = str;
      cb_user_data.ChainCallback = callback;
      cb_user_data.ChainCallbackUserData = user_data;
      return ImGui::InputTextMultiline(label, (char *) str->c_str(), str->capacity() + 1, size, flags, InputTextCallback, &cb_user_data);
    }

    /** ImGui InputTextWithHint with dynamic std::string */
    static bool InputTextWithHint(const char *label, const char *hint, std::string *str, const float width, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = nullptr, void *user_data = nullptr) {
      IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
      flags |= ImGuiInputTextFlags_CallbackResize;

      InputTextCallback_UserData cb_user_data;
      cb_user_data.Str = str;
      cb_user_data.ChainCallback = callback;
      cb_user_data.ChainCallbackUserData = user_data;
      ImGui::PushItemWidth(width);
      return ImGui::InputTextWithHint(label, hint, (char *) str->c_str(), str->capacity() + 1, flags, InputTextCallback, &cb_user_data);
    }

#if 0
	static int InputTextCallback(ImGuiInputTextCallbackData* data)
	{
		if (data->EventFlag == static_cast<decltype(data->EventFlag)>(ImGuiInputTextFlags_CallbackResize))
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
	static bool InputText(const char* label, std::string* str, const float width, ImGuiInputTextFlags flags = 0)
	{
		ImGui::PushItemWidth(width);
		flags |= ImGuiInputTextFlags_CallbackResize;
		bool ret = ImGui::InputText(label, (char*)str->c_str(), str->capacity() + 1, flags, ImGuiWidgets::InputTextCallback, (void*)str);
		ImGui::PopItemWidth();
		return ret;
	}
	static bool InputTextWithHint(const char* label, const char* hint, std::string* str, const float width, ImGuiInputTextFlags flags = 0)
	{
		ImGui::PushItemWidth(width);
		flags |= ImGuiInputTextFlags_CallbackResize;
		bool ret = ImGui::InputTextWithHint(label, hint, (char*)str->c_str(), str->capacity() + 1, flags, ImGuiWidgets::InputTextCallback, (void*)str);
		ImGui::PopItemWidth();
		return ret;

	}
#endif
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    /** ImGui Button with default color, hover color, active color */
    static bool Button(const char *text,
                       const ImVec2& size = ImVec2(0.0f, 0.0f),
                       const ImVec4& color = Constants::Colors::BUTTON_COLOR,
                       const ImVec4& color_hover = Constants::Colors::BUTTON_COLOR_HOVER,
                       const ImVec4& color_active = Constants::Colors::BUTTON_COLOR_ACTIVE) {
      ImGui::PushStyleColor(ImGuiCol_Button, color);              // buttons color idle
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color_hover); // buttons color hover
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, color_active); // buttons color pressed
      bool pressed_or_selected = ImGui::Button(text, size);
      ImGui::PopStyleColor(3);

      return pressed_or_selected;
    }

    /** ImGui BackButton with default color, hover color, active color */
    static bool BackButton(const char *str_id,
                           const ImVec2& size = ImVec2(0.0f, 0.0f),
                           const ImVec4& color = Constants::Colors::BACK_BUTTON_COLOR,
                           const ImVec4& color_hover = Constants::Colors::BACK_BUTTON_COLOR_HOVER,
                           const ImVec4& color_active = Constants::Colors::BACK_BUTTON_COLOR_ACTIVE) {
      ImGui::PushStyleColor(ImGuiCol_Button, color);              // buttons color idle
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color_hover); // buttons color hover
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, color_active); // buttons color pressed
      bool pressed_or_selected = ImGui::ArrowButtonEx(str_id, ImGuiDir_Left, size);
      ImGui::PopStyleColor(3);

      return pressed_or_selected;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
	*	ImGui Loading spinner 
	* 
	*	@note: https://github.com/ocornut/imgui/issues/1901
	*/
    static bool LoadingSpinner(const char *label, float radius, float thickness, const ImU32 color) {
      ImGuiWindow *window = ImGui::GetCurrentWindow();
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
      int start = (int) std::abs(sinf((float) g.Time * 1.8f) * (num_segments - 5));

      const float a_min = IM_PI * 2.0f * ((float) start) / (float) num_segments;
      const float a_max = IM_PI * 2.0f * ((float) num_segments - 3) / (float) num_segments;

      const ImVec2 centre = ImVec2(pos.x + radius, pos.y + radius + style.FramePadding.y);

      for (int i = 0; i < num_segments; i++) {
        const float a = a_min + ((float) i / (float) num_segments) * (a_max - a_min);
        window->DrawList->PathLineTo(ImVec2(centre.x + cosf(a + (float) g.Time * 8.0f) * radius,
                                            centre.y + sinf(a + (float) g.Time * 8.0f) * radius));
      }

      window->DrawList->PathStroke(color, false, thickness);

      return true;
    }

    /**
	*	ImGui Loading bar 
	*
	*	@note: https://github.com/ocornut/imgui/issues/1901
	*/
    static bool LoadingBar(const char *label, float value, const ImVec2& size_arg, const ImU32 bg_col, const ImU32 fg_col) {
      ImGuiWindow *window = ImGui::GetCurrentWindow();
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

      const float t = static_cast<float>(g.Time);
      const float r = size.y / 2;
      const float speed = 1.5f;

      const float a = speed * 0;
      const float b = speed * 0.333f;
      const float c = speed * 0.666f;

      const float o1 = (circleWidth + r) * (t + a - speed * (int) ((t + a) / speed)) / speed;
      const float o2 = (circleWidth + r) * (t + b - speed * (int) ((t + b) / speed)) / speed;
      const float o3 = (circleWidth + r) * (t + c - speed * (int) ((t + c) / speed)) / speed;

      window->DrawList->AddCircleFilled(ImVec2(pos.x + circleEnd - o1, bb.Min.y + r), r, bg_col);
      window->DrawList->AddCircleFilled(ImVec2(pos.x + circleEnd - o2, bb.Min.y + r), r, bg_col);
      window->DrawList->AddCircleFilled(ImVec2(pos.x + circleEnd - o3, bb.Min.y + r), r, bg_col);

      return true;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
	*	Popup dialog has spinner loading inside it, displayed when a worker thread is launched
	*	to prevent interactions with the scene widgets until the thread is finished.
	* 
	*	@note: https://github.com/ocornut/imgui/issues/249
	*/
    static void LoadingDialog(const char *text, const ImVec2& spinner_position, const float spinner_radius, const float spinner_thickness, const ImVec4& spinner_color,
                              const float container_width = (float) Application::getInstance()->GetWindow()->GetWidth(),
                              const float container_height = (float) Application::getInstance()->GetWindow()->GetHeight()) {
      static constexpr auto popup_id = "loadingDialog";

      const auto& [win_w, win_h] = Application::getInstance()->GetWindow()->GetSize();
      static const auto& fonts = Application::getInstance()->GetFonts();
      static ImFont *const& font_audiowide_regular_20 = fonts.at("Audiowide-Regular-20");

      ImGui::OpenPopup(popup_id, ImGuiPopupFlags_AnyPopup);
      static constexpr const auto popup_window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground;
      if (ImGui::BeginPopupModal(popup_id, nullptr, popup_window_flags)) {
        ImGui::SetWindowSize(ImVec2(container_width, container_height)); // set spinner container's size same size as window (to centerize it)
        ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));                         // top left

        static const std::uint32_t COLOR = ImGui::GetColorU32(spinner_color); // 0xrrggbbaa
        ImGui::SetCursorPos(spinner_position);
        ImGuiWidgets::LoadingSpinner("##spinner", spinner_radius, spinner_thickness, COLOR);
        ImGui::PushFont(font_audiowide_regular_20); // text font
        const auto text_size = ImGui::CalcTextSize(text);
        ImGui::SetCursorPosX((win_w - text_size.x) / 2.0f);
        ImGui::TextWrapped(text);
        ImGui::PopFont();
        ImGui::EndPopup();
      }
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /**
	*	ImGui GPU Based Image
	* 
	*	@note: https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples#Example-for-OpenGL-users
	*/
    class Image {
      public:
        explicit Image(const char *file_name) {
          // Load from file
          byte *buffer = stbi_load(file_name, &m_width, &m_height, nullptr, 4);
          ENIGMA_ASSERT(buffer, std::string("Failed to read image from ") + file_name);

          // Create a OpenGL texture identifier
          glAssert(glGenTextures(1, &m_id));
          glAssert(glBindTexture(GL_TEXTURE_2D, m_id));

          // Setup filtering parameters for display
          glAssert(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
          glAssert(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
          glAssert(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)); // This is required on WebGL for non power-of-two textures
          glAssert(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE)); // Same

          // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
          glAssert(glPixelStorei(GL_UNPACK_ROW_LENGTH, 0));
#endif
          ENIGMA_INFO("Storing texture #{0} {1} to GPU Memory", m_id, SizeUtils::FriendlySize((std::size_t) m_width * (std::size_t) m_height * sizeof(byte)));

          glAssert(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer));

          // at this point the buffer is uploaded to the gpu, no need to keep in the memory
          stbi_image_free(buffer);
        }

        ~Image() {
          ENIGMA_INFO("Deleting texture #{0} from GPU Memory", m_id);
          glAssert(glDeleteTextures(1, &m_id));
        }

        void Draw(const ImVec2& position, const float width, const float height) {
          ImGui::SetCursorPos(position);
          ImGui::Image((void *) (std::intptr_t) m_id, ImVec2(width, height));
        }

      public:
        std::int32_t GetWidth() const noexcept { return m_width; };
        std::int32_t GetHeight() const noexcept { return m_height; };
        GLuint GetID() const noexcept { return m_id; };

      private:
        GLuint m_id{}; // OpenGL texture id
        std::int32_t m_width{};
        std::int32_t m_height{};
    };
};

NS_ENIGMA_END

#endif // !ENIGMA_IMGUI_WIDGETS_H
