#pragma once
#include <vector>
#include "Component.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

	void AddComponent(Component* component);
	template<Component T> T* GetComponent();
private:
	std::vector<Component*> _components;
};

