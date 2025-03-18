#pragma once

#include "core/Scene.hpp"
#include <vector>

class SceneManager
{
public:

	SceneManager();
	~SceneManager();

	Scene CreateScene();
	void ChangeScene();
	void UseCurrentScene();

private:
	int scene_count = 0;
	Scene current_scene;
	std::vector<Scene> m_scenes;

	void SaveSceneToDisk();
};
