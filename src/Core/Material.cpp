#include "Core\Material.h"



Material::Material()
{
	_textures = std::map<std::string, Texture*>();
}


Material::~Material()
{
}
