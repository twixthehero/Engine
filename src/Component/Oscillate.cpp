#include "Component/Oscillate.h"
#include "Core/GameObject.h"
#include "Component/Transform.h"
#include "EngineTime.h"
#include <cmath>

namespace VoxEngine
{
	Oscillate::Oscillate()
	{
	}

	Oscillate::Oscillate(EAxis axis, float distance)
	{
		this->axis = axis;
		this->distance = distance;
	}

	Oscillate::~Oscillate()
	{
	}

	void Oscillate::Init()
	{
		centerPosition = gameObject->transform->GetPosition();
	}

	void Oscillate::Update()
	{
		glm::vec3 pos = gameObject->transform->GetPosition();

		switch (axis)
		{
			case EAxis::X:
				pos.x = centerPosition.x + sin(Time::time) * distance;
				break;
			case EAxis::Y:
				pos.y = centerPosition.y + sin(Time::time) * distance;
				break;
			case EAxis::Z:
				pos.z = centerPosition.z + sin(Time::time) * distance;
				break;
		}

		gameObject->transform->SetPosition(pos);
	}
}
