#include "Component\MeshRenderer.h"
#include "Draw\Mesh.h"
#include "Core\Material.h"
#include "Core\GameObject.h"
#include "Draw\Vertex.h"
#include <iostream>
#include "Component\Transform.h"
#include <glm\glm.hpp>
#include "Input.h"

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
	glDrawElements(GL_TRIANGLES, _mesh->indices.size(), GL_UNSIGNED_INT, 0);
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

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _mesh->CalcVertexSize() * _mesh->vertices.size(), _mesh->data, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * _mesh->indices.size(), &_mesh->indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), 0);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(Vertex), (void*)12);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(Vertex), (void*)20);
	
}