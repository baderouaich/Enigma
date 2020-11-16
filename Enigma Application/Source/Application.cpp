#include "pch.hpp"	// Precompiled Headers
#include <Enigma.hpp>
using namespace Enigma;

#include "Scenes/MainMenuScene.hpp"

#include <cryptlib.h>
#include <filters.h>
#include <files.h>
#include <modes.h>
#include <hex.h>
#include <aes.h>

class AppDelegate : public Enigma::Application
{
public:
	AppDelegate(const WindowSettings& window_settings)
		:
	Enigma::Application(window_settings)
	{	
		LOG(ENIGMA_CURRENT_FUNCTION);
		CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption e;
        ENIGMA_INFO(e.AlgorithmName().c_str());
        testEncryptAES();

		//Window
		{
			const auto& window = __super::GetWindow();
			window->SetCursor(Enigma::CursorMode::Arrow);
			//window->SetIcon("Resources/Textures/EnigmaLogo.png");
			//window->SetCursor("Resources/Textures/Cursor.png", 0, 0);
            
		}


		//Push Main Menu scene
		__super::PushScene(new MainMenuScene());
	}

    void testEncryptAES()
    {

        //Key and IV setup
        //AES encryption uses a secret key of a variable length (128-bit, 196-bit or 256-   
        //bit). This key is secretly exchanged between two parties before communication   
        //begins. DEFAULT_KEYLENGTH= 16 bytes
        CryptoPP::byte key[CryptoPP::AES::DEFAULT_KEYLENGTH], iv[CryptoPP::AES::BLOCKSIZE];
        memset(key, 0x00, CryptoPP::AES::DEFAULT_KEYLENGTH);
        memset(iv, 0x00, CryptoPP::AES::BLOCKSIZE);

        //
        // String and Sink setup
        //
        std::string plaintext = "Now is the time for all good men to come to the aide...";
        std::string ciphertext;
        std::string decryptedtext;

        //
        // Dump Plain Text
        //
        std::cout << "Plain Text (" << plaintext.size() << " bytes)" << std::endl;
        std::cout << plaintext;
        std::cout << std::endl << std::endl;

        //
        // Create Cipher Text
        //
        CryptoPP::AES::Encryption aesEncryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
        CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption(aesEncryption, iv);

        CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink(ciphertext));
        stfEncryptor.Put(reinterpret_cast<const unsigned char*>(plaintext.c_str()), plaintext.length());
        stfEncryptor.MessageEnd();

        //
        // Dump Cipher Text
        //
        std::cout << "Cipher Text (" << ciphertext.size() << " bytes)" << std::endl;

        for (int i = 0; i < ciphertext.size(); i++) {

            std::cout << "0x" << std::hex << (0xFF & static_cast<CryptoPP::byte>(ciphertext[i])) << " ";
        }

        std::cout << std::endl << std::endl;

        //
        // Decrypt
        //
        CryptoPP::AES::Decryption aesDecryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
        CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption(aesDecryption, iv);

        CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink(decryptedtext));
        stfDecryptor.Put(reinterpret_cast<const unsigned char*>(ciphertext.c_str()), ciphertext.size());
        stfDecryptor.MessageEnd();

        //
        // Dump Decrypted Text
        //
        std::cout << "Decrypted Text: " << std::endl;
        std::cout << decryptedtext;
        std::cout << std::endl << std::endl;
    }

	virtual ~AppDelegate()
	{
		LOG(ENIGMA_CURRENT_FUNCTION);
	}
};


/*
*	Entry Point
*/
extern Enigma::UniquePtr<Enigma::Application> Enigma::CreateApplication()
{ 
	//Construct Window
	Enigma::WindowSettings window_settings;
	window_settings.title = "Enigma";
	window_settings.width = 640;
	window_settings.height = 480;
	//window_settings.minimum_width = 1280 - 100;
	//window_settings.minimum_height = 720 - 100;
	//window_settings.maximum_width = 1280 + 100;
	//window_settings.maximum_height = 720 + 100;
	window_settings.is_vsync = true;
	window_settings.is_resizable = false;
	window_settings.gl_major_version = 4;
	window_settings.gl_minor_version = 3;

	return Enigma::MakeUnique<AppDelegate>(std::move(window_settings));
}


