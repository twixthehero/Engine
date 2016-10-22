#include "Component\MeshRenderer.h"



MeshRenderer::MeshRenderer()
{
}

MeshRenderer::MeshRenderer(Mesh* mesh, Material* material)
{
	_mesh = mesh;
	_material = material;
}


MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::Render(RenderingEngine* renderingEngine)
{

}