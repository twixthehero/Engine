#pragma once
#include <map>

class Shader;
class Texture;
class Material
{
public:
	Material();
	~Material();
private:
	Shader* _shader;
	std::map<std::string, Texture*> _textures;
};

