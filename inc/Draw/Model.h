#pragma once
#include "Mesh.h"

class Model
{
public:
	Model(Mesh* mesh);
	~Model();
private:
	Mesh* _mesh;
};

