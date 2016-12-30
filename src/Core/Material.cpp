#include "Core\Material.h"
#include "Draw\Shader.h"
#include "Core\Texture.h"

namespace VoxEngine
{
	Material::Material()
	{
	}

	Material::Material(Shader* shader)
	{
		_shader = shader;
	}

	Material::Material(Shader* shader, Texture* diffuseTexture)
	{
		_shader = shader;
		_textures.insert(std::pair<std::string, Texture*>("diffuse", diffuseTexture));
	}

	Material::~Material()
	{
	}

	void Material::Use(Transform* transform)
	{
		_shader->Bind();

		glBindTexture(GL_TEXTURE_2D, _textures["diffuse"]->GetID());

		_shader->UpdateUniforms(transform);
	}

	Shader* Material::GetShader()
	{
		return _shader;
	}

	Texture* Material::GetTexture()
	{
		if (_textures.size() > 0)
			return _textures[0];

		return nullptr;
	}
}
