#include "Draw\Shader.h"
#include <fstream>
#include <iostream>
#include "Utils.h"
#include "Draw\RenderingEngine.h"
#include <glm\glm.hpp>
#include "Component\Transform.h"
#include "Component\Camera.h"
#include <glm\gtc\matrix_transform.hpp>
#include <Core\GameObject.h>
#include "Component\Light.h"
#include "Draw\ShaderStruct.h"
#include "Utils.h"
#include "Logger.h"

namespace VoxEngine
{
	Shader::Shader(int id)
	{
		_id = id;

		_program = glCreateProgram();

		if (_program == 0)
		{
			Logger::WriteLine("ERROR: Unable to create program space for shader");
			exit(-1);
		}
	}

	Shader::~Shader()
	{
	}

	void Shader::AttachShader(GLenum shaderType, std::string filename)
	{
		std::string content = ReadText(filename);
		const char* contentPtr = content.c_str();

		GLuint shader = glCreateShader(shaderType);
		glShaderSource(shader, 1, &contentPtr, NULL);
		glCompileShader(shader);

		GLint success = -1;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (success != GL_TRUE)
		{
			Logger::WriteLine("ERROR: GL " + std::to_string(shaderType) + " shader type of index " + std::to_string(shader) + " did not compile");

			int logLength;

			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

			if (logLength > 0)
			{
				char* errorMessage = new char[logLength + 1];
				glGetShaderInfoLog(shader, logLength, NULL, errorMessage);
				Logger::WriteLine(std::string(errorMessage));
				delete errorMessage;
			}

			exit(-1);
		}

		glAttachShader(_program, shader);

		_shaders.push_back(shader);
	}

	std::string Shader::ReadText(std::string filename)
	{
		std::ifstream file("shaders\\" + filename);
		std::string line;
		std::string text;

		std::string currentStruct;
		ShaderStruct* ss = nullptr;
		std::map<std::string, ShaderStruct*> structs;
		bool inStruct = false;

		if (file.is_open())
		{
			while (getline(file, line))
			{
				text += line + "\n";

				if (line.find("struct") == 0)
				{
					inStruct = true;
					currentStruct = Utils::Split(line, ' ')[1];
					Logger::WriteLine("=================================================");
					Logger::WriteLine("Found new shader struct: " + currentStruct);
					ss = new ShaderStruct(currentStruct);
					structs.insert(std::pair<std::string, ShaderStruct*>(currentStruct, ss));
					continue;
				}
				else if (line.find("};") == 0)
				{
					inStruct = false;
					Logger::WriteLine("End of shader struct");
					Logger::WriteLine("=================================================");
				}

				if (inStruct)
				{
					Utils::Trim(line);
					std::vector<std::string> parts = Utils::Split(line, ' ');

					if (parts.size() < 2) continue;

					std::string varType = parts[0];
					std::string varName = parts[1].substr(0, parts[1].length() - 1);

					//found struct type in struct
					if (structs.find(varType) != structs.end())
					{
						Logger::WriteLine("struct included another struct: " + varType);
						ShaderStruct* included = structs[varType];

						for (int i = 0; i < included->uniformNames.size(); i++)
						{
							Logger::WriteLine("Adding uniform: " + included->uniformTypes[i] + " " + varName + "." + included->uniformNames[i]);
							ss->AddUniform(included->uniformTypes[i], varName + "." + included->uniformNames[i]);
						}
					}

					Logger::WriteLine("Adding uniform: " + varType + " " + varName);
					ss->AddUniform(varType, varName);
				}
				else
				{
					if (line.find("uniform") == 0)
					{
						std::vector<std::string> parts = Utils::Split(line, ' ');

						std::string uniformType = parts[1];
						std::string uniformName = parts[2].substr(0, parts[2].length() - 1);

						Logger::WriteLine("new uniform: " + uniformType + " " + uniformName);

						_uniformNames.push_back(uniformName);
						_uniformTypes.push_back(uniformType);
					}
				}
			}

			file.close();
		}

		return text;
	}

	void Shader::Finish()
	{
		Logger::WriteLine("Linking shader " + std::to_string(_id));

		glLinkProgram(_program);

		GLint success;
		glGetProgramiv(_program, GL_LINK_STATUS, &success);
		if (success != GL_TRUE)
		{
			Logger::WriteLine("ERROR: shader " + std::to_string(_id) + " did not link properly");
			exit(-1);
		}

		while (_shaders.size() > 0)
		{
			glDetachShader(_program, _shaders[0]);
			glDeleteShader(_shaders[0]);

			_shaders.erase(_shaders.begin());
		}

		//add all uniforms to map
		for (auto it = _uniformNames.begin(); it != _uniformNames.end(); it++)
		{
			AddUniform(*it);
			Logger::WriteLine(*it + " location: " + std::to_string(GetUniformLocation(*it)));
		}
	}

	void Shader::AddUniform(std::string name)
	{
		if (_uniforms.find(name) == _uniforms.end())
			_uniforms.insert(std::pair<std::string, int>(name, glGetUniformLocation(_program, name.c_str())));
	}

	void Shader::Bind()
	{
		glUseProgram(_program);
	}

	int Shader::GetUniformLocation(std::string uniform)
	{
		if (_uniforms.find(uniform) != _uniforms.end())
			return _uniforms[uniform];

		return -1;
	}

	void Shader::SetUniform1i(std::string uniform, int i)
	{
		glUniform1i(GetUniformLocation(uniform), i);
	}

	void Shader::SetUniform2i(std::string uniform, int i1, int i2)
	{
		glUniform2i(GetUniformLocation(uniform), i1, i2);
	}

	void Shader::SetUniform3i(std::string uniform, int i1, int i2, int i3)
	{
		glUniform3i(GetUniformLocation(uniform), i1, i2, i3);
	}

	void Shader::SetUniform4i(std::string uniform, int i1, int i2, int i3, int i4)
	{
		glUniform4i(GetUniformLocation(uniform), i1, i2, i3, i4);
	}

	void Shader::SetUniform1f(std::string uniform, float x)
	{
		glUniform1f(GetUniformLocation(uniform), x);
	}

	void Shader::SetUniform2f(std::string uniform, float x, float y)
	{
		glUniform2f(GetUniformLocation(uniform), x, y);
	}

	void Shader::SetUniform3f(std::string uniform, float x, float y, float z)
	{
		glUniform3f(GetUniformLocation(uniform), x, y, z);
	}

	void Shader::SetUniform4f(std::string uniform, float x, float y, float z, float w)
	{
		glUniform4f(GetUniformLocation(uniform), x, y, z, w);
	}

	void Shader::SetUniform1d(std::string uniform, double x)
	{
		glUniform1d(GetUniformLocation(uniform), x);
	}

	void Shader::SetUniform2d(std::string uniform, double x, double y)
	{
		glUniform2d(GetUniformLocation(uniform), x, y);
	}

	void Shader::SetUniform3d(std::string uniform, double x, double y, double z)
	{
		glUniform3d(GetUniformLocation(uniform), x, y, z);
	}

	void Shader::SetUniform4d(std::string uniform, double x, double y, double z, double w)
	{
		glUniform4d(GetUniformLocation(uniform), x, y, z, w);
	}

	void Shader::SetUniformMatrix4fv(std::string uniform, glm::mat4 matrix)
	{
		glUniformMatrix4fv(GetUniformLocation(uniform), 1, false, &matrix[0][0]);
	}

	void Shader::SetUniform2i(std::string uniform, glm::vec2 v)
	{
		SetUniform2i(uniform, v.x, v.y);
	}

	void Shader::SetUniform3i(std::string uniform, glm::vec3 v)
	{
		SetUniform3i(uniform, v.x, v.y, v.z);
	}

	void Shader::SetUniform4i(std::string uniform, glm::vec4 v)
	{
		SetUniform4i(uniform, v.x, v.y, v.z, v.w);
	}

	void Shader::SetUniform2f(std::string uniform, glm::vec2 v)
	{
		SetUniform2f(uniform, v.x, v.y);
	}

	void Shader::SetUniform3f(std::string uniform, glm::vec3 v)
	{
		SetUniform3f(uniform, v.x, v.y, v.z);
	}

	void Shader::SetUniform4f(std::string uniform, glm::vec4 v)
	{
		SetUniform4f(uniform, v.x, v.y, v.z, v.w);
	}

	void Shader::SetUniform2d(std::string uniform, glm::vec2 v)
	{
		SetUniform2d(uniform, v.x, v.y);
	}

	void Shader::SetUniform3d(std::string uniform, glm::vec3 v)
	{
		SetUniform3d(uniform, v.x, v.y, v.z);
	}

	void Shader::SetUniform4d(std::string uniform, glm::vec4 v)
	{
		SetUniform4d(uniform, v.x, v.y, v.z, v.w);
	}
}
