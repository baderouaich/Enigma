#pragma once
#include <Algorithm/Algorithm.hpp>

NS_ENIGMA_BEGIN
/**
*	Camellia-GCM Algorithm Encryptor/Decryptor
* 
*	Camellia is a block cipher developed by Mitsubishi Electric Corporation and Nippon Telegraph and Telephone.
*	The cipher has been approved for use by the ISO/IEC, the European Union's NESSIE project, the Japanese CRYPTREC project,
*	and the Internet Engineering Task Force. The Camellia homepage is located at http://info.isl.ntt.co.jp/crypt/eng/camellia/index.html.
*/
class Camellia : public Algorithm {
  public:
    /**
    *	@param intent: Operation, Encrypt or Decrypt
    */
    explicit Camellia(const Algorithm::Intent intent) noexcept;
    ~Camellia() noexcept override;

  public:
    std::vector<byte> Encrypt(const std::string& password, const byte *buffer, const std::size_t buffSize) override;
    std::vector<byte> Encrypt(const std::string& password, const std::vector<byte>& buffer) override;
    std::vector<byte> Decrypt(const std::string& password, const byte *cipher, const std::size_t cipherSize) override;
    std::vector<byte> Decrypt(const std::string& password, const std::vector<byte>& cipher) override;
    void Encrypt(const std::string& password, const fs::path& in_filename, const fs::path& out_filename) override;
    void Decrypt(const std::string& password, const fs::path& in_filename, const fs::path& out_filename) override;

  private:
    std::unique_ptr<CryptoPP::GCM<CryptoPP::Camellia>::Encryption> m_camellia_encryptor; /**< Camellia-GCM encryptor */
    std::unique_ptr<CryptoPP::GCM<CryptoPP::Camellia>::Decryption> m_camellia_decryptor; /**< Camellia-GCM decryptor */
};

NS_ENIGMA_END
