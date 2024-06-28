#include <pch.hpp>
#include "Cursor.hpp"
#include <Logger/Logger.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

NS_ENIGMA_BEGIN

Cursor::Cursor() noexcept
	:
	m_mode(CursorMode::Arrow),
	m_GLFWcursor(nullptr)
{
}

Cursor::Cursor(CursorMode mode)
	:
	m_mode(mode),
	m_GLFWcursor(nullptr)
{
	/*
	Arrow		= GLFW_ARROW_CURSOR,
	IBeam		= GLFW_IBEAM_CURSOR,
	CrossHair	= GLFW_CROSSHAIR_CURSOR,
	Hand		= GLFW_HAND_CURSOR,
	HResize		= GLFW_HRESIZE_CURSOR,
	VResize		= GLFW_VRESIZE_CURSOR,
	*/
	switch (mode)
	{
	case Enigma::CursorMode::Arrow:
		m_GLFWcursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
		break;
	case Enigma::CursorMode::IBeam:
		m_GLFWcursor = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
		break;
	case Enigma::CursorMode::CrossHair:
		m_GLFWcursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
		break;
	case Enigma::CursorMode::Hand:
		m_GLFWcursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
		break;
	case Enigma::CursorMode::HResize:
		m_GLFWcursor = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
		break;
	case Enigma::CursorMode::VResize:
		m_GLFWcursor = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
		break;
	default:
		ENIGMA_ASSERT(false, "Unsupported Cursor Mode");
		break;
	}
	ENIGMA_ASSERT(m_GLFWcursor, "Could not Create Standard Cursor");
}

Cursor::Cursor(const fs::path& image_path, const std::int32_t xhot, const std::int32_t yhot)
	:
	m_mode(CursorMode::NONE)
{
  ENIGMA_ASSERT(fs::is_regular_file(image_path), "Invalid image path");
	std::int32_t width{}, height{}, channels{};
	stbi_set_flip_vertically_on_load(false);
	byte* pixels = stbi_load(image_path.string().c_str(), &width, &height, &channels, 4);
	ENIGMA_ASSERT(pixels, "Failed to load image");
	//the alpha channel isn't necessary
	//ENIGMA_ASSERT(channels == 4, "Image must be RGBA");

	std::array<GLFWimage, 1> images{};
	images[0].pixels = pixels;
	images[0].width = width;
	images[0].height = height;

	m_GLFWcursor = glfwCreateCursor(images.data(), xhot, yhot);
	ENIGMA_ASSERT(m_GLFWcursor, "Could not Create Image Cursor");

	stbi_image_free(pixels);
	pixels = nullptr;
}

void Cursor::Destroy()
{
	if (m_GLFWcursor) 
	{
		glfwDestroyCursor(m_GLFWcursor);
		m_GLFWcursor = nullptr;
	}
}

Cursor::~Cursor()
{
	this->Destroy();
}

NS_ENIGMA_END
