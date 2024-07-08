#pragma once
#include <Algorithm/Algorithm.hpp>
#include <memory>
#include <optional>
#include <rsa.h>
#include <base64.h>
#include "Utility/FileUtils.hpp"

NS_ENIGMA_BEGIN
/**
*	RSA-AES-GCM Algorithm Encryptor/Decryptor
* 
* RSA is a public-key cryptosystem, one of the oldest widely used for secure data transmission
*/
class RSA : public Algorithm {
    inline static const std::string BEGIN_RSA_PRIVATE_KEY_HEADER = "-----BEGIN RSA PRIVATE KEY-----\n";
    inline static const std::string END_RSA_PRIVATE_KEY_FOOTER = "-----END RSA PRIVATE KEY-----\n";
    inline static const std::string BEGIN_RSA_PUBLIC_KEY_HEADER = "-----BEGIN RSA PUBLIC KEY-----\n";
    inline static const std::string END_RSA_PUBLIC_KEY_FOOTER = "-----END RSA PUBLIC KEY-----\n";

  public:
    /**
    *	@param intent: Operation, Encrypt or Decrypt
    */
    explicit RSA(const Algorithm::Intent intent) noexcept;
    ~RSA() noexcept override;

    struct RSASettings {
      std::size_t keySize{};
      std::optional<std::string> privateKey{std::nullopt};
      //std::optional<std::string> publicKey{std::nullopt};
      std::optional<fs::path> privateKeyFilename{std::nullopt};
      //std::optional<fs::path> publicKeyFilename{std::nullopt};
    };

    void setSettings(RSASettings&& settings) {
      m_settings = std::make_unique<RSASettings>(std::move(settings));
      initialize();
    }

    void initialize(){
      m_params = std::make_unique<CryptoPP::InvertibleRSAFunction>();
      m_params->GenerateRandomWithKeySize(*m_auto_seeded_random_pool, m_settings->keySize);

      if(static_cast<bool>(m_intent & Intent::Encrypt)) {
        m_private_key = std::make_unique<CryptoPP::RSA::PrivateKey>(*m_params);
        m_public_key = std::make_unique<CryptoPP::RSA::PublicKey>(*m_params);
        m_rsa_encryptor = std::make_unique<decltype(m_rsa_encryptor)::element_type>(*m_public_key);
      }
      if(static_cast<bool>(m_intent & Intent::Decrypt)) {
        ENIGMA_ASSERT_OR_THROW(m_settings->privateKey or m_settings->privateKeyFilename, "RSA private key was not set for decryption");
        if(m_settings->privateKey)
          setPrivateKey(*m_settings->privateKey);
        else {
         std::vector<byte> pk;
         FileUtils::Read(*m_settings->privateKeyFilename, pk);
         setPrivateKey(std::string(pk.begin(), pk.end()));
        }m_rsa_decryptor = std::make_unique<decltype(m_rsa_decryptor)::element_type>(*m_private_key);
      }
    }
  public:
    std::vector<byte> Encrypt(const std::string& password, const byte *buffer, const std::size_t buffSize) override;
    std::vector<byte> Encrypt(const std::string& password, const std::vector<byte>& buffer) override;
    std::vector<byte> Decrypt(const std::string& password, const byte *cipher, const std::size_t cipherSize) override;
    std::vector<byte> Decrypt(const std::string& password, const std::vector<byte>& cipher) override;
    void Encrypt(const std::string& password, const fs::path& in_filename, const fs::path& out_filename) override;
    void Decrypt(const std::string& password, const fs::path& in_filename, const fs::path& out_filename) override;


  private:

    void setPrivateKey(const std::string& privateKey) {
      std::string privateKeyBase64 = privateKey;
      if(privateKeyBase64.starts_with(BEGIN_RSA_PRIVATE_KEY_HEADER)) {
        privateKeyBase64.erase(privateKeyBase64.begin(), privateKeyBase64.begin() + BEGIN_RSA_PRIVATE_KEY_HEADER.size());
      }
      if(privateKeyBase64.ends_with(END_RSA_PRIVATE_KEY_FOOTER)) {
        privateKeyBase64.erase(privateKeyBase64.begin() + privateKeyBase64.size() - END_RSA_PRIVATE_KEY_FOOTER.size(), privateKeyBase64.end());
      }

      std::string decoded;
      CryptoPP::StringSource ss(privateKeyBase64, true, new CryptoPP::Base64Decoder(new CryptoPP::StringSink(decoded)));
      m_private_key.reset(new CryptoPP::RSA::PrivateKey());

      CryptoPP::ArraySource as(reinterpret_cast<const byte*>(decoded.data()), decoded.size(), true);
      m_private_key->Load(as);
    }

    void setPublicKey(const std::string& publicKey) {
      std::string publicKeyBase64 = publicKey;
      if(publicKeyBase64.starts_with(BEGIN_RSA_PUBLIC_KEY_HEADER)) {
        publicKeyBase64.erase(publicKeyBase64.begin(), publicKeyBase64.begin() + BEGIN_RSA_PUBLIC_KEY_HEADER.size());
      }
      if(publicKeyBase64.ends_with(END_RSA_PUBLIC_KEY_FOOTER)) {
        publicKeyBase64.erase(publicKeyBase64.begin() + publicKeyBase64.size() - END_RSA_PUBLIC_KEY_FOOTER.size(), publicKeyBase64.end());
      }

      std::string decoded;
      CryptoPP::StringSource ss(publicKeyBase64, true, new CryptoPP::Base64Decoder(new CryptoPP::StringSink(decoded)));
      m_public_key.reset(new CryptoPP::RSA::PublicKey());


      CryptoPP::ArraySource as(reinterpret_cast<const byte*>(decoded.data()), decoded.size(), true);
      m_public_key->Load(as);
    }

  public:
    std::string getPrivateKey() const {
      std::string derPrivateKey;
      CryptoPP::StringSink derSink(derPrivateKey);
      m_private_key->DEREncode(derSink);

      std::string base64PrivateKey;
      CryptoPP::StringSource ss(derPrivateKey, true,
                             new CryptoPP::Base64Encoder(new CryptoPP::StringSink(base64PrivateKey), true /* with newlines */)
      );
      return BEGIN_RSA_PRIVATE_KEY_HEADER + base64PrivateKey + END_RSA_PRIVATE_KEY_FOOTER;
    }

    std::string getPublicKey() const {
      std::string derPublicKey;
      CryptoPP::StringSink derSink(derPublicKey);
      m_public_key->DEREncode(derSink);

      std::string base64PublicKey;
      CryptoPP::StringSource ss(derPublicKey, true,
                             new CryptoPP::Base64Encoder(new CryptoPP::StringSink(base64PublicKey), true /* with newlines */)
      );
      return BEGIN_RSA_PUBLIC_KEY_HEADER + base64PublicKey + END_RSA_PUBLIC_KEY_FOOTER;
    }

  public:
    std::size_t getMaximumBufferSize() const {
      ENIGMA_ASSERT_OR_THROW(m_rsa_encryptor, "RSA encryptor was not initialized properly");
      return m_rsa_encryptor->FixedMaxPlaintextLength();
    }
//    std::size_t getKeySize() const {
//      ENIGMA_ASSERT_OR_THROW(m_params, "RSA params was not initialized properly");
//    }
    static constexpr std::size_t getMaximumBufferSizeFromKeySize(const std::size_t keySize) {
      return keySize / 8 - 2 * static_cast<std::size_t>(CryptoPP::SHA256::DIGESTSIZE) - 2;
    }

  private:
    std::unique_ptr<CryptoPP::RSAES<CryptoPP::OAEP<CryptoPP::SHA256>>::Encryptor> m_rsa_encryptor; /**< RSA encryptor */
    std::unique_ptr<CryptoPP::RSAES<CryptoPP::OAEP<CryptoPP::SHA256>>::Decryptor> m_rsa_decryptor; /**< RSA decryptor */
    std::unique_ptr<CryptoPP::RSA::PrivateKey> m_private_key;
    std::unique_ptr<CryptoPP::RSA::PublicKey> m_public_key;
    std::unique_ptr<CryptoPP::InvertibleRSAFunction> m_params;

    std::unique_ptr<RSASettings> m_settings{};

};

NS_ENIGMA_END
