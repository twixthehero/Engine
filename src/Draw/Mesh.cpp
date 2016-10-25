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
	return 3 + (hasUVs ? 2 : 0) + (hasNormals ? 3 : 0);
}

void Mesh::CreateData()
{
	data = new float[indices.size() * CalcVertexSize()];
	int index = 0;

	for (int i = 0; i < indices.size(); i++)
	{
		data[index++] = vertices[indices[i]].position.x;
		data[index++] = vertices[indices[i]].position.y;
		data[index++] = vertices[indices[i]].position.z;

		if (hasUVs)
		{
			data[index++] = vertices[indices[i]].uv.x;
			data[index++] = vertices[indices[i]].uv.y;
		}

		if (hasNormals)
		{
			data[index++] = vertices[indices[i]].normal.x;
			data[index++] = vertices[indices[i]].normal.y;
			data[index++] = vertices[indices[i]].normal.z;
		}
	}
}