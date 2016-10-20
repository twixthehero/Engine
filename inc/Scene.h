#pragma once
#include "GameObject.h"
#include <string>

class Scene
{
public:
	Scene();
	~Scene();

	void AddObject(GameObject* object);
	void RemoveObject(GameObject* object);

	GameObject* Find(std::string name);

	void Update();
	void Render();
private:
	GameObject* root;
};

