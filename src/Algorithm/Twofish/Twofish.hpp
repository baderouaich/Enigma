#pragma once
#include <Algorithm/Algorithm.hpp>

NS_ENIGMA_BEGIN
/**
*	Twofish-GCM Algorithm Encryptor/Decryptor
*/
class Twofish : public Algorithm {
  public:
    /**
    *	@param intent: Operation, Encrypt or Decrypt
    */
    explicit Twofish(const Algorithm::Intent intent) noexcept;
    virtual ~Twofish() noexcept;

  public:
    std::vector<byte> Encrypt(const std::string& password, const std::vector<byte>& buffer) override;
    std::vector<byte> Decrypt(const std::string& password, const std::vector<byte>& cipher) override;
    void Encrypt(const std::string& password, const fs::path& in_filename, const fs::path& out_filename) override;
    void Decrypt(const std::string& password, const fs::path& in_filename, const fs::path& out_filename) override;

  private:
    std::unique_ptr<CryptoPP::GCM<CryptoPP::Twofish>::Encryption> m_twofish_encryptor; /**< Twofish-GCM encryptor */
    std::unique_ptr<CryptoPP::GCM<CryptoPP::Twofish>::Decryption> m_twofish_decryptor; /**< Twofish-GCM decryptor */
};

NS_ENIGMA_END
