#pragma once
#include "Core\Component.h"
#include <glm\glm.hpp>

namespace VoxEngine
{
	class Light :
		public Component
	{
	public:
		Light();
		virtual ~Light();

		glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		float range = 10;
		float intensity = 1;
	};
}
