#include "Core\Material.h"
#include "Draw\Shader.h"
#include "ShaderManager.h"
#include "Core\Texture.h"

namespace VoxEngine
{
	Material::Material()
	{
	}

	Material::Material(Texture* diffuseTexture)
	{
		_textures.insert(std::pair<std::string, Texture*>("diffuse", diffuseTexture));
	}

	Material::~Material()
	{
	}

	void Material::Use()
	{
		if (_textures.find("diffuse") != _textures.end())
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, _textures["diffuse"]->GetID());
			ShaderManager::GetInstance()->GetCurrentShader()->SetUniform1i("diffuse", 0);
		}
	}

	Texture* Material::GetTexture()
	{
		if (_textures.size() > 0)
			return _textures[0];

		return nullptr;
	}
}
