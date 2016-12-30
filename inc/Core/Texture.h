#pragma once
#include <GL\gl3w.h>

namespace VoxEngine
{
	class Texture
	{
	public:
		Texture(GLuint id);
		~Texture();

		GLuint GetID();
	private:
		GLuint _id;
	};
}
