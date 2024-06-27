#pragma once
#include "Scene.hpp"
#include <Algorithm/Algorithm.hpp>

NS_ENIGMA_BEGIN
class DecryptTextScene : public Enigma::Scene
{
public:	/* Constructors / Destructor */
	explicit DecryptTextScene();
	virtual ~DecryptTextScene() = default;

private: /* Overrides */
	void OnCreate() override;
	[[maybe_unused]] void OnUpdate(const f32& dt) override;
	void OnDraw() override;
	void OnImGuiDraw() override;
	void OnEvent(Event& event) override;
	void OnDestroy() override;

private: /* Callbacks */
	void OnDecryptButtonPressed();
	void OnBackButtonPressed();
	void OnCopyDecryptedTextButtonPressed();

private:
	Algorithm::Type m_type; // Algorithm type, AES, ChaCha, TripleDES... to help us create polymorphic algorithm
	String m_recovered_text; // Decrypted Text
	String m_cipher; // Encrypted text
	String m_cipher_base64; // Encrypted text in Base64 to be Decrypted
	String m_password; // Encryption password

};
NS_ENIGMA_END
