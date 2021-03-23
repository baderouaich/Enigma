#pragma once
#include "Scene.hpp"

#include <Database/Database.hpp>

NS_ENIGMA_BEGIN

class ViewEncryptionScene : public Enigma::Scene
{
public:	/* Constructors / Destructor */
	explicit ViewEncryptionScene(const i64 encryption_id);
	virtual ~ViewEncryptionScene() noexcept = default;

private: /* Overrides */
	void OnCreate() override;
	[[maybe_unused]] void OnUpdate(const f32&) override;
	void OnDraw() override;
	void OnImGuiDraw() override;
	[[maybe_unused]] void OnEvent(Enigma::Event&) override;
	void OnDestroy() override;

private: /* Callbacks */
	void OnBackButtonPressed();

private:
	std::unique_ptr<Encryption> m_encryption{};
};

NS_ENIGMA_END
