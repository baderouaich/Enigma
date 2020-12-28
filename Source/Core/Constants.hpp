#pragma once
#ifndef ENIGMA_CONSTANTS_H
#define ENIGMA_CONSTANTS_H

#include "Types.hpp"
#include <vector>

namespace Enigma
{
	namespace Constants
	{
		namespace Algorithm
		{
			// Notes:
			// GCM is defined for block ciphers with a block size of 128 bits. https://en.m.wikipedia.org/wiki/Galois/Counter_Mode
			// No max password check since we using KDF SHA-256, his allows you to use a password smaller or larger than the cipher's key size: https://crypto.stackexchange.com/questions/68299/length-of-password-requirement-using-openssl-aes-256-cbc
			
			static constexpr const size_t MINIMUM_PASSWORD_LENGTH = 9; // AT LEAST 9 CHARACTERS, FOR SECURITY REASONS.
			
			namespace AES
			{
				//https://www.cryptopp.com/wiki/GCM_Mode
			}
			namespace ChaCha20
			{
				//https://www.cryptopp.com/wiki/ChaCha20
			}
			namespace TripleDES
			{
				//https://www.cryptopp.com/wiki/TripleDES
			}
		}

		namespace CLI
		{
			constexpr const char* CLI_HELP_MESSAGE = "say -h or --help for details";
		}

		namespace ErrorMessages
		{
			static constexpr const auto WEAK_PASSWORD_ERROR_MESSAGE = "Password is too weak! consider using 9 characters or more including special characters like :!@#$%^&*()_+-=[]{};'\",./?\\";
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


		// Advices
		static const std::vector<std::string_view> NOTES
		{
			"Don't use the same password for all your encrypted data",
			"Keep a written copy of your encryption password and the algorithm name you used in a safe place",
			"For serious data, use strong algorithms like AES-GCM and TripleDES",
		};
	}
}

#endif // !ENIGMA_CONSTANTS_H
