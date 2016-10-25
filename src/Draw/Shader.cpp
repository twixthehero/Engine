#include "Draw\Shader.h"
#include <fstream>
#include <iostream>
#include "Utils.h"
#include "Draw\RenderingEngine.h"
#include <glm\glm.hpp>
#include "Component\Transform.h"
#include "Component\Camera.h"
#include <glm\gtc\matrix_transform.hpp>


Shader::Shader(int id, std::string name)
{
	_id = id;
	_name = name;

	_uniforms = std::map<std::string, int>();
	_uniformNames = std::vector<std::string>();
	_uniformTypes = std::vector<std::string>();

	Load(_name);
}


Shader::~Shader()
{
}

void Shader::Bind()
{
	glUseProgram(_program);
}

void Shader::UpdateUniforms(Transform* transform, Material* material, RenderingEngine* renderingEngine)
{
	glm::mat4 modelMatrix = transform->GetModelMatrix();
	Camera* mainCamera = renderingEngine->GetCamera();
	glm::mat4 mvpMatrix = mainCamera->GetViewProjectionMatrix() * modelMatrix;

	glm::mat4 proj = glm::perspective(60.0f, 800.0f / 600, 0.01f, 500.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 model = glm::translate(glm::mat4(), glm::vec3(0, 0, 0));
	glm::mat4 mvp = proj * view * model;

	for (int i = 0; i < _uniformNames.size(); i++)
	{
		std::string uniformName = _uniformNames[i];
		std::string uniformType = _uniformTypes[i];

		//std::cout << uniformName << " location: " << GetUniformLocation(uniformName) << std::endl;

		if (uniformName == "mvp")
		{
			glUniformMatrix4fv(GetUniformLocation(uniformName), 1, false, &mvpMatrix[0][0]);
			//glUniformMatrix4fv(GetUniformLocation(uniformName), 1, false, &mvp[0][0]);
		}
		else if (uniformName == "model")
		{
			glUniformMatrix4fv(GetUniformLocation(uniformName), 1, false, &model[0][0]);
		}
	}
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

std::string Shader::ReadText(std::string filename, std::vector<std::string>* uniforms)
{
	std::ifstream file("shaders\\" + filename);
	std::string line;
	std::string text;

	if (file.is_open())
	{
		while (getline(file, line))
		{
			if (line.find("uniform") == 0)
			{
				std::vector<std::string> parts = Utils::Split(line, ' ');

				std::string uniformType = parts[1];
				std::string uniformName = parts[2].substr(0, parts[2].length() - 1);

				std::cout << "new uniform: " << uniformType << " " << uniformName << std::endl;

				uniforms->push_back(uniformName);
				_uniformNames.push_back(uniformName);
				_uniformTypes.push_back(uniformType);
			}

			text += line + "\n";
		}

		file.close();
	}

	return text;
}

void Shader::Load(std::string name)
{
	std::vector<std::string>* uniforms = new std::vector<std::string>();
	std::string v = ReadText(name + ".vs", uniforms);
	std::string f = ReadText(name + ".fs", uniforms);
	const char* vert = v.c_str();
	const char* frag = f.c_str();

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vert, NULL);
	glCompileShader(vs);

	int success = -1;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
	if (success != GL_TRUE)
	{
		std::cout << "ERROR: GL vertex shader index " << vs << " did not compile" << std::endl;

		int logLength;

		glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &logLength);
		
		if (logLength > 0)
		{
			char* errorMessage = new char[logLength + 1];
			glGetShaderInfoLog(vs, logLength, NULL, errorMessage);
			std::cout << errorMessage << std::endl;
			delete errorMessage;
		}

		exit(-1);
	}

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &frag, NULL);
	glCompileShader(fs);

	success = -1;
	glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
	if (success != GL_TRUE)
	{
		std::cout << "ERROR: GL fragment shader index " << fs << " did not compile" << std::endl;

		int logLength;

		glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &logLength);

		if (logLength > 0)
		{
			char* errorMessage = new char[logLength + 1];
			glGetShaderInfoLog(vs, logLength, NULL, errorMessage);
			std::cout << errorMessage << std::endl;
			delete errorMessage;
		}

		exit(-1);
	}

	//std::cout << "Reserving pointer for shader" << std::endl;
	_program = glCreateProgram();

	if (_program == 0)
	{
		std::cout << "ERROR: Unable to create program space for shader" << std::endl;
		exit(-1);
	}

	//std::cout << "Linking vertex and fragment" << std::endl;
	glAttachShader(_program, vs);
	glAttachShader(_program, fs);
	glLinkProgram(_program);

	glGetProgramiv(_program, GL_LINK_STATUS, &success);
	if (success != GL_TRUE)
	{
		std::cout << "ERROR: shader '" << name << "' did not link properly" << std::endl;
		exit(-1);
	}

	glDetachShader(_program, vs);
	glDetachShader(_program, fs);

	glDeleteShader(vs);
	glDeleteShader(fs);

	//add all uniforms to map
	while (uniforms->size() > 0)
	{
		AddUniform(uniforms->at(uniforms->size() - 1));
		uniforms->pop_back();
	}

	for (int i = 0; i < _uniformNames.size(); i++)
		std::cout << _uniformNames[i] << " location: " << GetUniformLocation(_uniformNames[i]) << std::endl;

	delete uniforms;
}

void Shader::AddUniform(std::string name)
{
	if (_uniforms.find(name) == _uniforms.end())
		_uniforms.insert(std::pair<std::string, int>(name, glGetUniformLocation(_program, name.c_str())));
}