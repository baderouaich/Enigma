#pragma once
#include "Types.hpp"
#include <array>

namespace Enigma {
  namespace Constants {
    using namespace std::string_literals; // using std::operator""s;

    static constexpr const ui16 ENIGMA_SINGLE_PROCESS_UNIQUE_PORT = 36363; // unique app socket port used to avoid multiple instances
    //static constexpr const auto ENIGMA_PACKAGE_NAME = "com.badereddineouaich.enigma"; // unique app id used to avoid multiple instances
    //static constexpr const auto ENIGMA_MAGIC_NUMBER = 0x454e49474d41; // "ENIGMA"

    namespace Algorithm {
      // Notes:
      // GCM is defined for block ciphers with a block size of 128 bits. https://en.m.wikipedia.org/wiki/Galois/Counter_Mode
      // No max password check since we using KDF SHA-256, his allows you to use a password smaller or larger than the cipher's key size: https://crypto.stackexchange.com/questions/68299/length-of-password-requirement-using-openssl-aes-256-cbc
      // Why GCM & EAX Modes: https://crypto.stackexchange.com/questions/18860/choice-of-authenticated-encryption-mode-for-whole-messages
      // https://i.stack.imgur.com/0hNsK.jpg

      static constexpr const size_t MINIMUM_PASSWORD_LENGTH = 6; // AT LEAST 6 CHARACTERS, FOR SECURITY REASONS.
      static constexpr const char *SPECIAL_CHARACTERS = "!\"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~";

      namespace AES {
        //https://www.cryptopp.com/wiki/GCM_Mode
      }
      namespace ChaCha20 {
        // The ChaCha20 algorithm is a stream cipher using a key of 256 bit size.
        // the ChaCha20 stream cipher does not have a block operation mode therefore it does not use padding.
        //https://www.cryptopp.com/wiki/ChaCha20
      }
      namespace TripleDES {
        //https://www.cryptopp.com/wiki/TripleDES
      }
      namespace Twofish {
        //https://www.cryptopp.com/wiki/TWOFISH
      }
      namespace IDEA {
        //https://www.cryptopp.com/wiki/IDEA
      }
    }

    namespace Config {
      static const fs::path WINDOW_CONFIG_FILE_PATH = fs::path(CONFIG_DIR) / "WindowSettings.ini";
    }

    namespace ENV // Environment Variable
    {
      static constexpr const char *ENIGMA_DIR_ENV_KEY = "ENIGMA_DIR"; // Holds the directory path of Enigma
    }

    namespace Logger {
      static const fs::path LOG_FILE_PATH = fs::path(LOGS_DIR) / "Enigma.log";
    }

    namespace Database {
      static const fs::path DATABASE_FILE_PATH = fs::path(DATABASE_DIR) / "Enigma.db";
      // Unfortunately, u cant create multiple tables at once..
      static constexpr const std::array<std::string_view, 2> CREATE_TABLES_SQL =
        {
          // Encryptions table
          R"(
					CREATE TABLE IF NOT EXISTS Encryption
					(
						ide		INTEGER PRIMARY KEY AUTOINCREMENT,
						title		VARCHAR(255) NOT NULL,
						date_time	DATETIME NOT NULL,
						size		INTEGER NOT NULL,			-- size of compressed cipher in bytes
						is_file		BOOLEAN NOT NULL,
						file_ext	VARCHAR(9),				-- file extension to remember file type on decryption

						CHECK(LENGTH(title) <= 255)				-- check title length <= 255
					);
				)",

          // Ciphers table holds either cipher text or file binary cipher for optimization
          R"(
					CREATE TABLE IF NOT EXISTS Cipher
					(
						idc		INTEGER PRIMARY KEY AUTOINCREMENT,
						data		BLOB NOT NULL,				-- cipher text or file binary cipher compressed             
						ide		INTEGER,

						FOREIGN KEY(ide) REFERENCES Encryption(ide) ON DELETE CASCADE --  when an Encryption record is deleted, all associated Cipher records will also be auto deleted.
					);
				)"};

    }

#if TRANSLATION_ENABLED
    namespace Translation {
      static const fs::path TRANSLATION_DIR = "Translation";
      static const fs::path TRANSLATION_CURRENT_FILE_PATH = fs::path(TRANSLATION_DIR) / ".current"; // application selected language to remember
      static const fs::path TRANSLATION_ENGLISH_PATH = fs::path(TRANSLATION_DIR) / "English.ini";
    }
#endif

    namespace CLI {
      static constexpr const char *CLI_HELP_MESSAGE = "Say -h or --help to display available options";
    }

    namespace Links {
      static const auto ENIGMA_GITHUB_REPOSITORY = "https://github.com/baderouaich/Enigma"s;
      static const auto ENIGMA_GITHUB_REPOSITORY_ISSUES = ENIGMA_GITHUB_REPOSITORY + "/issues"s;
      static const auto ENIGMA_GITHUB_REPOSITORY_PULL_REQUESTS = ENIGMA_GITHUB_REPOSITORY + "/pulls"s;
      static const auto ENIGMA_GITHUB_API = "https://api.github.com/repos/baderouaich/Enigma"s;
      static const auto ENIGMA_GITHUB_API_LATEST_RELEASE = ENIGMA_GITHUB_API + "/releases/latest"s;
    }

    namespace Resources {
      namespace Textures {
        // window runtime icon
        static const fs::path ENIGMA_LOGO_PNG_PATH = fs::path(BRANDING_DIR) / "Logo.png";
      }

      namespace Fonts {
        static const fs::path AUDIOWIDE_FONT_PATH = fs::path(FONTS_DIR) / "Audiowide-Regular.ttf";
        static const fs::path MONTSERRAT_FONT_PATH = fs::path(FONTS_DIR) / "Montserrat-Medium.ttf";
        static const fs::path UBUNTU_FONT_PATH = fs::path(FONTS_DIR) / "Ubuntu-Regular.ttf";
      }
    }

    namespace Colors {
      static constexpr const auto COLOR4I_TO_COLOR4F = [](const f32& r, const f32& g, const f32& b, const f32& a) -> Color4f {
        return Color4f{r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f};
      };

      static const Color4f RED(1.0f, 0.0f, 0.0f, 1.0f);
      static const Color4f GREEN(0.0f, 1.0f, 0.0f, 1.0f);
      static const Color4f BLUE(0.0f, 0.0f, 1.0f, 1.0f);
      static const auto ORANGE = COLOR4I_TO_COLOR4F(255, 182, 0, 255);

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

      static const Color4f TOOLS_BUTTON_COLOR = COLOR4I_TO_COLOR4F(25, 101, 130, 255);
      static const Color4f TOOLS_BUTTON_COLOR_HOVER = COLOR4I_TO_COLOR4F(48, 146, 184, 255);
      static const Color4f TOOLS_BUTTON_COLOR_ACTIVE = COLOR4I_TO_COLOR4F(60, 151, 186, 255);

      static const Color4f LOADING_SPINNER_COLOR = BACK_BUTTON_COLOR;
      //static const Color4f LOADING_BACKGROUND_COLOR = { BACKGROUND_COLOR.x, BACKGROUND_COLOR.y, BACKGROUND_COLOR.z, 0.6f };
      static const Color4f LOADING_BACKGROUND_COLOR = {0.0f, 0.0f, 0.0f, 0.8f};


      static const Color4f& SCENE_TITLE_BACKGROUND_COLOR = MENUBAR_BACKGROUND_COLOR;


    }
  }
}
