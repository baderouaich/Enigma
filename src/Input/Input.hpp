#pragma once
#ifndef ENIGMA_INPUT_H
#define ENIGMA_INPUT_H

#include <Core/Core.hpp>

// Keyboard
#include <Input/Keyboard/KeyCodes.hpp>
// Mouse
#include <Input/Mouse/MouseCodes.hpp>

#include <vector>

NS_ENIGMA_BEGIN

/** Input static class to gather input information at runtime like Mouse position, Is a keyboard button pressed.. */
class Input {
  public: /* Keyboard */
    /** Returns true if the keyboard button is pressed */
    static bool IsKeyPressed(const KeyCode keycode) noexcept;

    /** Returns true if the keyboard buttons are pressed */
    static bool AreKeysPressed(const std::initializer_list<KeyCode>& keycodes) noexcept;

  public: /* Mouse */
    /** Returns true if the mouse button is pressed */
    static bool IsMouseButtonPressed(const MouseCode button) noexcept;

    /** Returns x,y coords of the mouse positon relative to application's window */
    static std::pair<float, float> GetMousePosition() noexcept;

    /** Returns x coord of the mouse positon relative to application's window */
    static float GetMouseX() noexcept;

    /** Returns y coord of the mouse positon relative to application's window */
    static float GetMouseY() noexcept;
};

NS_ENIGMA_END


#endif // !ENIGMA_INPUT_H
