#pragma once
#include <map>

class Shader;
class Texture;
class Transform;
class Material
{
public:
	Material();
	Material(Shader* shader);
	Material(Shader* shader, Texture* texture);
	~Material();

	void Use(Transform* transform);

	Shader* GetShader();
	Texture* GetTexture();
private:
	Shader* _shader;
	std::map<std::string, Texture*> _textures = std::map<std::string, Texture*>();
};

