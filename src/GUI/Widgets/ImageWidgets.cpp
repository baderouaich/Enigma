//
// Created by bader on 5/28/26.
//

#include "ImageWidgets.hpp"

using namespace Enigma;

ImageWidgets::Image::Image(const byte* memBuffer, std::size_t size) {
  // Load image from memory buffer
  byte* pixels = stbi_load_from_memory(
      memBuffer,
      static_cast<int>(size),
      &m_width,
      &m_height,
      nullptr,
      4);
  ENIGMA_ASSERT(pixels, "Failed to read image from memory buffer");

  this->createOpenGLTexture(pixels, size);

  // at this point the buffer is uploaded to the gpu, no need to keep in the memory
  stbi_image_free(pixels);
}

ImageWidgets::Image::Image(const char *file_name) {
  // Load from file
  byte *pixels = stbi_load(file_name, &m_width, &m_height, nullptr, 4);
  ENIGMA_ASSERT(pixels, std::string("Failed to read image from ") + file_name);

  this->createOpenGLTexture(pixels, m_width * m_height * sizeof(byte));

  // at this point the buffer is uploaded to the gpu, no need to keep in the memory
  stbi_image_free(pixels);

}
void ImageWidgets::Image::createOpenGLTexture(const byte *buffer, const std::size_t size) {
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

}

ImageWidgets::Image::~Image() {
  ENIGMA_INFO("Deleting texture #{0} from GPU Memory", m_id);
  glAssert(glDeleteTextures(1, &m_id));
}

void ImageWidgets::Image::Draw(const ImVec2 &position, const float width, const float height) {
  ImGui::SetCursorPos(position);
  ImGui::Image(m_id, ImVec2(width, height));
}