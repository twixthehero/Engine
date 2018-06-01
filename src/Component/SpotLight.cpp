#include "Component/SpotLight.h"
#include "Draw/RenderingEngine.h"

namespace VoxEngine
{
	SpotLight::SpotLight()
	{
		_componentType = EComponentType::LIGHT_SPOT;

		RenderingEngine::GetInstance()->AddSpotLight(this);
	}

	SpotLight::~SpotLight()
	{
		RenderingEngine::GetInstance()->RemoveSpotLight(this);
	}
}