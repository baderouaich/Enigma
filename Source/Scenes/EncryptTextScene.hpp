#pragma once
#include "Scene.hpp"
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

private:
	/*
	*	Shows alert dialog to user asking whether the operation should be aborted
	*/
	bool UserWantsToCancel();

private:
	const std::unordered_map<std::string_view, ImFont*>& m_fonts; // from MainMenuScene where fonts are loaded

private:
	Constants::Algorithm::Type m_algorithm; // selected algorithm to encrypt text with
	String m_text; // Text to encrypt
	String m_encryption_password; // Encryption key
};



