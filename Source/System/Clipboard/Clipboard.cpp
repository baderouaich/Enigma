#include <pch.hpp>
#include "Clipboard.hpp"

#include <GLFW/glfw3.h> 
#include <Application/Application.hpp>

NS_ENIGMA_BEGIN

const String Clipboard::Get()
{
	auto glfw_window = Application::GetInstance()->GetWindow()->GetGLFWwindow();
	return glfwGetClipboardString(glfw_window);
}

void Clipboard::Set(const String& buffer)
{
	auto glfw_window = Application::GetInstance()->GetWindow()->GetGLFWwindow();
	glfwSetClipboardString(glfw_window, buffer.c_str());
}

NS_ENIGMA_END

