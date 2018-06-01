#include "Component/Rotate.h"
#include "EngineTime.h"
#include "Core/GameObject.h"
#include "Component/Transform.h"
#include <glm/glm.hpp>

namespace VoxEngine
{
	Rotate::Rotate(EAxis axis, float rotateSpeed)
	{
		this->axis = axis;
		this->rotateSpeed = rotateSpeed;
	}

	Rotate::~Rotate()
	{
	}

	void Rotate::Update()
	{
		switch (axis)
		{
			case EAxis::X:
				gameObject->transform->Rotate(glm::vec3(1, 0, 0), rotateSpeed * Time::deltaTime);
				break;
			case EAxis::Y:
				gameObject->transform->Rotate(glm::vec3(0, 1, 0), rotateSpeed * Time::deltaTime);
				break;
			case EAxis::Z:
				gameObject->transform->Rotate(glm::vec3(0, 0, 1), rotateSpeed * Time::deltaTime);
				break;
		}
	}
}