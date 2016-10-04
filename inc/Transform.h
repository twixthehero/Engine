#pragma once
#include <glm\glm.hpp>
#include "Component.h"

class Transform :
	public Component
{
public:
	Transform();
	~Transform();

	Transform* parent;
	Transform* root;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};

