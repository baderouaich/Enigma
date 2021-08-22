#pragma once
#include "Scene.hpp"
#include <Algorithm/AES/AES.hpp>

NS_ENIGMA_BEGIN
class RSAScene : public Enigma::Scene
{
public:	/* Constructors / Destructor */
	RSAScene();
	virtual ~RSAScene() noexcept = default;

private: /* Overrides */
	void OnCreate() override;
	[[maybe_unused]] void OnUpdate(const f32& dt) override;
	void OnDraw() override;
	void OnImGuiDraw() override;
	void OnEvent(Enigma::Event& event) override;
	void OnDestroy() override;

private: /* Callbacks */


private:


};
NS_ENIGMA_END
