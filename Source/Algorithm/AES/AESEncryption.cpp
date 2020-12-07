#include "pch.hpp"
#include "AESEncryption.hpp"
#include "System/Dialogs/MessageBox.hpp"
#include "Utility/Base64.hpp"

NS_ENIGMA_BEGIN

AESEncryption::AESEncryption()
    :
    m_aes_encryption(std::make_unique<CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption>()),
    m_auto_seeded_random_pool(std::make_unique<CryptoPP::AutoSeededRandomPool>())
{
}

String AESEncryption::Encrypt(const String& password, const String& buffer)
{
    // Make sure encryption mode and the seeder are initialized
    { 
        ENIGMA_ASSERT(m_aes_encryption, "AESEncryption is not initialized properly");
        ENIGMA_ASSERT(m_auto_seeded_random_pool, "AESEncryption is not initialized properly");
    }
    // Validate Arguments
    {
        // AES password length must be at least 9 for security reasons
        if (password.size() < Constants::Algorithm::AES::AES_MINIMUM_PASSWORD_LENGTH)
        {
            const String msg = "AES Minimum Password Length is " + std::to_string(Constants::Algorithm::AES::AES_MINIMUM_PASSWORD_LENGTH);
			//console alert
            ENIGMA_ERROR(msg.c_str());
			//ui alert
            Enigma::MessageBox msg_box("AES Encryption Failure", msg, Enigma::MessageBox::Icon::Error, Enigma::MessageBox::Choice::Ok);
			[[maybe_unused]] auto action = msg_box.Show();

            return String();
        }
    }

    String encrypted; // Final encrypted buffer
    String iv = this->GenerateRandomIV(); // Random generated 16 bytes IV
    try
    {
        using namespace CryptoPP;
        // Prepare Key
        SecByteBlock key(AES::MAX_KEYLENGTH + AES::BLOCKSIZE); // Encryption key to be generated from user password + IV
        HKDF<SHA256> hkdf;
        hkdf.DeriveKey(
            key, 
            key.size(),
            reinterpret_cast<const byte*>(password.data()),
            password.size(),
            reinterpret_cast<const byte*>(iv.data()),
            iv.size(),
            nullptr,
            0);
        m_aes_encryption->SetKeyWithIV(key, AES::MAX_KEYLENGTH, key + AES::MAX_KEYLENGTH); // key, kl, iv, ivl

        // Encrypt
        StringSource encryptor(buffer, true,
            new StreamTransformationFilter(*m_aes_encryption,
                new StringSink(encrypted))
        );
        //NOTE: StringSource will auto clean the allocated memory
    }
    catch (const CryptoPP::Exception& e)
    {
        ENIGMA_ERROR("{0}", e.what());

        Enigma::MessageBox msg_box("AES Encryption Failure", e.what(), Enigma::MessageBox::Icon::Error, Enigma::MessageBox::Choice::Ok);
		[[maybe_unused]] auto action = msg_box.Show();
    }

    return (iv + encrypted);
}

String AESEncryption::GenerateRandomIV()
{
    String iv(CryptoPP::AES::BLOCKSIZE, '\000');
    m_auto_seeded_random_pool->GenerateBlock(reinterpret_cast<byte*>(iv.data()), iv.size());
    return iv;
}

AESEncryption::~AESEncryption() noexcept
{
}

NS_ENIGMA_END

