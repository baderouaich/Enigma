#pragma once
#include "Scene.hpp"


NS_ENIGMA_BEGIN
class DecryptFileScene : public Scene
{
public:	/* Constructors / Destructor */
	explicit DecryptFileScene(const std::unordered_map<std::string_view, ImFont*>& fonts);
	virtual ~DecryptFileScene() = default;

private: /* Overrides */
	void OnCreate() override;
	[[maybe_unused]] void OnUpdate(const f32& dt) override;
	void OnDraw() override;
	void OnImGuiDraw() override;
	void OnEvent(Event& event) override;
	void OnDestroy() override;

private: /* Callbacks */
	void OnAutoDetectAlgorithmButtonPressed();
	void OnBrowseInFileButtonPressed();
	void OnBrowseOutFileButtonPressed();
	void OnDecryptButtonPressed();
	void OnBackButtonPressed();

private:
	const std::unordered_map<std::string_view, ImFont*>& m_fonts; // from MainMenuScene where fonts are loaded
	
private:
	Algorithm::Type m_type; // Algorithm type, AES, ChaCha, TripleDES... to help us create polymorphic algorithm
	String m_in_filename; // In File to decrypt
	String m_out_filename; // Out File to be recover
	String m_password; // encryption password
	bool m_decompress{ true }; // Whether to deompress file with Gzip after decrypting 

};
NS_ENIGMA_END

