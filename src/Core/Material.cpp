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

	void Material::Finalize()
	{
		std::string content = "";

		for (auto it = _textures.begin(); it != _textures.end(); it++)
		{
			content += (*it).first;
		}

		_hash = std::hash<std::string>{}(content);
	}
}
