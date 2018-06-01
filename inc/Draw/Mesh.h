#pragma once
#include <vector>
#include <GL/gl3w.h>
#include <glm/glm.hpp>

namespace VoxEngine
{
	struct Vertex;
	class Material;
	class Mesh
	{
	public:
		Mesh(int id);
		~Mesh();

		void Finalize();

		void Render(Material* material = nullptr);
		void Render(int instanceCount, Material* material, const glm::mat4* data);

		std::vector<Vertex> vertices;
		std::vector<int> indices;
		float* data = nullptr;

		bool operator<(const Mesh& otherMesh) const
		{
			return _id < otherMesh._id;
		}

		bool operator==(const Mesh& otherMesh) const
		{
			return _id == otherMesh._id;
		}

		bool operator!=(const Mesh& otherMesh) const
		{
			return _id != otherMesh._id;
		}
	private:
		void CreateData();
		void CreateBuffers();
		void DeleteBuffers();
		void BufferData();
		void Reupload();

		int CalcVertexSize();

		int _id;
		bool _needReupload = false;
		GLuint vao;
		GLuint vbo;
		GLuint ibo;
		GLuint _mbo;
	};
}
