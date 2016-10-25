#pragma once
#include <string>
#include <map>

class Mesh;
class MeshManager
{
public:
	static void Init();
	static MeshManager* GetInstance();
	static void Shutdown();

	Mesh* GetMesh(std::string name);
private:
	MeshManager();
	~MeshManager();

	bool LoadMesh(std::string name);

	static MeshManager* _instance;

	std::map<std::string, Mesh*> _meshes;
};

