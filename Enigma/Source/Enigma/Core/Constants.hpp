#pragma once
#ifndef ENIGMA_CONSTANTS_H
#define ENIGMA_CONSTANTS_H

#include "Types.hpp"
#include <math.h>
namespace Enigma
{
	namespace Constants
	{
		namespace Graphics
		{
			/*
			*	OpenGL Default Version (Consider using version 4.3 as minimum to work properly)
			*/
			static constexpr const ui16 OpenGL_MAJOR_VERSION = 4;
			static constexpr const ui16 OpenGL_MINOR_VERSION = 3;
		}

		namespace Fonts
		{
			static constexpr const auto ENIGMA_FONT_PATH = "./Resources/Fonts/Enigma.ttf";
			static constexpr const auto PIECES_OF_EIGHT_FONT_PATH = "./Resources/Fonts/Pieces of Eight.ttf";

		}


	}
}

#endif // !ENIGMA_CONSTANTS_H
