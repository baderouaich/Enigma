#pragma once
#include "Scene.hpp"
#include <Algorithm/AES/AES.hpp>
#include <Algorithm/ChaCha20/ChaCha20.hpp>
using namespace Enigma;

class EncryptTextScene : public Enigma::Scene
{
public:	/* Constructors / Destructor */
	explicit EncryptTextScene(const std::unordered_map<std::string_view, ImFont*>& fonts);
	virtual ~EncryptTextScene() = default;

private: /* Overrides */
	void OnCreate() override;
	[[maybe_unused]] void OnUpdate(const f32& dt) override;
	void OnDraw() override;
	void OnImGuiDraw() override;
	void OnEvent(Enigma::Event& event) override;
	void OnDestroy() override;

private: /* Callbacks */
	void OnEncryptButtonPressed();
	void OnBackButtonPressed();
	void OnCopyEncryptedBase64TextButtonPressed();

private:
	const std::unordered_map<std::string_view, ImFont*>& m_fonts; // from MainMenuScene where fonts are loaded

private:
	std::unique_ptr<Enigma::Algorithm> m_algorithm; // selected algorithm to encrypt text with
	String m_text; // Text to encrypt
	String m_encrypted_text; // Encrypted text cipher
	String m_encrypted_text_base64; // Encrypted text cipher in Base64
	String m_password, m_confirm_password; // Encryption password
	bool m_operation_completed; // true if user completed encryption successfully, used as a flag to whether show the user confirmation dialog on back pressed.
};



