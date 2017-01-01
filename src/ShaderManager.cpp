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
		Shader* geometry = new Shader(_nextShaderIndex++);
		geometry->AttachShader(GL_VERTEX_SHADER, "geometry.vs");
		geometry->AttachShader(GL_FRAGMENT_SHADER, "geometry.fs");
		geometry->Finish();
		AddShader("geometry", geometry);

		Shader* ambientLight = new Shader(_nextShaderIndex++);
		ambientLight->AttachShader(GL_VERTEX_SHADER, "light.vs");
		ambientLight->AttachShader(GL_FRAGMENT_SHADER, "ambientLight.fs");
		ambientLight->Finish();
		AddShader("ambientLight", ambientLight);

		Shader* pointLight = new Shader(_nextShaderIndex++);
		pointLight->AttachShader(GL_VERTEX_SHADER, "light.vs");
		pointLight->AttachShader(GL_FRAGMENT_SHADER, "pointLight.fs");
		pointLight->Finish();
		AddShader("pointLight", pointLight);

		Shader* directionalLight = new Shader(_nextShaderIndex++);
		directionalLight->AttachShader(GL_VERTEX_SHADER, "light.vs");
		directionalLight->AttachShader(GL_FRAGMENT_SHADER, "directionalLight.fs");
		directionalLight->Finish();
		AddShader("directionalLight", directionalLight);
	}

	ShaderManager::~ShaderManager()
	{
	}

	void ShaderManager::AddShader(std::string name, Shader* shader)
	{
		if (_shaders.find(name) != _shaders.end())
		{
			Logger::WriteLine("Shader '" + name + "' is already added!");
			return;
		}

		_shaders.insert(std::pair<std::string, Shader*>(name, shader));
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

	Shader* ShaderManager::GetCurrentShader()
	{
		return _currentShader;
	}

	Shader* ShaderManager::UseShader(std::string name)
	{
		if (_shaders.find(name) == _shaders.end())
		{
			Logger::WriteLine("Tried to use non-loaded shader '" + name + "'");
			glUseProgram(0);
			
			return nullptr;
		}

		_currentShader = _shaders[name];
		_currentShader->Bind();

		return _currentShader;
	}
}
