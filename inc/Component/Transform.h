#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>
#include "Core\Component.h"

class Transform :
	public Component
{
public:
	Transform();
	~Transform();

	glm::vec3 GetForward();
	glm::vec3 GetRight();
	glm::vec3 GetUp();

	glm::mat4 GetModelMatrix();

	Transform* parent;
	Transform* root;

	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;
};

