#include "Core/Texture.h"

namespace VoxEngine
{
	Texture::Texture(GLuint id)
	{
		_id = id;
	}

	Texture::~Texture()
	{
	}

	GLuint Texture::GetID()
	{
		return _id;
	}
}