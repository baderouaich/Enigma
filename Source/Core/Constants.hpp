#pragma once
#ifndef ENIGMA_CONSTANTS_H
#define ENIGMA_CONSTANTS_H

#include "Types.hpp"
#include <array>

namespace Enigma
{
	/*
	* Notes
	* Raw Text:
	- When encrypting a raw text, cipher is encoded to Base64 to make it readable and copyable to user.
	- When decrypting a raw text, Base64 text is decoded to cipher, then its decrypted to recovered text.
	* Binary:
	- infile can be either encrypted file to be recovered, or a new file to be encrypted
	- outfile can be either encrypted file or recovered file.
	- When encrypting a file, cipher is outputed in binary to the outfile specified by the user without any encoding.
	- When decrypting a file, recovered binary is outputed to outfile specified by the user.

	Crypto++ Important Usage Notes *

	 - If a constructor for A takes a pointer to an object B (except primitive types such as int and char),
	 then A owns B and will delete B at A's destruction. If a constructor for A takes a reference to an object B,
	 then the caller retains ownership of B and should not destroy it until A no longer needs it.

	 - Crypto++ is thread safe at the class level. This means you can use Crypto++ safely in a multithreaded
	 application, but you must provide synchronization when multiple threads access a common Crypto++ object.
	*/
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
				// The ChaCha20 algorithm is a stream cipher using a key of 256 bit size.
				// the ChaCha20 stream cipher does not have a block operation mode therefore it does not use padding.
				//https://www.cryptopp.com/wiki/ChaCha20
			}
			namespace TripleDES
			{
				//https://www.cryptopp.com/wiki/TripleDES
			}
			namespace Twofish
			{
				//https://www.cryptopp.com/wiki/TWOFISH
			}
			namespace IDEA
			{
				//https://www.cryptopp.com/wiki/IDEA
			}
		}

		namespace Config
		{
			static constexpr const char* ENIGMA_WINDOW_SETTINGS_CONFIG_PATH = "./Resources/Config/WindowSettings.ini";
		}

		namespace ENV // Environment Variable
		{
			static constexpr const char* ENIGMA_DIR_ENV_KEY = "ENIGMA_DIR"; // Holds the directory path of Enigma
		}

		namespace Database
		{
			//https://www.tutorialspoint.com/sqlite/sqlite_data_types.htm
			static constexpr const char* DATABASE_FILE_DIR = "./Resources/Database/";
			static constexpr const char* DATABASE_FILE_NAME = "./Resources/Database/Enigma.db";
			// Unfortunetly u cant create multiple tables at once..
			static constexpr const std::array<const char*, 2> CREATE_TABLES_SQL =
			{
				// Encryptions table
				R"(
					CREATE TABLE IF NOT EXISTS Encryption
					(
						ide				INTEGER PRIMARY KEY AUTOINCREMENT,
						title			VARCHAR(255) NOT NULL,
						date_time		DATETIME NOT NULL,
						size			INTEGER NOT NULL,   -- size of compressed cipher in bytes
						is_file			BOOLEAN NOT NULL,

						CHECK(LENGTH(title) <= 255) -- check title length <= 255
					);
				)",

				// Ciphers table holds either cipher text or file binary cipher for optimization
				R"(
					CREATE TABLE IF NOT EXISTS Cipher
					(
						idc				INTEGER PRIMARY KEY AUTOINCREMENT,
						data			BLOB NOT NULL, -- cipher text or file binary cipher compressed             
						ide				INTEGER,

						FOREIGN KEY(ide) REFERENCES Encryption(ide) ON DELETE CASCADE --  when an Encryption record is deleted, all associated Cipher records will also be auto deleted.
					);
				)"
			};
			
			/*
			static constexpr const char* CREATE_TABLES_QUERY =
			R"(
			-- Encryptions table
			CREATE TABLE IF NOT EXISTS Encryptions
			(
				id              INTEGER PRIMARY KEY AUTOINCREMENT,
				title           VARCHAR(255) NOT NULL,
				cipher			BLOB NOT NULL, -- cipher text or file binary cipher compressed             
				date_time       DATETIME NOT NULL,
				is_file			BOOLEAN NOT NULL
			);
			)";
			*/
	}

		namespace CLI
		{
			static constexpr const char* CLI_HELP_MESSAGE = "Say -h or --help to display available options";
		}

		namespace ErrorMessages
		{
			static const String WEAK_PASSWORD = String("Password is too weak! consider using ") + std::to_string(Constants::Algorithm::MINIMUM_PASSWORD_LENGTH) + " characters or more including special characters like :!@#$%^&*()_+-=[]{};'\",./?\\";
		}

		namespace Links
		{
			static const String ENIGMA_GITHUB_API = "https://api.github.com/repos/BaderEddineOuaich/Enigma";
			static const String ENIGMA_GITHUB_API_LATEST_RELEASE = ENIGMA_GITHUB_API + "/releases/latest";
			static const String ENIGMA_GITHUB_REPOSITORY = "https://github.com/BaderEddineOuaich/Enigma";
		}

		namespace Resources
		{
			namespace Textures
			{
				// window runtime icon
				static constexpr const char* ENIGMA_LOGO_PNG_PATH = "./Resources/Branding/Wolf.png";
			}

			namespace Fonts
			{
				static constexpr const char* AUDIOWIDE_FONT_PATH = "./Resources/Fonts/Audiowide-Regular.ttf";
				static constexpr const char* MONTSERRAT_FONT_PATH = "./Resources/Fonts/Montserrat-Medium.ttf";
			}
		}

		namespace Colors
		{
			static constexpr const auto COLOR4I_TO_COLOR4F = [](const f32& r, const f32& g, const f32& b, const f32& a) -> Color4f
			{
				return Color4f{ r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f };
			};

#if 0
			static const Color4f BACKGROUND_COLOR = COLOR4I_TO_COLOR4F(12, 26, 50, 200);

			static const Color4f TEXT_COLOR = COLOR4I_TO_COLOR4F(216, 174, 123, 225);
			static const Color4f ERROR_TEXT_COLOR = COLOR4I_TO_COLOR4F(201, 46, 101, 255);
			static const Color4f PASSWORD_MATCH_TEXT_COLOR = COLOR4I_TO_COLOR4F(94, 172, 161, 225);


			static const Color4f BUTTON_COLOR = COLOR4I_TO_COLOR4F(167, 114, 63, 200);
			static const Color4f BUTTON_COLOR_HOVER = COLOR4I_TO_COLOR4F(202, 154, 91, 200);
			static const Color4f BUTTON_COLOR_ACTIVE = COLOR4I_TO_COLOR4F(229, 185, 121, 200); // PRESSED

			static const Color4f MENUBAR_BACKGROUND_COLOR = COLOR4I_TO_COLOR4F(43, 132, 202, 100); // WHITE TRANSPARENT
#endif
#if 0
			static const Color4f BACKGROUND_COLOR = COLOR4I_TO_COLOR4F(50, 51, 74, 255);

			static const Color4f TEXT_COLOR = COLOR4I_TO_COLOR4F(242, 242, 243, 225);
			static const Color4f ERROR_TEXT_COLOR = COLOR4I_TO_COLOR4F(201, 46, 101, 255);
			static const Color4f PASSWORD_MATCH_TEXT_COLOR = COLOR4I_TO_COLOR4F(94, 172, 161, 225);


			static const Color4f BUTTON_COLOR = COLOR4I_TO_COLOR4F(89, 101, 231, 255);
			static const Color4f BUTTON_COLOR_HOVER = COLOR4I_TO_COLOR4F(86, 195, 252, 255);
			static const Color4f BUTTON_COLOR_ACTIVE = COLOR4I_TO_COLOR4F(94, 172, 161, 255); // PRESSED

			static const Color4f MENUBAR_BACKGROUND_COLOR = COLOR4I_TO_COLOR4F(91, 103, 234, 200); // WHITE TRANSPARENT
#endif


#if  0
			//static const Color4f BACKGROUND_COLOR = COLOR4I_TO_COLOR4F(36, 54, 54, 255);
			static const Color4f BACKGROUND_COLOR = COLOR4I_TO_COLOR4F(30, 37, 43, 255);

			/*
			static const Color4f TEXT_COLOR = COLOR4I_TO_COLOR4F(255, 255, 255, 225);
			static const Color4f ERROR_TEXT_COLOR = COLOR4I_TO_COLOR4F(176, 39, 39, 255);
			static const Color4f PASSWORD_MATCH_TEXT_COLOR = COLOR4I_TO_COLOR4F(66, 158, 83, 225);
			*/
			static const Color4f TEXT_COLOR = COLOR4I_TO_COLOR4F(242, 242, 243, 225);
			static const Color4f ERROR_TEXT_COLOR = COLOR4I_TO_COLOR4F(201, 46, 64, 255);
			static const Color4f PASSWORD_MATCH_TEXT_COLOR = COLOR4I_TO_COLOR4F(94, 172, 161, 225);


			static const Color4f BUTTON_COLOR = COLOR4I_TO_COLOR4F(31, 103, 145, 150);
			static const Color4f BUTTON_COLOR_HOVER = COLOR4I_TO_COLOR4F(31, 103, 145, 200);
			static const Color4f BUTTON_COLOR_ACTIVE = COLOR4I_TO_COLOR4F(31, 103, 145, 255); // PRESSED

			static const Color4f MENUBAR_BACKGROUND_COLOR = Color4f{1.0f, 1.0f, 1.0f, 0.06f}; // WHITE TRANSPARENT
#endif
			static const Color4f BACKGROUND_COLOR = COLOR4I_TO_COLOR4F(43, 43, 43, 255); // brown dark gray
			//static const Color4f BACKGROUND_COLOR = COLOR4I_TO_COLOR4F(46, 53, 56, 255); // cyan dark gray
			//static const Color4f BACKGROUND_COLOR = COLOR4I_TO_COLOR4F(46, 51, 56, 255); // blue dark gray

			static const Color4f TEXT_COLOR = COLOR4I_TO_COLOR4F(242, 242, 243, 225);
			static const Color4f ERROR_TEXT_COLOR = COLOR4I_TO_COLOR4F(201, 46, 64, 255);
			static const Color4f PASSWORD_MATCH_TEXT_COLOR = COLOR4I_TO_COLOR4F(94, 172, 161, 225);


			static const Color4f BUTTON_COLOR = COLOR4I_TO_COLOR4F(10, 120, 122, 200);
			static const Color4f BUTTON_COLOR_HOVER = COLOR4I_TO_COLOR4F(29, 188, 191, 200);
			static const Color4f BUTTON_COLOR_ACTIVE = COLOR4I_TO_COLOR4F(11, 65, 66, 200); // PRESSSED

			static const Color4f MENUBAR_BACKGROUND_COLOR = COLOR4I_TO_COLOR4F(66, 66, 80, 200);

			static const Color4f BACK_BUTTON_COLOR = COLOR4I_TO_COLOR4F(209, 61, 86, 255);
			static const Color4f BACK_BUTTON_COLOR_HOVER = COLOR4I_TO_COLOR4F(232, 81, 107, 255);
			static const Color4f BACK_BUTTON_COLOR_ACTIVE = COLOR4I_TO_COLOR4F(150, 41, 59, 255);

			static const Color4f MY_ENCRYPTIONS_BUTTON_COLOR = COLOR4I_TO_COLOR4F(26, 72, 97, 255);
			static const Color4f MY_ENCRYPTIONS_BUTTON_COLOR_HOVER = COLOR4I_TO_COLOR4F(38, 91, 120, 255);
			static const Color4f MY_ENCRYPTIONS_BUTTON_COLOR_ACTIVE = COLOR4I_TO_COLOR4F(10, 132, 199, 255);

			static const Color4f LOADING_SPINNER_COLOR = BACK_BUTTON_COLOR;
			//static const Color4f LOADING_BACKGROUND_COLOR = { BACKGROUND_COLOR.x, BACKGROUND_COLOR.y, BACKGROUND_COLOR.z, 0.6f };
			static const Color4f LOADING_BACKGROUND_COLOR = {0.0f, 0.0f, 0.0f, 0.8f };


		}
	}
}

#endif // !ENIGMA_CONSTANTS_H
