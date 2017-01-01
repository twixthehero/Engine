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

		void Use();

		Texture* GetTexture();
	private:
		std::map<std::string, Texture*> _textures = std::map<std::string, Texture*>();
	};
}
