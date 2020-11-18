#include "Epch.hpp"
#include "AESDecryption.hpp"

#include "Enigma/System/Dialogs/MessageBox.hpp"
#include "Enigma/Utility/Base64.hpp"

NS_ENIGMA_BEGIN
using namespace CryptoPP;

AESDecryption::AESDecryption()
{
    m_aes_decryption = MakeUnique<CFB_Mode<AES>::Decryption>();
}


String AESDecryption::Decrypt(const String& password, const String& buffer)
{
    // Split IV and Cipher from buffer
    String iv = buffer.substr(0, AES::BLOCKSIZE);
    String encrypted = buffer.substr(AES::BLOCKSIZE, buffer.size() - 1);
    // Final decrypted buffer
    String decrypted; 
    try
    {
        // Prepare Key
        CryptoPP::SecByteBlock key(AES::MAX_KEYLENGTH + AES::BLOCKSIZE);
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
        m_aes_decryption->SetKeyWithIV(key, AES::MAX_KEYLENGTH, key + AES::MAX_KEYLENGTH); // key, kl, iv, ivl

        // Decrypt
        StringSource decryptor(encrypted, true,
            new StreamTransformationFilter(*m_aes_decryption,
                new StringSink(decrypted))
        );
        //NOTE: StringSource will auto clean the allocated memory

    }
    catch (const CryptoPP::Exception& e)
    {
        ENIGMA_CORE_ERROR("{0}", e.what());

        Enigma::MessageBox msg_box("AES Decryption Failure", e.what(), Enigma::MessageBox::Icon::Error, Enigma::MessageBox::Choice::Ok);
        UNUSED(msg_box.Show());
    }

    return decrypted;
}



AESDecryption::~AESDecryption()
{

}

NS_ENIGMA_END

