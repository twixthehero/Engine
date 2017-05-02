#include "Core\Material.h"
#include "Core\Texture.h"

namespace VoxEngine
{
	Material::Material()
	{
	}

	Material::~Material()
	{
	}

	GLuint Material::GetTexture(std::string name)
	{
		if (_textures.find(name) != _textures.end())
			return _textures[name]->GetID();

		return -1;
	}
}
