#include "pch.hpp"
#include "AESEncryption.hpp"
#include "System/Dialogs/MessageBox.hpp"
#include "Utility/Base64.hpp"

NS_ENIGMA_BEGIN
using namespace CryptoPP;


AESEncryption::AESEncryption()
{
    m_aes_encryption = MakeUnique<CFB_Mode<AES>::Encryption>();
    m_auto_seeded_random_pool = MakeUnique<CryptoPP::AutoSeededRandomPool>();
}

String AESEncryption::Encrypt(const String& password, const String& buffer)
{
    // Make sure encryption mode and the seeder are initialized
    { 
        ENIGMA_CORE_ASSERT(m_aes_encryption, "AESEncryption is not initialized properly");
        ENIGMA_CORE_ASSERT(m_auto_seeded_random_pool, "AESEncryption is not initialized properly");
    }
    // Validate Arguments
    {
        // AES password length must be at least 9 for security reasons
        if (password.size() < Constants::Algorithm::AES::AES_MINIMUM_PASSWORD_LENGTH)
        {
            const String msg = "AES Minimum Password Length is " + std::to_string(Constants::Algorithm::AES::AES_MINIMUM_PASSWORD_LENGTH);
            ENIGMA_CORE_ERROR(msg.c_str());
            Enigma::MessageBox msg_box("AES Encryption Failure", msg, Enigma::MessageBox::Icon::Error, Enigma::MessageBox::Choice::Ok);
            Enigma::MessageBox::Action action = msg_box.Show();
            UNUSED(action);
            return String();
        }
    }

    String encrypted; // Final encrypted buffer
    String iv = this->GenerateRandomIV(); // Random generated 16 bytes IV
    try
    {
        // Prepare Key
        CryptoPP::SecByteBlock key(AES::MAX_KEYLENGTH + AES::BLOCKSIZE); // Encryption key to be generated from user password + IV
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
        ENIGMA_CORE_ERROR("{0}", e.what());

        Enigma::MessageBox msg_box("AES Encryption Failure", e.what(), Enigma::MessageBox::Icon::Error, Enigma::MessageBox::Choice::Ok);
        UNUSED(msg_box.Show());
    }

    return (iv + encrypted);
}


String AESEncryption::GenerateRandomIV()
{
    String iv(CryptoPP::AES::BLOCKSIZE, '\000');
    m_auto_seeded_random_pool->GenerateBlock(reinterpret_cast<byte*>(iv.data()), iv.size());
    return iv;
}

#if 0
String AESEncryption::Decrypt(const String& password,  String& encrypted_buffer)
{
    String decrypted;
    try
    {
        //ExtractIVFromEncryptedBuffer
        String iv = encrypted_buffer.substr(0, 16);
        //Erase iv part from buffer
        encrypted_buffer.erase(encrypted_buffer.begin(), encrypted_buffer.begin() + 16);
        std::cout << "Decrypt IV: " << iv << std::endl;

        //Prepare
        HKDF<SHA256> hkdf;
        hkdf.DeriveKey(m_key, m_key.size(), (const byte*)password.data(), password.size(), (const byte*)iv.data(), iv.size(), nullptr, 0);


        //Decrypt
        ///////////////////////////////////////////////////////////////////////
        CTR_Mode<AES>::Decryption decryption;
        decryption.SetKeyWithIV(key, AES::MAX_KEYLENGTH, key + AES::MAX_KEYLENGTH);

        StringSource decryptor(encrypted, true,
            new StreamTransformationFilter(decryption,
                new StringSink(recovered))
        );
        ///////////////////////////////////////////////////////////////////////

        cout << "Recovered: " << recovered << endl;

    }
    catch (const CryptoPP::Exception& e)
    {
        ENIGMA_CORE_ERROR("{0}", e.what());

        Enigma::MessageBox msg_box("AES Decryption Failure", e.what(), Enigma::MessageBox::Icon::Error, Enigma::MessageBox::Choice::Ok);
        UNUSED(msg_box.Show());
    }

    return decrypted;
}
#endif

#if 0
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
#endif 
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
   // delete[] m_key;
   // delete[] m_iv;
}

NS_ENIGMA_END

