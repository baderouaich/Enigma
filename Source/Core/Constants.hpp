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

			namespace AES
			{
				/*
				Modes: https://stackoverflow.com/questions/1220751/how-to-choose-an-aes-encryption-mode-cbc-ecb-ctr-ocb-cfb
				#ECB should not be used if encrypting more than one block of data with the same key.

				#CBC, OFB and CFB are similar, however OFB/CFB is better because you only need encryption and not decryption, which can save code space.

				#CTR is used if you want good parallelization (ie. speed), instead of CBC/OFB/CFB.

				#XTS mode is the most common if you are encoding a random accessible data (like a hard disk or RAM).

				#OCB is by far the best mode, as it allows encryption and authentication in a single pass. However there are patents on it in USA.
				*/
				static constexpr const ui16 AES_MINIMUM_PASSWORD_LENGTH = 9; // at least 6 to 64 characters
				//No max password check since we using KDF SHA-256, his allows you to use a password smaller or larger than the cipher's key size: https://crypto.stackexchange.com/questions/68299/length-of-password-requirement-using-openssl-aes-256-cbc
			}
			namespace ChaCha
			{
				static constexpr const ui16 CHACHA_MINIMUM_PASSWORD_LENGTH = 9; // at least 6 to 64 characters
				//No max password check since we using KDF SHA-256, his allows you to use a password smaller or larger than the cipher's key size: https://crypto.stackexchange.com/questions/68299/length-of-password-requirement-using-openssl-aes-256-cbc
			}

		}

		namespace Utility
		{
			static constexpr const byte BASE64_CHARACTERS[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
		}

		namespace Resources
		{
			namespace Textures
			{
				static constexpr const auto ENIGMA_LOGO_PNG_PATH = "./Resources/Branding/EnigmaLetterE.png";
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
