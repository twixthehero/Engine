#include "Draw\Mesh.h"
#include "Draw\Vertex.h"


Mesh::Mesh()
{
}


Mesh::~Mesh()
{
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