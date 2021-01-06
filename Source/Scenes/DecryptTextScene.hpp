#pragma once
#include "Scene.hpp"
using namespace Enigma;

class DecryptTextScene : public Enigma::Scene
{
public:	/* Constructors / Destructor */
	explicit DecryptTextScene(const std::unordered_map<std::string_view, ImFont*>& fonts);
	virtual ~DecryptTextScene() = default;

private: /* Overrides */
	void OnCreate() override;
	[[maybe_unused]] void OnUpdate(const f32& dt) override;
	void OnDraw() override;
	void OnImGuiDraw() override;
	void OnEvent(Event& event) override;
	void OnDestroy() override;

private: /* Callbacks */
	void OnAutoDetectAlgorithmButtonPressed();
	void OnDecryptButtonPressed();
	void OnBackButtonPressed();

private:
	const std::unordered_map<std::string_view, ImFont*>& m_fonts; // from MainMenuScene where fonts are loaded

	std::unique_ptr<Enigma::Algorithm> m_algorithm; // selected algorithm used to encrypt text with
	String m_recovered_text; // Decrypted Text
	String m_cipher; // Encrypted text
	String m_cipher_base64; // Encrypted text in Base64 to be Decrypted
	String m_password; // Encryption password

};


