#pragma once
#ifndef ENIGMA_CURSOR_H
#define ENIGMA_CURSOR_H

#include <Core/Core.hpp>
#include <GLFW/glfw3.h>

NS_ENIGMA_BEGIN

/*Ref https://www.glfw.org/docs/3.4/group__shapes.html */
enum class CursorMode : ui8
{
	Arrow	= 0,
	IBeam,
	CrossHair,
	Hand,
	HResize,
	VResize,

	NumCursors,

	NONE = Arrow
};

class ENIGMA_API Cursor

{
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
	explicit Cursor(const String& image_path, const i32& xhot = 0, const i32& yhot = 0);

	virtual ~Cursor();

	void Destroy();

public:
	const CursorMode& GetMode() const noexcept { return m_mode; }
	const GLFWcursor* GetGLFWCursor() const noexcept { return m_GLFWcursor; }
	GLFWcursor* GetGLFWCursor() noexcept { return m_GLFWcursor; }

private:
	CursorMode m_mode;
	GLFWcursor* m_GLFWcursor;
};

NS_ENIGMA_END

#endif // !ENIGMA_CURSOR_H
