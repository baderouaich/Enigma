#pragma once
#include <Enigma.hpp>
using namespace Enigma;

#define USING_SOUND false

class MainScene : public Enigma::Scene
{
public:	/* Constructors / Destructor */
	MainScene();
	virtual ~MainScene() = default;

private: /* Overrides */
	void OnCreate() override;
	void OnUpdate(const f32& dt) override;
	void OnDraw() override;
	void OnImGuiDraw() override;
	void OnEvent(Event& event) override;
	void OnDestroy() override;

private:
	//Hardware
	UniquePtr<RAMInfo> m_ram_info{};
};

