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

	void Oscillate::Update()
	{
		switch (axis)
		{
			case EAxis::X:
				gameObject->transform->position.x = sin(Time::time) * distance;
				break;
			case EAxis::Y:
				gameObject->transform->position.y = sin(Time::time) * distance;
				break;
			case EAxis::Z:
				gameObject->transform->position.z = sin(Time::time) * distance;
				break;
		}
	}
}
