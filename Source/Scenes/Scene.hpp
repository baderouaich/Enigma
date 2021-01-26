#pragma once
#ifndef ENIGMA_SCENE_H
#define ENIGMA_SCENE_H

#include <Core/Core.hpp>
#include <Logger/Logger.hpp>
#include <Window/Window.hpp>
#include <Events/Event.hpp>

NS_ENIGMA_BEGIN

class ENIGMA_API Scene
{
public:
	friend class Application;

	ENIGMA_NON_COPYABLE(Scene);
	ENIGMA_NON_MOVEABLE(Scene);

public:
    Scene() noexcept;
   // Scene(SceneData& scene_data)  noexcept;
    virtual ~Scene() noexcept;

	/* Scene Life Cicle */
	virtual void OnCreate() = 0;
	virtual void OnEvent(Event& event) = 0;
	virtual void OnUpdate(const f32& dt) = 0;
	virtual void OnDraw() = 0;
	virtual void OnImGuiDraw() = 0;
	virtual void OnDestroy() = 0;

public: /* Controlls */
	/* Ends current Scene */
	void EndScene() noexcept;
	
	/* Pauses current Scene */
	//void PauseScene() noexcept;
	
	/* Resumes current Scene */
	//void ResumeScene() noexcept;

public: /*Accessors*/
	constexpr const bool& WantsToQuit() const noexcept { return m_quit; }
	//constexpr const bool& IsPaused() const noexcept { return m_isPaused; }
	//constexpr const SceneData& GetSceneData() const noexcept { return *m_scene_data; }


public: /*Modifiers*/


protected:
	bool m_quit;
    //SceneData* m_scene_data;
	//bool m_isPaused;
};

NS_ENIGMA_END

#endif // !ENIGMA_SCENE_H
