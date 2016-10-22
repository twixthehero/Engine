#pragma once
#include "Core\Component.h"

class Mesh;
class Material;
class MeshRenderer :
	public Component
{
public:
	MeshRenderer();
	MeshRenderer(Mesh* mesh, Material* material);
	~MeshRenderer();

	void Render(RenderingEngine* renderingEngine) override;
private:
	Mesh* _mesh = nullptr;
	Material* _material = nullptr;
};

