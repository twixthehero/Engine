#include "Component\MeshRenderer.h"
#include "Draw\Mesh.h"
#include "Core\Material.h"
#include "Core\GameObject.h"
#include <iostream>

MeshRenderer::MeshRenderer(Mesh* mesh, Material* material)
{
	_mesh = mesh;
	_material = material;

	_mesh->CreateData();
	CreateBuffers();
	BufferData();
}


MeshRenderer::~MeshRenderer()
{
	DeleteBuffers();
}

void MeshRenderer::Update()
{
	if (_needReupload)
	{
		_needReupload = false;

		_mesh->CreateData();
		BufferData();
	}
}

void MeshRenderer::Render(RenderingEngine* renderingEngine)
{
	_material->Use(gameObject->transform, renderingEngine);

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	//glDrawElements(GL_TRIANGLES, _mesh->indices.size(), GL_UNSIGNED_INT, 0);
	//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
}

void MeshRenderer::Reupload()
{
	_needReupload = true;
}

void MeshRenderer::CreateBuffers()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);
}

void MeshRenderer::DeleteBuffers()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

void MeshRenderer::BufferData()
{
	glBindVertexArray(vao);

	float data[] =
	{
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f
	};
	/*
	float data[] =
	{
		-1, -1, 0,
		1, -1, 0,
		-1, 1, 0,
		-1, 1, 0,
		1, -1, 0,
		1, 1, 0
	};
	/*int indices[] =
	{
		0, 1, 2
	};*/

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glBufferData(GL_ARRAY_BUFFER, _mesh->CalcVertexSize() * _mesh->indices.size(), _mesh->data, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * _mesh->indices.size(), &_mesh->indices[0], GL_STATIC_DRAW);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	int index = 0;

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);

	/*if (_mesh->hasUVs)
	{
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index++, 2, GL_FLOAT, false, 0, 0);
	}

	if (_mesh->hasNormals)
	{
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index++, 3, GL_FLOAT, false, 0, 0);
	}*/
}