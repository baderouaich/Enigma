#pragma once
#include <Algorithm/Algorithm.hpp>
#include <memory>
#include <optional>
#include <rsa.h>
#include <base64.h>
#include <Utility/FileUtils.hpp>
#include <pssr.h>

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

    void setSettings(RSASettings&& settings);


  public:
    std::vector<byte> Encrypt(const std::string& password, const byte *buffer, const std::size_t buffSize) override;
    std::vector<byte> Encrypt(const std::string& password, const std::vector<byte>& buffer) override;
    std::vector<byte> Decrypt(const std::string& password, const byte *cipher, const std::size_t cipherSize) override;
    std::vector<byte> Decrypt(const std::string& password, const std::vector<byte>& cipher) override;
    void Encrypt(const std::string& password, const fs::path& in_filename, const fs::path& out_filename) override;
    void Decrypt(const std::string& password, const fs::path& in_filename, const fs::path& out_filename) override;

    /**
     * todo:
     * @return
     */
    [[deprecated("Not implemented yet")]]
    bool SignAndVerify(const std::vector<byte>& message) {
      /*
       * TO SIGN: YOU NEED PRIV KEY
       * TO VERIFY: YOU NEED PUB KEY
       *
       */
      // Verifier object
      // Signer object
      using namespace CryptoPP;
      RSASS<PSS, SHA256>::Signer signer(*m_private_key);

      // Create signature space
      size_t length = signer.MaxSignatureLength();
      SecByteBlock signature(length);

      // Sign message
      length = signer.SignMessage(*m_auto_seeded_random_pool, message.data(),
                                  message.size(),
                                  signature);

      // Resize now we know the true size of the signature
      signature.resize(length);

      // Verifier object
      RSASS<PSS, SHA256>::Verifier verifier(*m_public_key);

      // Verify
      bool result = verifier.VerifyMessage(message.data(),
                                           message.size(), signature, signature.size());
      return result;
    }

  private:
    void initialize();
    void setPrivateKey(const std::string& privateKey);
    void setPublicKey(const std::string& publicKey);

  public:
    std::string getPrivateKey() const;
    std::string getPublicKey() const;

  public:
    std::size_t getMaximumBufferSize() const;
    static constexpr std::size_t getMaximumBufferSizeFromKeySize(const std::size_t keySize);

  private:
    std::unique_ptr<CryptoPP::RSAES<CryptoPP::OAEP<CryptoPP::SHA256>>::Encryptor> m_rsa_encryptor; /**< RSA encryptor */
    std::unique_ptr<CryptoPP::RSAES<CryptoPP::OAEP<CryptoPP::SHA256>>::Decryptor> m_rsa_decryptor; /**< RSA decryptor */
    std::unique_ptr<CryptoPP::RSA::PrivateKey> m_private_key;
    std::unique_ptr<CryptoPP::RSA::PublicKey> m_public_key;
    std::unique_ptr<CryptoPP::InvertibleRSAFunction> m_params;

    std::unique_ptr<RSASettings> m_settings{};

  public:
    /// Key, description
    inline static const std::map<std::size_t, std::string_view> RECOMMENDED_KEY_SIZES = {
      {2048, "Secure for use at least until 2030."},
      {3072, "Secure for use beyond 2030."},
      {4096, "Secure for longer-term security needs."},
      {8192, "Very high security but very slow. Suitable for cases requiring extremely high security,\nthough it's rarely used in practice due to performance concerns."},
      {16384, "Almost never used due to extreme computational cost.\nThis key size will take about ~5 minutes to complete"},
      {32768, "Theoretical and impractical for most applications\ndue to excessive computational and storage requirements.\nThis key size will take a lot of time to complete"},
    };
};

NS_ENIGMA_END
