//
// Created by bader on 5/28/26.
//
#pragma once

#include <Application/Application.hpp>
#include <Core/Core.hpp>
#include <imgui.h>

NS_ENIGMA_BEGIN
class ImageWidgets final {
  ENIGMA_STATIC_CLASS(ImageWidgets);

public:
  /**
	*	ImGui GPU Based Image
	*
	*	@note: https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples#Example-for-OpenGL-users
	*/
  class Image {
  public:
    explicit Image(const byte* memBuffer, std::size_t size);
    explicit Image(const char *file_name);
    ~Image();

    void Draw(const ImVec2 &position, const float width, const float height);

  private:
    void createOpenGLTexture(const byte* buffer, const std::size_t size);

  public:
    [[nodiscard]] std::int32_t GetWidth() const noexcept { return m_width; };
    [[nodiscard]] std::int32_t GetHeight() const noexcept { return m_height; };
    [[nodiscard]] GLuint GetID() const noexcept { return m_id; };

  private:
    GLuint m_id{}; // OpenGL texture id
    std::int32_t m_width{};
    std::int32_t m_height{};
  };
};


NS_ENIGMA_END