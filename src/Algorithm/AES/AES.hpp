#pragma once
#include <Algorithm/Algorithm.hpp>

NS_ENIGMA_BEGIN
/**
*	AES-GCM Algorithm Encryptor/Decryptor
* 
*	AES symmetric cipher is recommended to be used with GCM mode
*/
class AES : public Algorithm {
  public:
    /**
    *	@param intent: Operation, Encrypt or Decrypt
    */
    explicit AES(const Algorithm::Intent intent) noexcept;
    ~AES() noexcept override;

  public:
    std::vector<byte> Encrypt(const std::string& password, const byte* buffer, const std::size_t buffSize) override;
    std::vector<byte> Encrypt(const std::string& password, const std::vector<byte>& buffer) override;
    std::vector<byte> Decrypt(const std::string& password, const byte* cipher, const std::size_t cipherSize) override;
    std::vector<byte> Decrypt(const std::string& password, const std::vector<byte>& cipher) override;
    void Encrypt(const std::string& password, const fs::path& in_filename, const fs::path& out_filename) override;
    void Decrypt(const std::string& password, const fs::path& in_filename, const fs::path& out_filename) override;

  private:
    std::unique_ptr<CryptoPP::GCM<CryptoPP::AES>::Encryption> m_aes_encryptor; /**< AES-GCM encryptor */
    std::unique_ptr<CryptoPP::GCM<CryptoPP::AES>::Decryption> m_aes_decryptor; /**< AES-GCM decryptor */
};

NS_ENIGMA_END
