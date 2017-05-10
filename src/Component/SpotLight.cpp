#include "Component\SpotLight.h"

namespace VoxEngine
{
	SpotLight::SpotLight()
	{
		_componentType = EComponentType::LIGHT_SPOT;
	}

	SpotLight::~SpotLight()
	{
	}
}