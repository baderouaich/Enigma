#pragma once
#include "Scene.hpp"

NS_ENIGMA_BEGIN
class EncryptFileScene : public Scene
{
public:	/* Constructors / Destructor */
	explicit EncryptFileScene(const std::unordered_map<std::string_view, ImFont*>& fonts);
	virtual ~EncryptFileScene() = default;

private: /* Overrides */
	void OnCreate() override;
	[[maybe_unused]] void OnUpdate(const f32& dt) override;
	void OnDraw() override;
	void OnImGuiDraw() override;
	void OnEvent(Event& event) override;
	void OnDestroy() override;

private: /* Callbacks */
	void OnBrowseInFileButtonPressed();
	void OnBrowseOutFileButtonPressed();
	void OnEncryptButtonPressed();
	void OnBackButtonPressed();

private:
	const std::unordered_map<std::string_view, ImFont*>& m_fonts; // from MainMenuScene where fonts are loaded
	
private:
	Algorithm::Type m_type; // Algorithm type, AES, ChaCha, TripleDES... to help us create polymorphic algorithm
	String m_in_filename; // In File to encrypt
	String m_out_filename; // Out File encrypted location
	String m_password, m_confirm_password;
	bool m_compress{ true }; // Whether to compress file with Gzip before encrypting 

	// Save to database 
	String m_db_title;
	bool m_save_to_database{ false };

};
NS_ENIGMA_END
