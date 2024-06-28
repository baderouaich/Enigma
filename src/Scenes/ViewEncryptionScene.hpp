#pragma once
#include "Scene.hpp"

#include <Database/Database.hpp>

NS_ENIGMA_BEGIN

class ViewEncryptionScene : public Enigma::Scene
{
public:	/* Constructors / Destructor */
	explicit ViewEncryptionScene(const std::int64_t encryption_id);
	virtual ~ViewEncryptionScene() noexcept = default;

private: /* Overrides */
	void OnCreate() override;
	[[maybe_unused]] void OnUpdate(const float&) override;
	void OnDraw() override;
	void OnImGuiDraw() override;
	[[maybe_unused]] void OnEvent(Enigma::Event&) override;
	void OnDestroy() override;

private: /* Callbacks */
	void OnBackButtonPressed();
	void OnDecryptButtonPressed();
	void OnCopyRecoveredTextButtonPressed();

private:
	std::unique_ptr<Encryption> m_encryption{};

	std::string m_password{}; // password used for encryption text or file
	std::string m_recovered_text{}; // to be assigned Decrypted/Recovered Text after decryption success
};

NS_ENIGMA_END
