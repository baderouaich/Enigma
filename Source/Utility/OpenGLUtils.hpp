#pragma once
#ifndef ENIGMA_OPENGL_UTILS_H
#define ENIGMA_OPENGL_UTILS_H
#include <Core/Core.hpp>
#include <glad/glad.h>

NS_ENIGMA_BEGIN

class ENIGMA_API OpenGLUtils
{
public:
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
			GLenum err = glGetError(); \
			if (err != GL_NO_ERROR) \
			{ \
				ENIGMA_ASSERT(false, OpenGLUtils::GetGLErrorEnumString(err)); \
			} \
		} while (false)
///

NS_ENIGMA_END

#endif // !ENIGMA_OPENGL_UTILS_H 
