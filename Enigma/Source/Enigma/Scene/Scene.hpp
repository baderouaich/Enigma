#pragma once
#ifndef ENIGMA_SCENE_H
#define ENIGMA_SCENE_H

#include <Enigma/Core/Core.hpp>
#include <Enigma/Debug/Logger/Logger.hpp>
#include <Enigma/Window/Window.hpp>
#include <Enigma/Events/Event.hpp>

NS_ENIGMA_BEGIN

class Scene;
struct SceneData
{
	Window* window;
	f32* delta_time;
	std::vector<Scene*>* scenes;
	ui32 *FPS;
};

class ENIGMA_API Scene : public CreatePtr<Scene>
{
public:
	friend class Application;

public:
    Scene();
    Scene(SceneData& scene_data);
    virtual ~Scene();

	ENIGMA_NON_COPYABLE(Scene);
	ENIGMA_NON_MOVEABLE(Scene);

	/*Scene Life Cicle*/
	virtual void OnCreate() = 0;
	virtual void OnEvent(Event& event) = 0;
	virtual void OnUpdate(const f32& dt) = 0;
	virtual void OnDraw() = 0;
	virtual void OnImGuiDraw() = 0;
	virtual void OnDestroy() = 0;

public: /* Controlls */
	/* Ends current Scene */
	virtual void EndScene();
	
	/* Pauses current Scene */
	void PauseScene();
	
	/* Resumes current Scene */
	void ResumeScene();

public: /*Accessors*/
	constexpr const bool& WantsQuit() const noexcept { return m_quit; }
	constexpr const bool& IsPaused() const noexcept { return m_isPaused; }
	constexpr const SceneData& GetSceneData() const noexcept { return *m_scene_data; }


public: /*Modifiers*/


protected:
    SceneData* m_scene_data;
	bool m_quit;
	bool m_isPaused;
};

NS_ENIGMA_END

#endif // !ENIGMA_SCENE_H
