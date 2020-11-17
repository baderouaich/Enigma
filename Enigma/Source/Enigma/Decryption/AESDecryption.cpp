#include "Epch.hpp"
#include "AESDecryption.hpp"

#include "Enigma/System/Dialogs/MessageBox.hpp"
#include "Enigma/Utility/Base64.hpp"

NS_ENIGMA_BEGIN

AESDecryption::AESDecryption()
    :
    m_aes_decryption(nullptr)
{
}

String AESDecryption::Decrypt(const String& base64_cipher)
{
#if 0
    //ExtractIVAndKeyAndEncryptedBufferFromBase64Cipher();
    //iv, key, cipher, buffer
    try
    {
        CTR_Mode<AES>::Decryption decryption;
        decryption.SetKeyWithIV(key, AES::MAX_KEYLENGTH, key + AES::MAX_KEYLENGTH);

        StringSource decryptor(encrypted, true,
            new StreamTransformationFilter(decryption,
                new StringSink(recovered))
        );

        ---------------------------
        m_aes_decryption = MakeUnique<CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption>();
        m_aes_decryption->SetKeyWithIV(m_key, Constants::Encryption::AES::DEFAULT_KEY_LENGTH, m_iv, Constants::Encryption::AES::DEFAULT_IV_LENGTH);

        // The StreamTransformationFilter removes
        //  padding as required.
        CryptoPP::StringSink* string_sink = new CryptoPP::StringSink(cipher);
        CryptoPP::StreamTransformationFilter* stream_transformation_filter = new CryptoPP::StreamTransformationFilter(*m_aes_encryption, string_sink);
        //CryptoPP::Base64Encoder* base64_encoder = new CryptoPP::Base64Encoder(string_sink);
        CryptoPP::StringSource* string_source = new CryptoPP::StringSource(buffer.data(), true, stream_transformation_filter);


        // Return cipher as iv + key + cipher for decripting purposes
        std::ostringstream oss;
        oss << m_iv << m_key << cipher;
        cipher = std::move(oss.str());


        // Encode cipher to base64
        byte* cipher_base64_ptr{ nullptr };
        size_t size = Base64::Encode(reinterpret_cast<byte*>(cipher.data()), cipher.size(), &cipher_base64_ptr);
        if (size > 0)
        {
            //construct a copy of cipher_base64_ptr which will be deleted 
            String cipher_base64 = reinterpret_cast<char*>(cipher_base64_ptr);
            ENIGMA_SAFE_DELETE_PTR(cipher_base64_ptr);

            return cipher_base64;
        }
        else
            throw CryptoPP::Exception(CryptoPP::Exception::ErrorType::OTHER_ERROR, "Failed to encode cipher to base64");

        //StringSource auto deletes sink & stream transform filter
        ENIGMA_SAFE_DELETE_PTR(string_source);
    }
    catch (const CryptoPP::Exception& e)
    {
        ENIGMA_CORE_ERROR("{0}", e.what());

        Enigma::MessageBox msg_box("AES Encryption Failure", e.what(), Enigma::MessageBox::Icon::Error, Enigma::MessageBox::Choice::Ok);
        UNUSED(msg_box.Show());

        return '\000';
    }
#endif 

    return "";
}

AESDecryption::~AESDecryption()
{
    delete[] m_key;
    delete[] m_iv;
}

NS_ENIGMA_END

