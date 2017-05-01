#pragma once
#include <map>
#include <vector>

namespace VoxEngine
{
	class Texture;
	class Material
	{
	public:
		Material();
		Material(std::string name, Texture* texture);
		~Material();

		virtual void Use();

		Texture* GetTexture(std::string name);
	protected:
		std::vector<std::string> _names = std::vector<std::string>();
		std::map<std::string, Texture*> _textures = std::map<std::string, Texture*>();
	};
}
