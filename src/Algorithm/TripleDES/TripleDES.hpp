#pragma once
#include <Algorithm/Algorithm.hpp>

NS_ENIGMA_BEGIN
/**
*	TripleDES Algorithm Encryptor/Decryptor
*	
*	TripleDES, TDEA or 3DES, is multiple applications of the DES algorithm to plain text data to increase the security
*	of the original DES algorithm.As the name implies, the DES algorithm is applied 3 times.There are two variants
*	of TripleDES : the first is two key;and the second is three key. 2 - key TDEA provides approximately 80 bits of security,
*	while 3 - key TDEA provides approximately 112 bits of security.In contrast, AES provides a minimum security level of 128.
*/
class TripleDES : public Algorithm {
  public:
    /**
    *	@param intent: Operation, Encrypt or Decrypt
    */
    explicit TripleDES(const Algorithm::Intent intent) noexcept;
    ~TripleDES() noexcept override;

  public:
    std::vector<byte> Encrypt(const std::string& password, const byte *buffer, const std::size_t buffSize) override;
    std::vector<byte> Encrypt(const std::string& password, const std::vector<byte>& buffer) override;
    std::vector<byte> Decrypt(const std::string& password, const byte *cipher, const std::size_t cipherSize) override;
    std::vector<byte> Decrypt(const std::string& password, const std::vector<byte>& cipher) override;
    void Encrypt(const std::string& password, const fs::path& in_filename, const fs::path& out_filename) override;
    void Decrypt(const std::string& password, const fs::path& in_filename, const fs::path& out_filename) override;

  private:
    std::unique_ptr<CryptoPP::EAX<CryptoPP::DES_EDE3>::Encryption> m_tripledes_encryptor; /**< 3 Key TripleDES-EAX encryptor */
    std::unique_ptr<CryptoPP::EAX<CryptoPP::DES_EDE3>::Decryption> m_tripledes_decryptor; /**< 3 Key TripleDES-EAX decryptor */
};

NS_ENIGMA_END
