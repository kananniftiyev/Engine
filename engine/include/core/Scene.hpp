#pragma once

#include <string>

class Scene
{
public:

	void Update();
	void Render();

private:
	std::string scene_name;
};