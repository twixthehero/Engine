#include "Draw/Mesh.h"
#include "Core/Material.h"
#include "Draw/Vertex.h"

namespace VoxEngine
{
	Mesh::Mesh(int id)
	{
		_id = id;
	}

	Mesh::~Mesh()
	{
		DeleteBuffers();

		if (data != nullptr)
		{
			delete data;
			data = nullptr;
		}
	}

	int Mesh::CalcVertexSize()
	{
		//position + uv + normal
		return 3 + 2 + 3;
	}

	void Mesh::Finalize()
	{
		CreateData();
		CreateBuffers();
		BufferData();
	}

	void Mesh::CreateData()
	{
		if (data != nullptr)
			delete[] data;

		data = new float[vertices.size() * CalcVertexSize()];
		int index = 0;

		for (int i = 0; i < vertices.size(); i++)
		{
			data[index++] = vertices[i].position.x;
			data[index++] = vertices[i].position.y;
			data[index++] = vertices[i].position.z;

			data[index++] = vertices[i].uv.x;
			data[index++] = vertices[i].uv.y;

			data[index++] = vertices[i].normal.x;
			data[index++] = vertices[i].normal.y;
			data[index++] = vertices[i].normal.z;
		}
	}

	void Mesh::Render(Material* material)
	{
		glBindVertexArray(vao);

		if (material != nullptr)
			material->Use();

		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
	}

	void Mesh::Render(int instanceCount, Material* material, const glm::mat4* data)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _mbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * instanceCount, data, GL_DYNAMIC_DRAW);

		glBindVertexArray(vao);

		if (material != nullptr)
			material->Use();

		glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, instanceCount);

		glBindVertexArray(0);
	}

	void Mesh::CreateBuffers()
	{
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ibo);
		glGenBuffers(1, &_mbo);
	}

	void Mesh::DeleteBuffers()
	{
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ibo);
		glDeleteBuffers(1, &_mbo);
	}

	void Mesh::BufferData()
	{
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * CalcVertexSize() * vertices.size(), data, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(), &indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), 0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(Vertex), (void*)12);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(Vertex), (void*)20);

		glBindBuffer(GL_ARRAY_BUFFER, _mbo);
		for (int i = 0; i < 4; i++)
		{
			glEnableVertexAttribArray(3 + i);
			glVertexAttribPointer(3 + i, 4, GL_FLOAT, false, sizeof(glm::mat4), (const void*)(sizeof(float) * i * 4));
			glVertexAttribDivisor(3 + i, 1);
		}
	}

	void Mesh::Reupload()
	{
		CreateData();
		BufferData();
	}
}
