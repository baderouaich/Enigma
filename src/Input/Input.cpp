#include "Input.hpp"
#include <pch.hpp>

#include <Application/Application.hpp>
#include <GLFW/glfw3.h>

NS_ENIGMA_BEGIN

bool Input::IsKeyPressed(const KeyCode keycode) noexcept {
  GLFWwindow *window = Application::getInstance()->GetWindow()->GetGLFWwindow();
  std::int32_t state = glfwGetKey(window, static_cast<std::int32_t>(keycode));
  return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::AreKeysPressed(const std::initializer_list<KeyCode>& keycodes) noexcept {
  for (const KeyCode k: keycodes)
    if (!IsKeyPressed(k))
      return false;
  return true;
}

bool Input::IsMouseButtonPressed(const MouseCode button) noexcept {

  GLFWwindow *window = Application::getInstance()->GetWindow()->GetGLFWwindow();
  std::int32_t state = glfwGetMouseButton(window, static_cast<std::int32_t>(button));
  return state == GLFW_PRESS;
}

std::pair<float, float> Input::GetMousePosition() noexcept {
  GLFWwindow *window = Application::getInstance()->GetWindow()->GetGLFWwindow();
  double xPos{0.0}, yPos{0.0};
  glfwGetCursorPos(window, &xPos, &yPos);
  return std::make_pair(static_cast<float>(xPos), static_cast<float>(yPos));
}

float Input::GetMouseX() noexcept {
  const auto [x, y] = GetMousePosition();
  return x;
}

float Input::GetMouseY() noexcept {
  const auto [x, y] = GetMousePosition();
  return y;
}
NS_ENIGMA_END
