#pragma once
#include <vector>
#include <glm\glm.hpp>

struct Vertex;
class Mesh
{
public:
	Mesh();
	~Mesh();

	int CalcVertexSize();

	void CreateData();

	bool hasUVs;
	bool hasNormals;

	std::vector<Vertex> vertices;
	std::vector<int> indices;
	float* data = nullptr;
};

