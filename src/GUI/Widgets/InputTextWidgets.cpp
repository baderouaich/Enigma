//
// Created by bader on 5/28/26.
//

#include "InputTextWidgets.hpp"

using namespace Enigma;

int InputTextWidgets::InputTextCallback(ImGuiInputTextCallbackData *data)  {
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

bool InputTextWidgets::InputText(const char *label, std::string *str, const float width, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void *user_data)  {
  IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
  flags |= ImGuiInputTextFlags_CallbackResize;

  InputTextCallback_UserData cb_user_data{};
  cb_user_data.Str = str;
  cb_user_data.ChainCallback = callback;
  cb_user_data.ChainCallbackUserData = user_data;
  ImGui::PushItemWidth(width);
  return ImGui::InputText(label, (char *) str->c_str(), str->capacity() + 1, flags, InputTextCallback, &cb_user_data);
}

bool InputTextWidgets::InputTextMultiline(const char *label, std::string *str, const ImVec2 &size, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void *user_data) {
  IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
  flags |= ImGuiInputTextFlags_CallbackResize;

  InputTextCallback_UserData cb_user_data{};
  cb_user_data.Str = str;
  cb_user_data.ChainCallback = callback;
  cb_user_data.ChainCallbackUserData = user_data;
  return ImGui::InputTextMultiline(label, (char *) str->c_str(), str->capacity() + 1, size, flags, InputTextCallback, &cb_user_data);
}

bool InputTextWidgets::InputTextWithHint(const char *label, const char *hint, std::string *str, const float width, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void *user_data)  {
  IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
  flags |= ImGuiInputTextFlags_CallbackResize;

  InputTextCallback_UserData cb_user_data{};
  cb_user_data.Str = str;
  cb_user_data.ChainCallback = callback;
  cb_user_data.ChainCallbackUserData = user_data;
  ImGui::PushItemWidth(width);
  return ImGui::InputTextWithHint(label, hint, (char *) str->c_str(), str->capacity() + 1, flags, InputTextCallback, &cb_user_data);
}
