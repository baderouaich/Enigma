#pragma once
#include <Algorithm/Algorithm.hpp>

NS_ENIGMA_BEGIN
/**
*	Serpent-GCM Algorithm Encryptor/Decryptor
* 
*	Serpent is a block cipher designed by Ross Anderson, Eli Biham, and Lars Knudsen.
*	It was ranked 2nd in the Advanced Encryption Standard contest.
*	The Serpent homepage is located at http://www.cl.cam.ac.uk/~rja14/serpent.html.
*/
class Serpent : public Algorithm {
  public:
    /**
    *	@param intent: Operation, Encrypt or Decrypt
    */
    explicit Serpent(const Algorithm::Intent intent) noexcept;
    ~Serpent() noexcept override;

  public:
    std::vector<byte> Encrypt(const std::string& password, const byte *buffer, const std::size_t buffSize) override;
    std::vector<byte> Encrypt(const std::string& password, const std::vector<byte>& buffer) override;
    std::vector<byte> Decrypt(const std::string& password, const byte *cipher, const std::size_t cipherSize) override;
    std::vector<byte> Decrypt(const std::string& password, const std::vector<byte>& cipher) override;
    void Encrypt(const std::string& password, const fs::path& in_filename, const fs::path& out_filename) override;
    void Decrypt(const std::string& password, const fs::path& in_filename, const fs::path& out_filename) override;

  private:
    std::unique_ptr<CryptoPP::GCM<CryptoPP::Serpent>::Encryption> m_serpent_encryptor; /**< Serpent-GCM encryptor */
    std::unique_ptr<CryptoPP::GCM<CryptoPP::Serpent>::Decryption> m_serpent_decryptor; /**< Serpent-GCM decryptor */
};

NS_ENIGMA_END
