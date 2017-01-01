#include "Component\PointLight.h"

namespace VoxEngine
{
	PointLight::PointLight()
	{
		_componentType = EComponentType::LIGHT_POINT;
	}

	PointLight::~PointLight()
	{
	}
}
