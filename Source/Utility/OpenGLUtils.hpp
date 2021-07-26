#pragma once
#ifndef ENIGMA_OPENGL_UTILS_H
#define ENIGMA_OPENGL_UTILS_H
#include <Core/Core.hpp>
#include <glad/glad.h>
#include <cstring>

// NVX_gpu_memory_info extension defines from https://www.khronos.org/registry/OpenGL/extensions/NVX/NVX_gpu_memory_info.tx
#define GL_GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX          0x9047
#define GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX    0x9048
#define GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX  0x9049
#define GL_GPU_MEMORY_INFO_EVICTION_COUNT_NVX            0x904A
#define GL_GPU_MEMORY_INFO_EVICTED_MEMORY_NVX            0x904B


NS_ENIGMA_BEGIN

class ENIGMA_API OpenGLUtils final
{
	ENIGMA_STATIC_CLASS(OpenGLUtils);
public:
	/*
	*	Checks wether an extension available for example: GL_NVX_gpu_memory_info
	*	returns extension index of glGetStringi() if found, -1 otherwise
	*/
	static constexpr const GLint IsExtensionSupported(const char* extension_name) noexcept
	{	
		GLint ext_count{};
		glGetIntegerv(GL_NUM_EXTENSIONS, &ext_count);
		if (ext_count < 1)
			return -1;

		for (GLint i = 0; i < ext_count; ++i)
		{
			const char* extension = reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i));
			if (std::strcmp(extension, extension_name) == 0)
			{
				// Found
				return i;
			}
		}
		// Not found
		return -1;
	}

	/*
	*	Convert GL Error Enum macro name to a String, returns "Unknown GLenum" on non detected macros 
	*/
	static constexpr const char* GetGLErrorEnumString(const GLenum& _enum) noexcept
	{
#define CASE_ENUM(e) case e: return #e
		switch (_enum)
		{
			// errors
			CASE_ENUM(GL_NONE);
			CASE_ENUM(GL_INVALID_ENUM);
			CASE_ENUM(GL_INVALID_VALUE);
			CASE_ENUM(GL_INVALID_OPERATION);
			CASE_ENUM(GL_INVALID_FRAMEBUFFER_OPERATION);
			CASE_ENUM(GL_OUT_OF_MEMORY);
			default: return "Unknown GLenum";
		};
#undef CASE_ENUM
	}
};


/// glAssert to handle opengl calls errors (for opengl versions less than 4.3 with no error callback func to handle errors)
#define glAssert(call) \
		do \
		{ \
			(call); \
			const GLenum err = glGetError(); \
			if (err != GL_NO_ERROR) \
			{ \
				ENIGMA_ASSERT(false, OpenGLUtils::GetGLErrorEnumString(err)); \
			} \
		} while (false)
///

NS_ENIGMA_END

#endif // !ENIGMA_OPENGL_UTILS_H 
