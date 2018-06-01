#pragma once
#include "Component/Light.h"

namespace VoxEngine
{
	class SpotLight :
		public Light
	{
	public:
		SpotLight();
		virtual ~SpotLight();

		float angle = 30;
		float range = 10;
	};
}