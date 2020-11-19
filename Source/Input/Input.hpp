#pragma once
#ifndef ENIGMA_INPUT_H
#define ENIGMA_INPUT_H

#include <Core/Core.hpp>

//Keyboard
#include <Input/Keyboard/KeyCodes.hpp>
//Mouse
#include <Input/Mouse/MouseCodes.hpp>

NS_ENIGMA_BEGIN

class ENIGMA_API Input
{
public: /* Keyboard */
	static bool IsKeyPressed(KeyCode keycode) noexcept;

public: /* Mouse */
	static bool IsMouseButtonPressed(MouseCode button) noexcept;
	static std::pair<f32, f32> GetMousePosition() noexcept;
	static f32 GetMouseX() noexcept;
	static f32 GetMouseY() noexcept;
};

NS_ENIGMA_END


#endif // !ENIGMA_INPUT_H
