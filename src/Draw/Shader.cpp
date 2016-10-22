#include "Draw\Shader.h"
#include <fstream>
#include <iostream>
#include "Utils.h"


Shader::Shader(int id, std::string name)
{
	_id = id;
	_name = name;

	_uniforms = std::map<std::string, int>();
}


Shader::~Shader()
{
}

void Shader::Bind()
{
	glUseProgram(_program);
}

int Shader::GetUniformLocation(std::string uniform)
{
	if (_uniforms.find(uniform) != _uniforms.end())
		return _uniforms[uniform];

	return 0;
}

void Shader::SetUniform4fv(std::string uniform, float x, float y, float z, float w)
{
	glUniform4f(GetUniformLocation(uniform), x, y, z, w);
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
			if (line.find("uniform") != std::string::npos)
			{
				std::string part = Utils::Split(line, ' ')[2];
				uniforms->push_back(part.substr(0, part.length() - 1));
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

	//std::cout << "Compiling vertex shader" << std::endl;
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vert, NULL);
	glCompileShader(vs);

	int success = -1;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
	if (success != GL_TRUE)
	{
		std::cout << "ERROR: GL vertex shader index " << vs << " did not compile" << std::endl;
		exit(-1);
	}

	//std::cout << "Compiling fragment shader" << std::endl;
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &frag, NULL);
	glCompileShader(fs);

	success = -1;
	glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
	if (success != GL_TRUE)
	{
		std::cout << "ERROR: GL fragment shader index " << fs << " did not compile" << std::endl;
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

	Bind();

	//add all uniforms to map
	while (uniforms->size() > 0)
	{
		AddUniform(uniforms->at(uniforms->size() - 1));
		uniforms->pop_back();
	}

	delete uniforms;
}

void Shader::AddUniform(std::string name)
{
	if (_uniforms.find(name) != _uniforms.end())
		_uniforms[name] = glGetUniformLocation(_program, name.c_str());
}