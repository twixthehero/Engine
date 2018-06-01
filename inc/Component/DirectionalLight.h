#pragma once
#include "Component/Light.h"

namespace VoxEngine
{
	class DirectionalLight :
		public Light
	{
	public:
		DirectionalLight();
		~DirectionalLight();
	};
}
