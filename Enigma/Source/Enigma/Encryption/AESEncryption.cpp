#include "Epch.hpp"
#include "AESEncryption.hpp"
#include "Enigma/System/Dialogs/MessageBox.hpp"
#include "Enigma/Utility/Base64.hpp"

NS_ENIGMA_BEGIN

AESEncryption::AESEncryption()
    :
    m_aes_encryption(nullptr),
    m_auto_seeded_random_pool(MakeUnique<CryptoPP::AutoSeededRandomPool>())
{
    //Auto generate key & IV
//TODO: choose key length desired
    m_key = new byte[Constants::Encryption::AES::DEFAULT_KEY_LENGTH];
    m_auto_seeded_random_pool->GenerateBlock(m_key, Constants::Encryption::AES::DEFAULT_KEY_LENGTH);
    m_iv = new byte[Constants::Encryption::AES::DEFAULT_IV_LENGTH];
    m_auto_seeded_random_pool->GenerateBlock(m_iv, Constants::Encryption::AES::DEFAULT_IV_LENGTH);
}

String AESEncryption::Encrypt(const String& buffer)
{
    String cipher; 
    byte* cipher_base64_ptr{ nullptr };
    String cipher_base64;
    try
    {

        m_aes_encryption = MakeUnique<CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption>();
        m_aes_encryption->SetKeyWithIV(m_key, Constants::Encryption::AES::DEFAULT_KEY_LENGTH, m_iv, Constants::Encryption::AES::DEFAULT_IV_LENGTH);

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
       size_t size = Base64::Encode(reinterpret_cast<byte*>(cipher.data()), cipher.size(), &cipher_base64_ptr);
       if (size > 0)
       {
           //construct a copy of cipher_base64_ptr which will be deleted 
           cipher_base64 = reinterpret_cast<char*>(cipher_base64_ptr);
           ENIGMA_SAFE_DELETE_PTR(cipher_base64_ptr);
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
    }

    return cipher_base64;
}

#if 0
    try
    {
        m_aes_encryption = MakeUnique<CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption>();
        UniquePtr<CryptoPP::MD5> hash = MakeUnique<CryptoPP::MD5>();

        std::array<byte, CryptoPP::MD5::DIGESTSIZE> digest;
        String message = m_iv + m_key;

        hash->CalculateDigest(digest.data(), reinterpret_cast<unsigned char*>(&message[0]), message.size());
        m_iv = String(reinterpret_cast<char*>(digest.data()), 16);
        m_aes_encryption->SetKeyWithIV(reinterpret_cast<unsigned char*>(&m_key[0]), m_key.size(), digest.data());
        m_aes_encryption->ProcessData(
            reinterpret_cast<unsigned char*>(&buffer[0]),
            reinterpret_cast<unsigned char*>(&buffer[0]),
            buffer.size()
        );
    }
    catch (const CryptoPP::Exception& e)
    {
        ENIGMA_CORE_ERROR("{0}", e.what());
        return false;
    }
    return true;
}

String AESEncryption::Decrypt(const String& cipher)
{
    String buffer;
    try
    {
        m_aes_decryption = MakeUnique<CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption>();
        m_aes_decryption->SetKeyWithIV(reinterpret_cast<byte*>(m_key.data()), m_key.size(),
            reinterpret_cast<byte*>(m_iv.data()), m_iv.size());

        // The StreamTransformationFilter removes
        //  padding as required.
        UniquePtr<CryptoPP::StringSink> string_sink(
            new CryptoPP::StringSink(cipher)
        );
        UniquePtr<CryptoPP::StreamTransformationFilter> stream_transformation_filter(
            new CryptoPP::StreamTransformationFilter(*m_aes_decryption, string_sink.get())
        );
        UniquePtr<CryptoPP::StringSource> string_source(
            new CryptoPP::StringSource(buffer.data(), true, stream_transformation_filter.get())
        );
    }
    catch (const CryptoPP::Exception& e)
    {
        ENIGMA_CORE_ERROR("{0}", e.what());

        Enigma::MessageBox msg_box("AES Decryption Failure", e.what(), Enigma::MessageBox::Icon::Error, Enigma::MessageBox::Choice::Ok);
        UNUSED(msg_box.Show());
    }
    return buffer;
}
#endif

AESEncryption::~AESEncryption()
{
    delete[] m_key;
    delete[] m_iv;
}

NS_ENIGMA_END

