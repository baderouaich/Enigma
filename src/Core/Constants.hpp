#pragma once
#include "Types.hpp"
#include <ResourceManager/ResourceManager.hpp>
#include <array>


namespace Enigma::Constants {
  using namespace std::string_literals;

  static constexpr std::uint16_t ENIGMA_SINGLE_PROCESS_UNIQUE_PORT = 36363; // unique app socket port used to avoid multiple instances
  //static constexpr auto ENIGMA_PACKAGE_NAME = "com.badereddineouaich.enigma"; // unique app id used to avoid multiple instances
  //static constexpr auto ENIGMA_MAGIC_NUMBER = 0x454e49474d41; // "ENIGMA"

  namespace Algorithm {
    // Notes:
    // GCM is defined for block ciphers with a block size of 128 bits. https://en.m.wikipedia.org/wiki/Galois/Counter_Mode
    // No max password check since we're using KDF SHA-256, his allows you to use a password smaller or larger than the cipher's key size: https://crypto.stackexchange.com/questions/68299/length-of-password-requirement-using-openssl-aes-256-cbc
    // Why GCM & EAX Modes: https://crypto.stackexchange.com/questions/18860/choice-of-authenticated-encryption-mode-for-whole-messages
    // https://i.stack.imgur.com/0hNsK.jpg

    static constexpr std::size_t MINIMUM_PASSWORD_LENGTH = 6; // AT LEAST 6 CHARACTERS, FOR SECURITY REASONS.
    static constexpr char SPECIAL_CHARACTERS[] = R"(!"#$%&'()*+,-./:;<=>?@[\]^_`{|}~)";

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

  namespace Database {
    static const fs::path DATABASE_FILE_PATH = ::Enigma::ResourceManager::getResourcesDir() / "database" / "Enigma.db";
    // Unfortunately, u cant create multiple tables at once..
    static constexpr std::array<std::string_view, 2> CREATE_TABLES_SQL = {
      R"(
					CREATE TABLE IF NOT EXISTS Encryptions
					(
						ide		    INTEGER PRIMARY KEY AUTOINCREMENT,
            algo		  INTEGER NOT NULL,			-- algorithm enum id used in encryption. e.g: AES=1 ..
						title		  VARCHAR(255) NOT NULL,
						date_time	DATETIME NOT NULL,
						size		  INTEGER NOT NULL,			-- size of compressed cipher in bytes
						is_file		BOOLEAN NOT NULL,
						file_ext	VARCHAR(9),				-- file extension to remember file type on decryption

						CHECK(LENGTH(title) <= 255)				-- check title length <= 255
					);
				)",

      R"(
            CREATE TABLE IF NOT EXISTS CipherChunks
            (
              idc	   INTEGER PRIMARY KEY AUTOINCREMENT,
              ide		 INTEGER NOT NULL,
              offset INTEGER NOT NULL,
              size   INTEGER NOT NULL,
              bytes	 BLOB NOT NULL,

              FOREIGN KEY(ide) REFERENCES Encryptions(ide) ON DELETE CASCADE --  when an Encryption record is deleted, all associated Cipher records will also be deleted automatically.
            );
        )"

    };

    static constexpr std::array<std::string_view, 1> CREATE_INDEXES_SQL = {
      "CREATE INDEX IF NOT EXISTS ix_Encryptions_title ON Encryptions(title);"};

  }

#if TRANSLATION_ENABLED
  namespace Translation {
    static const fs::path TRANSLATION_DIR = "Translation";
    static const fs::path TRANSLATION_CURRENT_FILE_PATH = fs::path(TRANSLATION_DIR) / ".current"; // application selected language to remember
    static const fs::path TRANSLATION_ENGLISH_PATH = fs::path(TRANSLATION_DIR) / "English.ini";
  }
#endif

  namespace CLI {
    static constexpr char CLI_HELP_MESSAGE[] = "Say -h or --help to display available options";
  }

  namespace Links {
    static const auto ENIGMA_GITHUB_REPOSITORY = "https://github.com/baderouaich/Enigma"s;
    static const auto ENIGMA_GITHUB_REPOSITORY_ISSUES = ENIGMA_GITHUB_REPOSITORY + "/issues"s;
    static const auto ENIGMA_GITHUB_REPOSITORY_PULL_REQUESTS = ENIGMA_GITHUB_REPOSITORY + "/pulls"s;
    static const auto ENIGMA_GITHUB_API = "https://api.github.com/repos/baderouaich/Enigma"s;
    static const auto ENIGMA_GITHUB_API_LATEST_RELEASE = ENIGMA_GITHUB_API + "/releases/latest"s;
  }

  namespace Resources {
    // namespace Textures {
    //   // window runtime icon
    //   static const fs::path ENIGMA_LOGO_PNG_PATH = ::Enigma::ResourceManager::getResourcesDir() / "branding" / "Logo.png";
    // }

    // namespace Fonts {
    //   static const fs::path AUDIOWIDE_FONT_PATH = ::Enigma::ResourceManager::getResourcesDir() / "fonts" / "Audiowide-Regular.ttf";
    //   static const fs::path MONTSERRAT_FONT_PATH = ::Enigma::ResourceManager::getResourcesDir() / "fonts" / "Montserrat-Medium.ttf";
    //   static const fs::path UBUNTU_FONT_PATH = ::Enigma::ResourceManager::getResourcesDir() / "fonts" / "Ubuntu-Regular.ttf";
    // }
  }


  namespace Colors {
    static constexpr auto COLOR4I_TO_COLOR4F = [](const float r, const float g, const float b, const float a) -> ImVec4 {
      return ImVec4{r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f};
    };
    
    static constexpr ImVec4 PRIMARY_900 =
      COLOR4I_TO_COLOR4F(15, 17, 21, 255); // near-black background

    static constexpr ImVec4 PRIMARY_800 =
      COLOR4I_TO_COLOR4F(20, 22, 28, 255); // elevated dark

    static constexpr ImVec4 PRIMARY_700 =
      COLOR4I_TO_COLOR4F(26, 29, 36, 255); // surface

    static constexpr ImVec4 PRIMARY_500 =
      COLOR4I_TO_COLOR4F(155, 163, 180, 255); // muted text

    static constexpr ImVec4 PRIMARY_100 =
      COLOR4I_TO_COLOR4F(232, 236, 243, 255); // primary text


    static constexpr ImVec4 ACCENT_500 =
      COLOR4I_TO_COLOR4F(79, 140, 255, 255); // main blue

    static constexpr ImVec4 ACCENT_400 =
      COLOR4I_TO_COLOR4F(102, 158, 255, 255); // hover blue

    static constexpr ImVec4 ACCENT_600 =
      COLOR4I_TO_COLOR4F(61, 120, 242, 255); // active blue


    static constexpr ImVec4 SUCCESS_500 =
      COLOR4I_TO_COLOR4F(51, 194, 127, 255);

    static constexpr ImVec4 WARNING_500 =
      COLOR4I_TO_COLOR4F(240, 178, 74, 255);

    static constexpr ImVec4 ERROR_500 =
      COLOR4I_TO_COLOR4F(224, 90, 90, 255);

    static constexpr ImVec4 BACKGROUND_COLOR = PRIMARY_900;
    static constexpr ImVec4 SURFACE_COLOR = PRIMARY_700;
    static constexpr ImVec4 SIDEBAR_COLOR = PRIMARY_800;
    static constexpr ImVec4 MENUBAR_COLOR = PRIMARY_800;

    static constexpr ImVec4 TEXT_PRIMARY = PRIMARY_100;
    static constexpr ImVec4 TEXT_SECONDARY = PRIMARY_500;

    static constexpr ImVec4 ACCENT = ACCENT_500;
    static constexpr ImVec4 ACCENT_HOVER = ACCENT_400;
    static constexpr ImVec4 ACCENT_ACTIVE = ACCENT_600;


    namespace {
      static constexpr auto COLOR4I_TO_COLOR4F = [](const float r, const float g, const float b, const float a) -> ImVec4 {
        return ImVec4{r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f};
      };
      // Main backgrounds
      static const ImVec4 BACKGROUND_COLOR = COLOR4I_TO_COLOR4F(15, 17, 21, 255); // Primary background

      static const ImVec4 SURFACE_COLOR = COLOR4I_TO_COLOR4F(26, 29, 36, 255); // Secondary surface

      static const ImVec4 SIDEBAR_BACKGROUND_COLOR =
        COLOR4I_TO_COLOR4F(20, 22, 28, 255); // Sidebar background

      static const ImVec4 MENUBAR_BACKGROUND_COLOR =
        COLOR4I_TO_COLOR4F(23, 25, 31, 255); // Menubar background

      // Accent
      static const ImVec4 ACCENT_COLOR =
        COLOR4I_TO_COLOR4F(79, 140, 255, 255); // Primary accent

      static const ImVec4 ACCENT_HOVERED_COLOR =
        COLOR4I_TO_COLOR4F(102, 158, 255, 255); // Hover accent

      static const ImVec4 ACCENT_ACTIVE_COLOR =
        COLOR4I_TO_COLOR4F(61, 120, 242, 255); // Active accent

      // Text
      static const ImVec4 TEXT_PRIMARY_COLOR =
        COLOR4I_TO_COLOR4F(232, 236, 243, 255); // Primary text

      static const ImVec4 TEXT_SECONDARY_COLOR =
        COLOR4I_TO_COLOR4F(155, 163, 180, 255); // Secondary text

      // States
      static const ImVec4 SUCCESS_COLOR =
        COLOR4I_TO_COLOR4F(51, 194, 127, 255); // Success green

      static const ImVec4 WARNING_COLOR =
        COLOR4I_TO_COLOR4F(240, 178, 74, 255); // Warning amber

      static const ImVec4 ERROR_COLOR =
        COLOR4I_TO_COLOR4F(224, 90, 90, 255); // Error red


      static const ImVec4 RED(1.0f, 0.0f, 0.0f, 1.0f);
      static const ImVec4 GREEN(0.0f, 1.0f, 0.0f, 1.0f);
      static const ImVec4 BLUE(0.0f, 0.0f, 1.0f, 1.0f);
      static const auto ORANGE = COLOR4I_TO_COLOR4F(255, 182, 0, 255);

      // static const ImVec4 BACKGROUND_COLOR = COLOR4I_TO_COLOR4F(15, 17, 21, 255); // brown dark gray
      // static const ImVec4 SIDEBAR_BACKGROUND_COLOR = COLOR4I_TO_COLOR4F(0,0,0,255); // Black
      //static const ImVec4 BACKGROUND_COLOR = COLOR4I_TO_COLOR4F(46, 53, 56, 255); // cyan dark gray
      //static const ImVec4 BACKGROUND_COLOR = COLOR4I_TO_COLOR4F(46, 51, 56, 255); // blue dark gray

      static const ImVec4 TEXT_COLOR = COLOR4I_TO_COLOR4F(242, 242, 243, 225);
      static const ImVec4 ERROR_TEXT_COLOR = COLOR4I_TO_COLOR4F(201, 46, 64, 255);
      static const ImVec4 PASSWORD_MATCH_TEXT_COLOR = COLOR4I_TO_COLOR4F(94, 172, 161, 225);


      static const ImVec4 BUTTON_COLOR = COLOR4I_TO_COLOR4F(10, 120, 122, 200);
      static const ImVec4 BUTTON_COLOR_HOVER = COLOR4I_TO_COLOR4F(29, 188, 191, 200);
      static const ImVec4 BUTTON_COLOR_ACTIVE = COLOR4I_TO_COLOR4F(11, 65, 66, 200); // PRESSSED

      // static const ImVec4 MENUBAR_BACKGROUND_COLOR = COLOR4I_TO_COLOR4F(66, 66, 80, 200);

      static const ImVec4 BACK_BUTTON_COLOR = COLOR4I_TO_COLOR4F(209, 61, 86, 255);
      static const ImVec4 BACK_BUTTON_COLOR_HOVER = COLOR4I_TO_COLOR4F(232, 81, 107, 255);
      static const ImVec4 BACK_BUTTON_COLOR_ACTIVE = COLOR4I_TO_COLOR4F(150, 41, 59, 255);

      static const ImVec4 MY_ENCRYPTIONS_BUTTON_COLOR = COLOR4I_TO_COLOR4F(26, 72, 97, 255);
      static const ImVec4 MY_ENCRYPTIONS_BUTTON_COLOR_HOVER = COLOR4I_TO_COLOR4F(38, 91, 120, 255);
      static const ImVec4 MY_ENCRYPTIONS_BUTTON_COLOR_ACTIVE = COLOR4I_TO_COLOR4F(10, 132, 199, 255);

      static const ImVec4 TOOLS_BUTTON_COLOR = COLOR4I_TO_COLOR4F(25, 101, 130, 255);
      static const ImVec4 TOOLS_BUTTON_COLOR_HOVER = COLOR4I_TO_COLOR4F(48, 146, 184, 255);
      static const ImVec4 TOOLS_BUTTON_COLOR_ACTIVE = COLOR4I_TO_COLOR4F(60, 151, 186, 255);

      static const ImVec4 LOADING_SPINNER_COLOR = BACK_BUTTON_COLOR;
      //static const ImVec4 LOADING_BACKGROUND_COLOR = { BACKGROUND_COLOR.x, BACKGROUND_COLOR.y, BACKGROUND_COLOR.z, 0.6f };
      static const ImVec4 LOADING_BACKGROUND_COLOR = {0.0f, 0.0f, 0.0f, 0.8f};


      static const ImVec4 &SCENE_TITLE_BACKGROUND_COLOR = MENUBAR_BACKGROUND_COLOR;


    }

  }

}
