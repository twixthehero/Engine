#pragma once
#include "Core\Component.h"
#include <glew\glew.h>

class Mesh;
class Material;
class MeshRenderer :
	public Component
{
public:
	MeshRenderer(Mesh* mesh, Material* material);
	~MeshRenderer();

	void Render() override;

	void Reupload();
private:
	void CreateBuffers();
	void DeleteBuffers();
	void BufferData();

	Mesh* _mesh = nullptr;
	Material* _material = nullptr;

	bool _needReupload = false;
	GLuint vao;
	GLuint vbo;
	GLuint ibo;
};

