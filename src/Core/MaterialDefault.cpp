#include <GL/gl3w.h>
#include "Core/MaterialDefault.h"
#include "ShaderManager.h"
#include "Core/Texture.h"
#include "Draw/Shader.h"
#include "TextureManager.h"
#include <string>

namespace VoxEngine
{
	MaterialDefault::MaterialDefault(Texture* albedo, Texture* specular)
	{
		if (albedo != nullptr)
			_textures.insert(std::pair<std::string, Texture*>("albedo", albedo));
		if (specular != nullptr)
			_textures.insert(std::pair<std::string, Texture*>("specular", specular));

		Finalize();
	}

	MaterialDefault::~MaterialDefault()
	{
	}

	void MaterialDefault::Use()
	{
		Shader* shader = ShaderManager::GetInstance()->GetCurrentShader();
		shader->SetUniform3f("albedoColor", albedoColor);
		shader->SetUniform3f("specularColor", specularColor);

		if (GetTexture("albedo") != -1)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, GetTexture("albedo"));
			shader->SetUniform1i("albedo", 0);
		}

		if (GetTexture("specular") != -1)
		{
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, GetTexture("specular"));
			shader->SetUniform1i("specular", 1);
		}
	}
}