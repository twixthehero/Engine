#pragma once
#include <vector>
#include "Component.h"

class Component;
class GameObject
{
public:
	GameObject();
	~GameObject();

	void AddComponent(Component* component);
private:
	std::vector<Component*> _components;
};

