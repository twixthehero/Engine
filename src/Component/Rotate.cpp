#include "Component\Rotate.h"
#include "EngineTime.h"
#include "Core\GameObject.h"
#include "Component\Transform.h"
#include <glm\glm.hpp>

namespace VoxEngine
{
	Rotate::Rotate(float rotateSpeed)
	{
		this->rotateSpeed = rotateSpeed;
	}

	Rotate::~Rotate()
	{
	}

	void Rotate::Update()
	{
		gameObject->transform->Rotate(glm::vec3(0, 1, 0), rotateSpeed * Time::deltaTime);
	}
}