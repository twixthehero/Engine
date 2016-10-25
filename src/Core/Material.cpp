#include "Core\Material.h"
#include "Draw\Shader.h"

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

void Material::Use(Transform* transform, RenderingEngine* renderingEngine)
{
	_shader->Bind();
	_shader->UpdateUniforms(transform, this, renderingEngine);
}

Texture* Material::GetTexture()
{
	if (_textures.size() > 0)
		return _textures[0];

	return nullptr;
}