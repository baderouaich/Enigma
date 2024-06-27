#pragma once
#ifndef ENIGMA_MOUSE_CODES_H
#define ENIGMA_MOUSE_CODES_H

#include <Core/Core.hpp>
#include <GLFW/glfw3.h>

NS_ENIGMA_BEGIN

/** Mouse codes From glfw3.h */
typedef enum class MouseCode : ui16
{
	Button0 = GLFW_MOUSE_BUTTON_1,
	Button1 = GLFW_MOUSE_BUTTON_2,
	Button2 = GLFW_MOUSE_BUTTON_3,
	Button3 = GLFW_MOUSE_BUTTON_4,
	Button4 = GLFW_MOUSE_BUTTON_5,
	Button5 = GLFW_MOUSE_BUTTON_6,
	Button6 = GLFW_MOUSE_BUTTON_7,
	Button7 = GLFW_MOUSE_BUTTON_8,

	ButtonLast = Button7,
	ButtonLeft = Button0,
	ButtonRight = Button1,
	ButtonMiddle = Button2
} Mouse;

inline std::ostream& operator<<(std::ostream& os, const MouseCode& keyCode)
{
	return os << static_cast<ui16>(keyCode);
}


#define ENIGMA_MOUSE_BUTTON_0      ::Enigma::Mouse::Button0
#define ENIGMA_MOUSE_BUTTON_1      ::Enigma::Mouse::Button1
#define ENIGMA_MOUSE_BUTTON_2      ::Enigma::Mouse::Button2
#define ENIGMA_MOUSE_BUTTON_3      ::Enigma::Mouse::Button3
#define ENIGMA_MOUSE_BUTTON_4      ::Enigma::Mouse::Button4
#define ENIGMA_MOUSE_BUTTON_5      ::Enigma::Mouse::Button5
#define ENIGMA_MOUSE_BUTTON_6      ::Enigma::Mouse::Button6
#define ENIGMA_MOUSE_BUTTON_7      ::Enigma::Mouse::Button7
#define ENIGMA_MOUSE_BUTTON_LAST   ::Enigma::Mouse::ButtonLast
#define ENIGMA_MOUSE_BUTTON_LEFT   ::Enigma::Mouse::ButtonLeft
#define ENIGMA_MOUSE_BUTTON_RIGHT  ::Enigma::Mouse::ButtonRight
#define ENIGMA_MOUSE_BUTTON_MIDDLE ::Enigma::Mouse::ButtonMiddle

NS_ENIGMA_END

#endif // !ENIGMA_MOUSE_CODES_H
