#pragma once
#include "Scene.hpp"
using namespace Enigma;

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
	void OnBrowseButtonClicked();

private:
	const std::unordered_map<std::string_view, ImFont*>& m_fonts; // from MainMenuScene where fonts are loaded
	std::unique_ptr<Enigma::Algorithm> m_algorithm; // selected algorithm to encrypt File with

	String m_in_file_path; // In File to encrypt
};

