#pragma once
#include <string>
#include <vector>

struct ShaderStruct
{
	ShaderStruct(std::string name);
	~ShaderStruct();

	void AddUniform(std::string type, std::string name);

	std::vector<std::string> uniformTypes;
	std::vector<std::string> uniformNames;
	std::string _name;
};

