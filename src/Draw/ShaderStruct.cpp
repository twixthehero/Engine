#include "Draw/ShaderStruct.h"

ShaderStruct::ShaderStruct(std::string name)
{
	_name = name;
}

ShaderStruct::~ShaderStruct()
{
}

void ShaderStruct::AddUniform(std::string type, std::string name)
{
	uniformTypes.push_back(type);
	uniformNames.push_back(name);
}
