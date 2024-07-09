#pragma once
#include "Scene.hpp"
#include <Algorithm/Algorithm.hpp>

NS_ENIGMA_BEGIN
class DecryptTextScene : public Enigma::Scene {
  public: /* Constructors / Destructor */
    explicit DecryptTextScene();
    virtual ~DecryptTextScene() = default;

  private: /* Overrides */
    void OnCreate() override;
    [[maybe_unused]] void OnUpdate(const float& dt) override;
    void OnDraw() override;
    void OnImGuiDraw() override;
    void OnEvent(Event& event) override;
    void OnDestroy() override;

  private: /* Callbacks */
    void OnDecryptButtonPressed();
    void OnBackButtonPressed();
    void OnCopyDecryptedTextButtonPressed();

  private:
    Algorithm::Type m_type;       // Algorithm type, AES, ChaCha, TripleDES... to help us create polymorphic algorithm
    std::string m_recovered_text; // Decrypted Text
    std::vector<byte> m_cipher;   // Encrypted text
    std::string m_cipher_base64;  // Encrypted text in Base64 to be Decrypted
    std::string m_password;       // Encryption password

    // Rsa special stuff
    std::string m_rsa_private_key; // Only private key is needed for decryption (pub key used for verification)
    std::size_t m_selected_rsa_keySize = 2048;
};
NS_ENIGMA_END
