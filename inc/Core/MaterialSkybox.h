#pragma once
#include "Material.h"
#include <string>

namespace VoxEngine
{
	class MaterialSkybox :
		public Material
	{
	public:
		MaterialSkybox(std::string name) : MaterialSkybox(name, "front.png", "back.png", "left.png", "right.png", "up.png", "down.png") { }
		MaterialSkybox(std::string name,
			std::string front, std::string back,
			std::string left, std::string right,
			std::string top, std::string bottom);
		virtual ~MaterialSkybox();

		void Use() override;
	};
}
