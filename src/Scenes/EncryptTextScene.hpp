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
};
NS_ENIGMA_END
/*
Note:
	 # Compression
	 - You should compress before encrypting. Encryption turns your data into high-entropy data,
	 usually indistinguishable from a random stream. Compression relies on patterns in order to gain
	 any size reduction. Since encryption destroys such patterns, the compression algorithm would be
	 unable to give you much (if any) reduction in size if you apply it to encrypted data.
*/
