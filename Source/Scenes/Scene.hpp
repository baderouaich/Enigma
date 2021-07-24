#pragma once
#ifndef ENIGMA_SCENE_H
#define ENIGMA_SCENE_H

#include <Core/Core.hpp>
#include <Logger/Logger.hpp>
#include <Events/Event.hpp>

#include <mutex>

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
	virtual ~Scene() noexcept = default;

	/* Scene Life Cicle */
	virtual void OnCreate() = 0;
	virtual void OnEvent(Event& event) = 0;
	virtual void OnUpdate(const f32& dt) = 0;
	virtual void OnDraw() = 0;
	virtual void OnImGuiDraw() = 0;
	virtual void OnDestroy() = 0;

public: /*Accessors*/
	const bool WantsToQuit() const noexcept { return m_quit; }
	const bool IsLoading() const noexcept { return m_isLoading; }
	std::mutex& GetMutex() noexcept { return m_mutex; }

public: /*Modifiers*/
	void EndScene() noexcept { m_quit = true; }
	void SetLoading(const bool loading) noexcept { m_isLoading = loading; }

protected:
	std::mutex m_mutex{}; // each scene has a mutex, which will guard code running by worker thread seperated from UI main thread. | used by std::scoped_lock
	bool m_quit{};
	bool m_isLoading{};
};

NS_ENIGMA_END

#endif // !ENIGMA_SCENE_H
