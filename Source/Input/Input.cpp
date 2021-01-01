#include <pch.hpp>
#include "Input.hpp"

#include <GLFW/glfw3.h>

NS_ENIGMA_BEGIN

bool Input::IsKeyPressed(KeyCode keycode) noexcept
{
	GLFWwindow* window = Application::GetInstance()->GetWindow()->GetGLFWwindow();
	i32 state = glfwGetKey(window, static_cast<i32>(keycode));
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMouseButtonPressed(MouseCode button) noexcept
{
	GLFWwindow* window = Application::GetInstance()->GetWindow()->GetGLFWwindow();
	i32 state = glfwGetMouseButton(window, static_cast<i32>(button));
	return state == GLFW_PRESS;
}

std::pair<f32, f32> Input::GetMousePosition() noexcept
{
	GLFWwindow* window = Application::GetInstance()->GetWindow()->GetGLFWwindow();
	f64 xPos{0.0}, yPos{0.0};
	glfwGetCursorPos(window, &xPos, &yPos);
	return std::make_pair( static_cast<f32>(xPos), static_cast<f32>(yPos) );
}

f32 Input::GetMouseX() noexcept
{
	const auto [x, y] = GetMousePosition();
	return x;
}

f32 Input::GetMouseY() noexcept
{
	const auto [x, y] = GetMousePosition();
	return y;
}
NS_ENIGMA_END
