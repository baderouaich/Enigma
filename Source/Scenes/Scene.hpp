#pragma once
#ifndef ENIGMA_SCENE_H
#define ENIGMA_SCENE_H

#include <Core/Core.hpp>
#include <Logger/Logger.hpp>
#include <Events/Event.hpp>

NS_ENIGMA_BEGIN
/*
*	Scene Abstract class
*/
class ENIGMA_API Scene
{
public:
	friend class Application;

	ENIGMA_NON_COPYABLE(Scene);
	ENIGMA_NON_MOVEABLE(Scene);

public:
    Scene() noexcept;
    virtual ~Scene() noexcept;

	/* Scene Life Cicle */
	virtual void OnCreate() = 0;
	virtual void OnEvent(Event& event) = 0;
	virtual void OnUpdate(const f32& dt) = 0;
	virtual void OnDraw() = 0;
	virtual void OnImGuiDraw() = 0;
	virtual void OnDestroy() = 0;

public: /*Accessors*/
	const bool& WantsToQuit() const noexcept { return m_quit; }

public: /*Modifiers*/
	void EndScene() noexcept { m_quit = true; }


protected:
	bool m_quit;
};

NS_ENIGMA_END

#endif // !ENIGMA_SCENE_H
