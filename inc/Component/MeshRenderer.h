#pragma once
#include "Core/Component.h"
#include <GL/gl3w.h>

namespace VoxEngine
{
	class Mesh;
	class Material;
	class MeshRenderer :
		public Component
	{
	public:
		MeshRenderer(Mesh* mesh, Material* material);
		~MeshRenderer();

		bool operator<(const MeshRenderer& otherRenderer) const
		{
			return mesh < otherRenderer.mesh && material < otherRenderer.material;
		}

		Mesh* mesh = nullptr;
		Material* material = nullptr;
	};
}
