#pragma once
#include <map>

namespace VoxEngine
{
	class Texture;
	class Material
	{
	public:
		Material();
		Material(Texture* texture);
		~Material();

		virtual void Use();

		Texture* GetTexture(std::string name);
	protected:
		std::map<std::string, Texture*> _textures = std::map<std::string, Texture*>();
	};
}
