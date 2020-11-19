#include <pch.hpp>
#include "Scene.hpp"

NS_ENIGMA_BEGIN


Scene::Scene()
	:
	m_scene_data(nullptr),
	m_quit(false),
	m_isPaused(false)

{

}
Scene::Scene(SceneData& scene_data)
	:
	m_scene_data(&scene_data),
	m_quit(false),
	m_isPaused(false)
{
}

//
//void Scene::SetSceneData(SceneData* scene_data) noexcept
//{
//	m_scene_data = scene_data;
//}
//

//void Scene::PushScene(Scene* scene) noexcept
//{
//	ENIGMA_CORE_ASSERT(scene, "Scene is nullptr");
//
//	scene->SetSceneData(m_scene_data);
//	m_scene_data->scenes->emplace_back(scene);
//
//	//Notify user
//	scene->OnCreate();
//}


void Scene::PauseScene()
{
	this->m_isPaused = true;
}

void Scene::ResumeScene()
{
	this->m_isPaused = false;
}


void Scene::EndScene()
{
	this->m_quit = true;
}

Scene::~Scene()
{
}

NS_ENIGMA_END


