#pragma once
#include <map>
#include <GL\gl3w.h>

namespace VoxEngine
{
	class Texture;
	class Material
	{
	public:
		Material();
		virtual ~Material();

		virtual void Use() = 0;

		GLuint GetTexture(std::string name);
	protected:
		std::map<std::string, Texture*> _textures = std::map<std::string, Texture*>();
	};
}
