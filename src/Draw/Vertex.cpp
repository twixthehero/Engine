#include "Draw/Vertex.h"

namespace VoxEngine
{
	std::ostream& operator<<(std::ostream& os, const Vertex& v)
	{
		os << "Vertex[";
		os << "pos=(" << v.position.x << "," << v.position.y << "," << v.position.z << "), ";
		os << "uv=(" << v.uv.x << "," << v.uv.y << "), ";
		os << "norm=(" << v.normal.x << "," << v.normal.y << "," << v.normal.z << ")";
		os << "]";

		return os;
	}
}
