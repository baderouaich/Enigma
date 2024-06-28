#pragma once
#include "../Scene.hpp"
#include "Tool.hpp"

NS_ENIGMA_BEGIN

class ToolsScene : public Enigma::Scene
{
public:	/* Constructors / Destructor */
	ToolsScene() noexcept;
	virtual ~ToolsScene() noexcept = default;

private: /* Overrides */
	void OnCreate() override;
	[[maybe_unused]] void OnUpdate(const float&) override;
	void OnDraw() override;
	void OnImGuiDraw() override;
	[[maybe_unused]] void OnEvent(Event&) override;
	void OnDestroy() override;

private:
	std::vector<std::unique_ptr<Tool>> m_tools{}; // Polymorphic tool classes which draws collapsing header view of their context
};

NS_ENIGMA_END
