#include "Component\DirectionalLight.h"
#include "Draw\RenderingEngine.h"

namespace VoxEngine
{
	DirectionalLight::DirectionalLight()
	{
		_componentType = EComponentType::LIGHT_DIRECTIONAL;

		RenderingEngine::GetInstance()->AddDirectionalLight(this);
	}

	DirectionalLight::~DirectionalLight()
	{
		RenderingEngine::GetInstance()->RemoveDirectionalLight(this);
	}
}
