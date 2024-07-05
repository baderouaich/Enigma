#pragma once
#include "Scene.hpp"
#include <Algorithm/Algorithm.hpp>

NS_ENIGMA_BEGIN
class EncryptFileScene : public Scene {
  public: /* Constructors / Destructor */
    explicit EncryptFileScene();
    virtual ~EncryptFileScene() = default;

  private: /* Overrides */
    void OnCreate() override;
    [[maybe_unused]] void OnUpdate(const float& dt) override;
    void OnDraw() override;
    void OnImGuiDraw() override;
    void OnEvent(Event& event) override;
    void OnDestroy() override;

  private: /* Callbacks */
    void OnBrowseInFileButtonPressed();
    void OnBrowseOutFileLocationButtonPressed();
    void OnEncryptButtonPressed();
    void OnBackButtonPressed();

  private:
    Algorithm::Type m_type;     // Algorithm type, AES, ChaCha, TripleDES... to help us create polymorphic algorithm
    std::string m_in_filename;  // In File to encrypt
    std::string m_out_filename; // Out File encrypted location
    std::string m_password, m_confirm_password;
    //unused, by default compression is enabled now. bool m_compress{ true }; // Whether to compress file with Gzip before encrypting

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
