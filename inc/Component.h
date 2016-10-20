#pragma once
#include "GameObject.h"

class GameObject;
class Component
{
public:
	Component();
	~Component();

	GameObject* gameObject;
};

