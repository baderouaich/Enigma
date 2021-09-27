#pragma once
#ifndef ENIGMA_INPUT_H
#define ENIGMA_INPUT_H

#include <Core/Core.hpp>

// Keyboard
#include <Input/Keyboard/KeyCodes.hpp>
// Mouse
#include <Input/Mouse/MouseCodes.hpp>

NS_ENIGMA_BEGIN

/** Input static class to gather input information at runtime like Mouse position, Is a keyboard button pressed.. */
class ENIGMA_API Input
{
public: /* Keyboard */
	/** Returns true if the keyboard button is pressed */
	static bool IsKeyPressed(KeyCode keycode) noexcept;

public: /* Mouse */
	/** Returns true if the mouse button is pressed */
	static bool IsMouseButtonPressed(MouseCode button) noexcept;

	/** Returns x,y coords of the mouse positon relative to application's window */
	static std::pair<f32, f32> GetMousePosition() noexcept;

	/** Returns x coord of the mouse positon relative to application's window */
	static f32 GetMouseX() noexcept;

	/** Returns y coord of the mouse positon relative to application's window */
	static f32 GetMouseY() noexcept;
};

NS_ENIGMA_END


#endif // !ENIGMA_INPUT_H
