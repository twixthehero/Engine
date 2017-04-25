#include "Component\Oscillate.h"
#include "Core\GameObject.h"
#include "Component\Transform.h"
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
		centerPosition = gameObject->transform->position;
	}

	void Oscillate::Update()
	{
		switch (axis)
		{
			case EAxis::X:
				gameObject->transform->position.x = centerPosition.x + sin(Time::time) * distance;
				break;
			case EAxis::Y:
				gameObject->transform->position.y = centerPosition.y + sin(Time::time) * distance;
				break;
			case EAxis::Z:
				gameObject->transform->position.z = centerPosition.z + sin(Time::time) * distance;
				break;
		}
	}
}
