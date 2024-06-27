#pragma once
#include "Scene.hpp"

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

public:
	void SetLoadingText(const std::string_view& text) noexcept { m_loading_text = text; }
	const String& GetLoadingText() const noexcept { return m_loading_text; }

private:
	String m_loading_text; // loading text from a scene to appear bellow loading spinner to inform user of whats happening
	inline static constexpr const f32 SPINNER_RADIUS = 66.66f;
	inline static constexpr const f32 SPINNER_THICKNESS = 4.0f;
};
NS_ENIGMA_END

