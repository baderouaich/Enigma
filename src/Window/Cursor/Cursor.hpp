#pragma once
#ifndef ENIGMA_CURSOR_H
#define ENIGMA_CURSOR_H

#include <Core/Core.hpp>
#include <GLFW/glfw3.h>

NS_ENIGMA_BEGIN

/*Ref https://www.glfw.org/docs/3.4/group__shapes.html */
enum class CursorMode : byte {
  Arrow = 0,
  IBeam,
  CrossHair,
  Hand,
  HResize,
  VResize,

  NumCursors,

  NONE = Arrow
};

class Cursor {
  public:
    /*
    *	Default Constructor
    */
    Cursor() noexcept;

    /*
    *	Creates Standard Cursor
    */
    explicit Cursor(CursorMode mode);

    /*
    *	Creates Image Based Cursor
    */
    explicit Cursor(const fs::path& image_path, const std::int32_t xhot = 0, const std::int32_t yhot = 0);

    ~Cursor();

    void Destroy();

  public:
    CursorMode GetMode() const noexcept { return m_mode; }
    const GLFWcursor *GetGLFWCursor() const noexcept { return m_GLFWcursor; }
    GLFWcursor *GetGLFWCursor() noexcept { return m_GLFWcursor; }

  private:
    CursorMode m_mode;
    GLFWcursor *m_GLFWcursor;
};

NS_ENIGMA_END

#endif // !ENIGMA_CURSOR_H
