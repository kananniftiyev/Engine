#include "core/SceneManager.hpp"

SceneManager::SceneManager()
{
	m_scenes.reserve(20);
}

SceneManager::~SceneManager()
{
}

Scene SceneManager::CreateScene()
{
	return Scene();
}

void SceneManager::ChangeScene()
{
}

void SceneManager::UseCurrentScene()
{
	current_scene.Update();
	current_scene.Render();
}

void SceneManager::SaveSceneToDisk()
{

}
