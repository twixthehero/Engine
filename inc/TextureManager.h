#pragma once
#include <string>
#include <map>
#include <glew\glew.h>

class Texture;
class TextureManager
{
public:
	static void Init();
	static TextureManager* GetInstance();
	static void Shutdown();

	Texture* GetTexture(std::string name);
private:
	TextureManager();
	~TextureManager();

	bool LoadTexture(std::string name);

	static TextureManager* _instance;

	Texture* _missingTexture;
	std::map<std::string, Texture*> _textures;

	GLenum _minificationFilter = GL_LINEAR;
	GLenum _magnificationFilter = GL_LINEAR;
};

