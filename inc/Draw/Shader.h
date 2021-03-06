#pragma once
#include <string>
#include <map>
#include <GL/gl3w.h>
#include <glm/glm.hpp>
#include <vector>

namespace VoxEngine
{
	class Transform;
	class Material;
	class RenderingEngine;
	class Shader
	{
	public:
		Shader(int id);
		~Shader();

		void AttachShader(GLenum shaderType, std::string filename);
		void Finish();

		void Bind();

		int GetUniformLocation(std::string uniform);

		void SetUniform1i(std::string uniform, int i);
		void SetUniform2i(std::string uniform, int i1, int i2);
		void SetUniform3i(std::string uniform, int i1, int i2, int i3);
		void SetUniform4i(std::string uniform, int i1, int i2, int i3, int i4);

		void SetUniform1f(std::string uniform, float x);
		void SetUniform2f(std::string uniform, float x, float y);
		void SetUniform3f(std::string uniform, float x, float y, float z);
		void SetUniform4f(std::string uniform, float x, float y, float z, float w);

		void SetUniform1d(std::string uniform, double x);
		void SetUniform2d(std::string uniform, double x, double y);
		void SetUniform3d(std::string uniform, double x, double y, double z);
		void SetUniform4d(std::string uniform, double x, double y, double z, double w);

		void SetUniformMatrix4fv(std::string uniform, glm::mat4 matrix);

		void SetUniform2i(std::string uniform, glm::vec2 v);
		void SetUniform3i(std::string uniform, glm::vec3 v);
		void SetUniform4i(std::string uniform, glm::vec4 v);

		void SetUniform2f(std::string uniform, glm::vec2 v);
		void SetUniform3f(std::string uniform, glm::vec3 v);
		void SetUniform4f(std::string uniform, glm::vec4 v);

		void SetUniform2d(std::string uniform, glm::vec2 v);
		void SetUniform3d(std::string uniform, glm::vec3 v);
		void SetUniform4d(std::string uniform, glm::vec4 v);
	protected:
		std::string ReadText(std::string filename);
		void AddUniform(std::string name);

		GLuint _program;
		std::map<std::string, int> _uniforms;
	private:
		int _id;

		std::vector<GLuint> _shaders;

		std::vector<std::string> _uniformNames;
		std::vector<std::string> _uniformTypes;
	};
}
