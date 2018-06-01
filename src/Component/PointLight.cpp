#include "Component/PointLight.h"
#include "Draw/RenderingEngine.h"

namespace VoxEngine
{
	PointLight::PointLight()
	{
		_componentType = EComponentType::LIGHT_POINT;

		RenderingEngine::GetInstance()->AddPointLight(this);
	}

	PointLight::~PointLight()
	{
		RenderingEngine::GetInstance()->RemovePointLight(this);
	}
}
