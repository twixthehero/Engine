#include "ShaderManager.h"
#include "Logger.h"
#include "Draw\Shader.h"
#include <GL\gl3w.h>

namespace VoxEngine
{
	ShaderManager* ShaderManager::_instance = nullptr;

	void ShaderManager::Init()
	{
		if (_instance == nullptr)
			_instance = new ShaderManager();
	}

	ShaderManager* ShaderManager::GetInstance()
	{
		return _instance;
	}

	void ShaderManager::Shutdown()
	{

	}

	ShaderManager::ShaderManager()
	{
		//LoadShader("default");
		LoadShader("geometry");
	}

	ShaderManager::~ShaderManager()
	{
	}

	void ShaderManager::LoadShader(std::string name)
	{
		if (_shaders.find(name) != _shaders.end())
		{
			Logger::WriteLine("Shader '" + name + "' is already loaded!");
			return;
		}

		_shaders.insert(std::pair<std::string, Shader*>(name, new Shader(_nextShaderIndex++, name)));
	}

	void ShaderManager::UnloadShader(std::string name)
	{

	}

	Shader* ShaderManager::GetShader(std::string name)
	{
		if (_shaders.find(name) == _shaders.end())
		{
			Logger::WriteLine("Tried to get non-loaded shader '" + name + "'");

			return nullptr;
		}

		return _shaders[name];
	}

	void ShaderManager::UseShader(std::string name)
	{
		if (_shaders.find(name) == _shaders.end())
		{
			Logger::WriteLine("Tried to use non-loaded shader '" + name + "'");
			glUseProgram(0);
			
			return;
		}

		_shaders[name]->Bind();
	}
}
