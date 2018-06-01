#include "Component/MeshRenderer.h"
#include "Draw/Mesh.h"
#include "Core/Material.h"
#include "Core/GameObject.h"
#include "Component/Transform.h"
#include "Draw/Shader.h"
#include "ShaderManager.h"
#include "Draw/Vertex.h"
#include "Draw/RenderingEngine.h"

namespace VoxEngine
{
	MeshRenderer::MeshRenderer(Mesh* mesh, Material* material)
	{
		_componentType = EComponentType::MESH_RENDERER;

		this->mesh = mesh;
		this->material = material;

		RenderingEngine::GetInstance()->AddRenderer(this);
	}

	MeshRenderer::~MeshRenderer()
	{
		RenderingEngine::GetInstance()->RemoveRenderer(this);
	}
}
