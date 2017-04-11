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

		float constant = 1;
		float linear = 0.00005f;
		float exponent = 0.00005f;

		float range = 10;
	};
}
