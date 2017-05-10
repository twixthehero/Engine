#pragma once
#include "Material.h"
#include <string>
#include <glm\glm.hpp>

namespace VoxEngine
{
	class MaterialDefault :
		public Material
	{
	public:
		MaterialDefault(Texture* albedo, Texture* specular);
		virtual ~MaterialDefault();

		void Use() override;

		glm::vec3 albedoColor = glm::vec3(1.0f);
		glm::vec3 specularColor = glm::vec3(51 / 255.0f);
	};
}
