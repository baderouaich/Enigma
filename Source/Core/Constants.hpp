#pragma once
#ifndef ENIGMA_CONSTANTS_H
#define ENIGMA_CONSTANTS_H

#include "Types.hpp"

namespace Enigma
{
	namespace Constants
	{
		namespace Algorithm
		{
			// GCM is defined for block ciphers with a block size of 128 bits. https://en.m.wikipedia.org/wiki/Galois/Counter_Mode

			namespace AES
			{
				//https://www.cryptopp.com/wiki/GCM_Mode
				static constexpr const ui16 AES_MINIMUM_PASSWORD_LENGTH = 9; // at least 6 to 64 characters
				//No max password check since we using KDF SHA-256, his allows you to use a password smaller or larger than the cipher's key size: https://crypto.stackexchange.com/questions/68299/length-of-password-requirement-using-openssl-aes-256-cbc
			}
			namespace ChaCha
			{
				//https://www.cryptopp.com/wiki/ChaCha20
				static constexpr const ui16 CHACHA_MINIMUM_PASSWORD_LENGTH = 9; // at least 6 to 64 characters
				//No max password check since we using KDF SHA-256, his allows you to use a password smaller or larger than the cipher's key size: https://crypto.stackexchange.com/questions/68299/length-of-password-requirement-using-openssl-aes-256-cbc
			}
			namespace TripleDES
			{
				//https://www.cryptopp.com/wiki/TripleDES
				static constexpr const ui16 TRIPLEDES_MINIMUM_PASSWORD_LENGTH = 9; // at least 6 to 64 characters
			}
		}

		namespace CLI
		{
			constexpr const char* CLI_HELP_MESSAGE = "say --help or -h for details";
		}

		namespace Utility
		{
			static constexpr const byte BASE64_CHARACTERS[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
		}

		namespace Resources
		{
			namespace Textures
			{
				// window icon
				static constexpr const auto ENIGMA_LOGO_PNG_PATH = "./Resources/Branding/Wolf2.png";
			}

			namespace Fonts
			{
				static constexpr const auto AUDIOWIDE_FONT_PATH = "./Resources/Fonts/Audiowide-Regular.ttf";
				static constexpr const auto MONTSERRAT_FONT_PATH = "./Resources/Fonts/Montserrat-Medium.ttf";
			}
		}

		namespace Colors
		{
			static constexpr const auto COLOR4I_TO_COLOR4F = [](const f32& r, const f32& g, const f32& b, const f32& a) -> Color4f
			{
				return Color4f{ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f };
			};

			static const Color4f BACKGROUND_COLOR = COLOR4I_TO_COLOR4F(30, 37, 43, 255);

			static const Color4f TEXT_COLOR = COLOR4I_TO_COLOR4F(255, 255, 255, 225);

			static const Color4f BUTTON_COLOR = COLOR4I_TO_COLOR4F(31, 103, 145, 150);
			static const Color4f BUTTON_COLOR_HOVER = COLOR4I_TO_COLOR4F(31, 103, 145, 200);
			static const Color4f BUTTON_COLOR_ACTIVE = COLOR4I_TO_COLOR4F(31, 103, 145, 255); // PRESSED

			static const Color4f MENUBAR_BACKGROUND_COLOR = Color4f{1.0f, 1.0f, 1.0f, 0.06f}; // WHITE TRANSPARENT

		}

	}
}

#endif // !ENIGMA_CONSTANTS_H
