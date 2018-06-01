#include <GL/gl3w.h>
#include "Core/MaterialSkybox.h"
#include "ShaderManager.h"
#include "Core/Texture.h"
#include "Draw/Shader.h"
#include "TextureManager.h"
#include <string>

namespace VoxEngine
{
	MaterialSkybox::MaterialSkybox(std::string name,
		std::string front, std::string back,
		std::string left, std::string right,
		std::string top, std::string bottom)
	{
		TextureManager::GetInstance()->LoadTextureCube(name, front, back, left, right, top, bottom);
		_textures.insert(std::pair<std::string, Texture*>("skybox", TextureManager::GetInstance()->GetTexture(name)));
		Finalize();
	}

	MaterialSkybox::~MaterialSkybox()
	{
	}

	void MaterialSkybox::Use()
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, _textures["skybox"]->GetID());
		ShaderManager::GetInstance()->GetCurrentShader()->SetUniform1i("skybox", 0);
	}
}