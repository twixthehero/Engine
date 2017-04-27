#pragma once
#include <string>
#include <GL\gl3w.h>
#include <map>

namespace VoxEngine
{
	class Texture;
	class TextureManager
	{
	public:
		static void Init();
		static TextureManager* GetInstance();
		static void Shutdown();

		Texture* GetTexture(std::string name);
		Texture* GetTextureCube(std::string name);

		bool LoadTextureCube(std::string name,
			std::string front, std::string back,
			std::string left, std::string right,
			std::string top, std::string bottom);
	private:
		TextureManager();
		~TextureManager();

		bool LoadTexture(std::string name);

		static TextureManager* _instance;

		Texture* _missingTexture;
		Texture* _missingTextureCube;
		std::map<std::string, Texture*> _textures;

		GLenum _minificationFilter = GL_LINEAR;
		GLenum _magnificationFilter = GL_LINEAR;
	};
}
