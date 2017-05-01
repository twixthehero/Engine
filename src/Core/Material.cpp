#include "Core\Material.h"
#include "Draw\Shader.h"
#include "ShaderManager.h"
#include "Core\Texture.h"

namespace VoxEngine
{
	Material::Material()
	{
	}

	Material::Material(std::string name, Texture* texture)
	{
		_names.push_back(name);
		_textures.insert(std::pair<std::string, Texture*>(name, texture));
	}

	Material::~Material()
	{
	}

	void Material::Use()
	{
		int textureIndex = 0;
		for (auto name : _names)
		{
			if (_textures.find(name) != _textures.end())
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, _textures[name]->GetID());
				ShaderManager::GetInstance()->GetCurrentShader()->SetUniform1i(name, textureIndex++);
			}
		}
	}

	Texture* Material::GetTexture(std::string name)
	{
		if (_textures.find(name) != _textures.end())
			return _textures[name];

		return nullptr;
	}
}
