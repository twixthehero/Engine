#pragma once
#include "GameObject.h"

class Component
{
public:
	Component();
	~Component();

	GameObject* gameObject;
};

