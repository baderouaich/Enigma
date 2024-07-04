#pragma once
#include <Algorithm/Algorithm.hpp>

NS_ENIGMA_BEGIN
/**
*	Blowfish Algorithm Encryptor/Decryptor
*/
class Blowfish : public Algorithm {
  public:
    /**
    *	@param intent: Operation, Encrypt or Decrypt
    */
    explicit Blowfish(const Algorithm::Intent intent) noexcept;
    ~Blowfish() noexcept override;

  public:
    std::vector<byte> Encrypt(const std::string& password, const byte* buffer, const std::size_t buffSize) override;
    std::vector<byte> Encrypt(const std::string& password, const std::vector<byte>& buffer) override;
    std::vector<byte> Decrypt(const std::string& password, const byte* cipher, const std::size_t cipherSize) override;
    std::vector<byte> Decrypt(const std::string& password, const std::vector<byte>& cipher) override;
    void Encrypt(const std::string& password, const fs::path& in_filename, const fs::path& out_filename) override;
    void Decrypt(const std::string& password, const fs::path& in_filename, const fs::path& out_filename) override;

  private:
    std::unique_ptr<CryptoPP::EAX<CryptoPP::Blowfish>::Encryption> m_blowfish_encryptor; /**< Blowfish-EAX encryptor */
    std::unique_ptr<CryptoPP::EAX<CryptoPP::Blowfish>::Decryption> m_blowfish_decryptor; /**< Blowfish-EAX decryptor */
};

NS_ENIGMA_END