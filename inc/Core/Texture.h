#pragma once
#include <glew\glew.h>

class Texture
{
public:
	Texture(GLuint id);
	~Texture();

	GLuint GetID();
private:
	GLuint _id;
};

