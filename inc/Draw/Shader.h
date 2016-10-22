#pragma once
#include <string>
#include <map>
#include <glew\glew.h>
#include <vector>

class Shader
{
public:
	Shader(int id, std::string name);
	~Shader();

	virtual void Bind();

	int GetUniformLocation(std::string uniform);

	void SetUniform4fv(std::string uniform, float x, float y, float z, float w);
protected:
	std::string ReadText(std::string filename, std::vector<std::string>* uniforms);
	virtual void Load(std::string name);
	void AddUniform(std::string name);

	GLuint _program;
	std::map<std::string, int> _uniforms;
private:
	int _id;
	std::string _name;
};

