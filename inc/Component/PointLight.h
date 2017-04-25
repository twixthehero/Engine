#pragma once
#include "Component\Light.h"

namespace VoxEngine
{
	class PointLight :
		public Light
	{
	public:
		PointLight();
		~PointLight();

		float constant = 1.0f;
		float linear = 0.2f;
		float exponent = 0.2f;

		float range = 10;
	};
}
