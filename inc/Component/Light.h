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

		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		float intensity = 1;
	};
}
