#pragma once
#include "Scene.hpp"
#include <Algorithm/AES/AES.hpp>

NS_ENIGMA_BEGIN
class EncryptTextScene : public Enigma::Scene {
  public: /* Constructors / Destructor */
    EncryptTextScene();
    ~EncryptTextScene() noexcept override = default;

  private: /* Overrides */
    void OnCreate() override;
    [[maybe_unused]] void OnUpdate(const float& dt) override;
    void OnDraw() override;
    void OnImGuiDraw() override;
    void OnEvent(Enigma::Event& event) override;
    void OnDestroy() override;

  private: /* Callbacks */
    void OnEncryptButtonPressed();
    void OnCopyEncryptedBase64TextButtonPressed();
    void OnBackButtonPressed();

  private:
    Algorithm::Type m_type;                     // Algorithm type, AES, ChaCha, TripleDES... to help us create polymorphic algorithm
    std::string m_text;                         // Text to encrypt
    std::vector<byte> m_cipher;                 // Encrypted text
    std::string m_cipher_base64;                // Encrypted text in Base64
    std::string m_password, m_confirm_password; // Encryption password

    // Save to database
    std::string m_db_title;
    bool m_save_to_database{false};

    // Rsa special stuff
    std::string m_rsa_private_key;
    std::string m_rsa_public_key;
    std::size_t m_selected_rsa_keySize = 4096;
};
NS_ENIGMA_END