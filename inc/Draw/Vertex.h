#pragma once
#include <glm/glm.hpp>
#include <iostream>

namespace VoxEngine
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec2 uv;
		glm::vec3 normal;

		bool operator==(const Vertex& o) const
		{
			return position == o.position &&
				uv == o.uv &&
				normal == o.normal;
		}

		friend std::ostream& operator<<(std::ostream& os, const Vertex& v);
	};

	struct VertexCompare
	{
		bool operator() (const Vertex& v1, const Vertex& v2) const
		{
			if (v1.normal.z < v2.normal.z) return true;
			if (v1.normal.z > v2.normal.z) return false;

			if (v1.normal.y < v2.normal.y) return true;
			if (v1.normal.y > v2.normal.y) return false;

			if (v1.normal.x < v2.normal.x) return true;
			if (v1.normal.x > v2.normal.x) return false;

			if (v1.uv.y < v2.uv.y) return true;
			if (v1.uv.y > v2.uv.y) return false;

			if (v1.uv.x < v2.uv.x) return true;
			if (v1.uv.x > v2.uv.x) return false;

			if (v1.position.z < v2.position.z) return true;
			if (v1.position.z > v2.position.z) return false;

			if (v1.position.y < v2.position.y) return true;
			if (v1.position.y > v2.position.y) return false;

			if (v1.position.x < v2.position.x)
				return true;

			return false;
		}
	};
}
