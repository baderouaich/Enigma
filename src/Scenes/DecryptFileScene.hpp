#pragma once
#include "Scene.hpp"
#include <Algorithm/Algorithm.hpp>


NS_ENIGMA_BEGIN
class DecryptFileScene : public Scene {
  public: /* Constructors / Destructor */
    DecryptFileScene();
    virtual ~DecryptFileScene() = default;

  private: /* Overrides */
    void OnCreate() override;
    [[maybe_unused]] void OnUpdate(const float& dt) override;
    void OnDraw() override;
    void OnImGuiDraw() override;
    void OnEvent(Event& event) override;
    void OnDestroy() override;

  private: /* Callbacks */
    //void OnBrowseInFileButtonPressed();
    //void OnBrowseOutFileButtonPressed();
    void OnBrowseInFileButtonPressed();          // browse file to decrypt
    void OnBrowseOutFileLocationButtonPressed(); // browse decrypted file location
    void OnDecryptButtonPressed();
    void OnBackButtonPressed();

  private:
    Algorithm::Type m_type;     // Algorithm type, AES, ChaCha, TripleDES... to help us create polymorphic algorithm
    std::string m_in_filename;  // In File to decrypt
    std::string m_out_filename; // Out File to be recover
    std::string m_password;     // encryption password
                                //bool m_decompress{ true }; // Whether to deompress file with Gzip after decrypting
};
NS_ENIGMA_END
