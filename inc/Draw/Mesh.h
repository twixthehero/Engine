#pragma once
#include <vector>
#include <glm\glm.hpp>

namespace VoxEngine
{
	struct Vertex;
	class Mesh
	{
	public:
		Mesh();
		~Mesh();

		int CalcVertexSize();

		void CreateData();

		std::vector<Vertex> vertices;
		std::vector<int> indices;
		float* data = nullptr;
	};
}
