#include "Core\Texture.h"



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