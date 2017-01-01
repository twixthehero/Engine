#include "Component\DirectionalLight.h"

namespace VoxEngine
{
	DirectionalLight::DirectionalLight()
	{
		_componentType = EComponentType::LIGHT_DIRECTIONAL;
	}

	DirectionalLight::~DirectionalLight()
	{
	}
}
