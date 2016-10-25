#pragma once
#include <glew\glew.h>

class Texture
{
public:
	Texture(GLuint id);
	~Texture();
private:
	GLuint _id;
};

