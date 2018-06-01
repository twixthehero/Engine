#include "ShaderManager.h"
#include "Logger.h"
#include "Draw/Shader.h"
#include <GL/gl3w.h>

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
		Shader* skybox = new Shader(_nextShaderIndex++);
		skybox->AttachShader(GL_VERTEX_SHADER, "skybox.vs");
		skybox->AttachShader(GL_FRAGMENT_SHADER, "skybox.fs");
		skybox->Finish();
		AddShader("skybox", skybox);

		Shader* forwardAmbient = new Shader(_nextShaderIndex++);
		forwardAmbient->AttachShader(GL_VERTEX_SHADER, "forward-ambient.vs");
		forwardAmbient->AttachShader(GL_FRAGMENT_SHADER, "forward-ambient.fs");
		forwardAmbient->Finish();
		AddShader("forward-ambient", forwardAmbient);

		Shader* forwardDirectional = new Shader(_nextShaderIndex++);
		forwardDirectional->AttachShader(GL_VERTEX_SHADER, "forward-directional.vs");
		forwardDirectional->AttachShader(GL_FRAGMENT_SHADER, "forward-directional.fs");
		forwardDirectional->Finish();
		AddShader("forward-directional", forwardDirectional);

		Shader* forwardPoint = new Shader(_nextShaderIndex++);
		forwardPoint->AttachShader(GL_VERTEX_SHADER, "forward-point.vs");
		forwardPoint->AttachShader(GL_FRAGMENT_SHADER, "forward-point.fs");
		forwardPoint->Finish();
		AddShader("forward-point", forwardPoint);

		Shader* geometry = new Shader(_nextShaderIndex++);
		geometry->AttachShader(GL_VERTEX_SHADER, "geometry.vs");
		geometry->AttachShader(GL_FRAGMENT_SHADER, "geometry.fs");
		geometry->Finish();
		AddShader("geometry", geometry);

		Shader* lightingDebug = new Shader(_nextShaderIndex++);
		lightingDebug->AttachShader(GL_VERTEX_SHADER, "lightingDebug.vs");
		lightingDebug->AttachShader(GL_FRAGMENT_SHADER, "lightingDebug.fs");
		lightingDebug->Finish();
		AddShader("lightingDebug", lightingDebug);

		Shader* ambientLight = new Shader(_nextShaderIndex++);
		ambientLight->AttachShader(GL_VERTEX_SHADER, "light.vs");
		ambientLight->AttachShader(GL_FRAGMENT_SHADER, "ambientLight.fs");
		ambientLight->Finish();
		AddShader("ambientLight", ambientLight);

		Shader* pointLight = new Shader(_nextShaderIndex++);
		pointLight->AttachShader(GL_VERTEX_SHADER, "light.vs");
		pointLight->AttachShader(GL_FRAGMENT_SHADER, "lightPoint.fs");
		pointLight->Finish();
		AddShader("pointLight", pointLight);

		Shader* directionalLight = new Shader(_nextShaderIndex++);
		directionalLight->AttachShader(GL_VERTEX_SHADER, "light.vs");
		directionalLight->AttachShader(GL_FRAGMENT_SHADER, "lightDirectional.fs");
		directionalLight->Finish();
		AddShader("directionalLight", directionalLight);

		Shader* spotLight = new Shader(_nextShaderIndex++);
		spotLight->AttachShader(GL_VERTEX_SHADER, "light.vs");
		spotLight->AttachShader(GL_FRAGMENT_SHADER, "lightSpot.fs");
		spotLight->Finish();
		AddShader("spotLight", spotLight);

		Shader* null = new Shader(_nextShaderIndex++);
		null->AttachShader(GL_VERTEX_SHADER, "null.vs");
		null->AttachShader(GL_FRAGMENT_SHADER, "null.fs");
		null->Finish();
		AddShader("null", null);
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
