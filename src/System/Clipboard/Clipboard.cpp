#include <pch.hpp>
#include "Clipboard.hpp"

#include <GLFW/glfw3.h> 
#include <Application/Application.hpp>

NS_ENIGMA_BEGIN

std::string Clipboard::Get()
{
	auto glfw_window = Application::getInstance()->GetWindow()->GetGLFWwindow();
	return std::string(glfwGetClipboardString(glfw_window));
}

void Clipboard::Set(const std::string& buffer)
{
	auto glfw_window = Application::getInstance()->GetWindow()->GetGLFWwindow();
	glfwSetClipboardString(glfw_window, buffer.c_str());
}

NS_ENIGMA_END

