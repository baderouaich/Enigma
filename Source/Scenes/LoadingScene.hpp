#pragma once
#include "Scene.hpp"
#include <Algorithm/AES/AES.hpp>
#include <Algorithm/ChaCha20/ChaCha20.hpp>

NS_ENIGMA_BEGIN
class LoadingScene : public Enigma::Scene
{
public:
	explicit LoadingScene();
	virtual ~LoadingScene() noexcept = default;

private: /* Overrides */
	void OnCreate() override;
	[[maybe_unused]] void OnUpdate(const f32& dt) override;
	void OnDraw() override;
	void OnImGuiDraw() override;
	void OnEvent(Enigma::Event& event) override;
	void OnDestroy() override;

private:
	inline static constexpr const f32 SPINNER_RADIUS = 66.66f;
	inline static constexpr const i32 SPINNER_THICKNESS = 4;
};
NS_ENIGMA_END

