#include "MeshManager.h"
#include <iostream>
#include <fstream>
#include "Utils.h"
#include <glm\glm.hpp>
#include "Draw\Mesh.h"
#include "Draw\Vertex.h"

MeshManager* MeshManager::_instance = nullptr;

MeshManager::MeshManager()
{
	_meshes = std::map<std::string, Mesh*>();
}


MeshManager::~MeshManager()
{
	for (std::map<std::string, Mesh*>::iterator it = _meshes.begin(); it != _meshes.end(); it++)
		delete it->second;
}

void MeshManager::Init()
{
	_instance = new MeshManager();
}

MeshManager* MeshManager::GetInstance()
{
	return _instance;
}

void MeshManager::Shutdown()
{
	if (_instance != nullptr)
	{
		delete _instance;
		_instance = nullptr;
	}
}

Mesh* MeshManager::GetMesh(std::string name)
{
	if (_meshes.find(name) == _meshes.end())
		if (!LoadMesh(name))
			return nullptr;

	return _meshes[name];
}

bool MeshManager::LoadMesh(std::string name)
{
	std::string path = "meshes\\" + name + ".obj";
	std::ifstream file(path);

	if (!file)
	{
		std::cout << "Unable to open file for read: " << path << std::endl;
		return false;
	}

	std::string line;

	std::vector<glm::vec3> vertices = std::vector<glm::vec3>();
	std::vector<glm::vec2> uvs = std::vector<glm::vec2>();
	std::vector<glm::vec3> normals = std::vector<glm::vec3>();
	
	std::vector<std::string> faces = std::vector<std::string>();

	while (std::getline(file, line))
	{
		std::vector<std::string> tokens = Utils::Split(line, ' ');
		std::string type = tokens[0];

		if (type == "#")
			continue;

		if (type == "v")
		{
			vertices.push_back(glm::vec3(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3])));
		}
		else if (type == "vt")
		{
			uvs.push_back(glm::vec2(std::stof(tokens[1]), std::stof(tokens[2])));
		}
		else if (type == "vn")
		{
			normals.push_back(glm::vec3(std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3])));
		}
		else if (type == "f")
		{
			faces.push_back(line);
		}
	}
	/*
	std::cout << "num verts: " << vertices.size() << std::endl;
	std::cout << "num uvs: " << uvs.size() << std::endl;
	std::cout << "num normals: " << normals.size() << std::endl;
	std::cout << "num faces: " << faces.size() << std::endl;
	*/
	std::vector<int> vertexIndices = std::vector<int>();
	std::vector<int> uvIndices = std::vector<int>();
	std::vector<int> normalIndices = std::vector<int>();

	for (int i = 0; i < faces.size(); i++)
	{
		std::vector<std::string> tokens = Utils::Split(faces[i].substr(2), ' ');
		bool needTriangulate = tokens.size() == 4;

		std::vector<int> vis = std::vector<int>();
		std::vector<int> uis = std::vector<int>();
		std::vector<int> nis = std::vector<int>();

		for (int i = 0; i < tokens.size(); i++)
		{
			std::vector<std::string> vert = Utils::Split(tokens[i], '/');
			int vertexIndex = std::stoi(vert[0]) - 1;
			int uvIndex = -1;
			int normalIndex = -1;

			if (vert[1].length() > 0)
				uvIndex = std::stoi(vert[1]) - 1;

			if (vert[2].length() > 0)
				normalIndex = std::stoi(vert[2]) - 1;

			vis.push_back(vertexIndex);
			uis.push_back(uvIndex);
			nis.push_back(normalIndex);
		}

		if (needTriangulate)
		{
			int indices[6] = { 0, 1, 2, 0, 3, 1 };

			for (int i = 0; i < sizeof(indices); i++)
			{
				vertexIndices.push_back(vis[indices[i]]);
				uvIndices.push_back(uis[indices[i]]);
				normalIndices.push_back(nis[indices[i]]);
			}
		}
		else
		{
			for (int i = 0; i < vis.size(); i++)
			{
				vertexIndices.push_back(vis[i]);
				uvIndices.push_back(uis[i]);
				normalIndices.push_back(nis[i]);
			}
		}
	}

	std::map<Vertex, int, VertexCompare> vertexMap = std::map<Vertex, int, VertexCompare>();
	std::vector<Vertex> vertex = std::vector<Vertex>();
	std::vector<int> indices = std::vector<int>();
	int index = 0;

	bool hasUVs = uvs.size() > 0;
	bool hasNormals = normals.size() > 0;

	for (int i = 0; i < vertexIndices.size(); i++)
	{
		Vertex v;
		v.position = vertices[vertexIndices[i]];

		if (hasUVs)
			v.uv = uvs[uvIndices[i]];

		if (hasNormals)
			v.normal = normals[normalIndices[i]];

		if (vertexMap.find(v) == vertexMap.end())
		{
			vertex.push_back(v);
			indices.push_back(index);
			vertexMap.insert(std::pair<Vertex, int>(v, index++));
		}
	}

	Mesh* mesh = new Mesh();
	mesh->vertices = vertex;
	mesh->indices = indices;
	mesh->hasUVs = hasUVs;
	mesh->hasNormals = hasNormals;

	_meshes.insert(std::pair<std::string, Mesh*>(name, mesh));

	return true;
}