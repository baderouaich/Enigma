#include <pch.hpp>
#include "Scene.hpp"

NS_ENIGMA_BEGIN


Scene::Scene() noexcept
	:
	m_quit(false)
	//,m_isPaused(false)
{

}

#if 0
Scene::Scene(SceneData& scene_data) noexcept
	: 
	m_scene_data(&scene_data),
	m_quit(false),
	m_isPaused(false)
{
}


void Scene::SetSceneData(SceneData* scene_data) noexcept
{
	m_scene_data = scene_data;
}


void Scene::PushScene(Scene* scene) noexcept
{
	ENIGMA_ASSERT(scene, "Scene is nullptr");

	scene->SetSceneData(m_scene_data);
	m_scene_data->scenes->emplace_back(scene);

	//Notify user
	scene->OnCreate();
}


void Scene::PauseScene() noexcept
{
	this->m_isPaused = true;
}

void Scene::ResumeScene() noexcept
{
	this->m_isPaused = false;
}
#endif

void Scene::EndScene() noexcept
{
	this->m_quit = true;
}

Scene::~Scene()  noexcept
{
}

NS_ENIGMA_END


