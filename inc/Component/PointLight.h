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

		float range = 10;
	};
}
