#include "Draw\RenderingEngine.h"
#include <GL\gl3w.h>
#include "ShaderManager.h"
#include "Component\Light.h"
#include "Component\PointLight.h"
#include "Component\DirectionalLight.h"
#include "Core\GameObject.h"
#include "Component\Transform.h"
#include "Component\Camera.h"
#include "Component\MeshRenderer.h"
#include "MeshManager.h"
#include "Draw\GBuffer.h"
#include "Draw\Shader.h"
#include "Window\WindowManager.h"
#include "Window\Window.h"
#include "Logger.h"
#include "Utils.h"
#include <iostream>

namespace VoxEngine
{
	RenderingEngine* RenderingEngine::_instance = nullptr;

	RenderingEngine::RenderingEngine()
	{
		_gbuffer = new GBuffer();

		_window = WindowManager::GetInstance()->GetMainWindow();
		_windowWidth = _window->GetWidth();
		_windowHeight = _window->GetHeight();

		if (!_gbuffer->Init(_windowWidth, _windowHeight))
		{
			Logger::WriteLine("Failed to initialize the GBuffer!");
		}

		_quad = new MeshRenderer(MeshManager::GetInstance()->GetMesh("quad"), nullptr);

		_ambientLight = new Light();
		_ambientLight->color.r = 0.2f;
		_ambientLight->color.g = 0.2f;
		_ambientLight->color.b = 0.2f;
		_ambientLight->color.a = 0.2f;
	}

	RenderingEngine::~RenderingEngine()
	{
	}

	void RenderingEngine::Init()
	{
		_instance = new RenderingEngine();

		//glClearColor(0.2f, 0.4f, 0.85f, 1.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		glEnable(GL_CULL_FACE);	glCullFace(GL_BACK);
	}

	RenderingEngine* RenderingEngine::GetInstance()
	{
		return _instance;
	}

	void RenderingEngine::Shutdown()
	{
		if (_instance != nullptr)
		{
			delete _instance;
			_instance = nullptr;
		}
	}

	void RenderingEngine::SetRenderingMode(ERenderingMode mode)
	{
		_renderingMode = mode;
	}

	void RenderingEngine::Render(GameObject* gameObject)
	{
		if (_camera != Camera::main)
			SetCamera(Camera::main);

		if (_renderingMode == ERenderingMode::FORWARD)
		{
			Forward(gameObject);
		}
		else if (_renderingMode == ERenderingMode::DEFERRED)
		{
			Deferred(gameObject);
		}
	}

	void RenderingEngine::Forward(GameObject* gameObject)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST); glDepthFunc(GL_LEQUAL);
		glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//fetch components - meshrenderers, lights

		_renderingComponents.clear();
		_meshRenderers.clear();
		gameObject->GetComponentsInChildren(EComponentType::MESH_RENDERER, _renderingComponents);
		for (auto it = _renderingComponents.begin(); it != _renderingComponents.end(); it++)
			_meshRenderers.push_back(dynamic_cast<MeshRenderer*>(*it));

		_renderingComponents.clear();
		_pointLights.clear();
		gameObject->GetComponentsInChildren(EComponentType::LIGHT_POINT, _renderingComponents);
		for (auto it = _renderingComponents.begin(); it != _renderingComponents.end(); it++)
			_pointLights.push_back(dynamic_cast<PointLight*>(*it));

		_renderingComponents.clear();
		_directionalLights.clear();
		gameObject->GetComponentsInChildren(EComponentType::LIGHT_DIRECTIONAL, _renderingComponents);
		for (auto it = _renderingComponents.begin(); it != _renderingComponents.end(); it++)
			_directionalLights.push_back(dynamic_cast<DirectionalLight*>(*it));

		glm::mat4 viewProjection = _camera->GetViewProjectionMatrix();
		glm::mat4 modelMatrix;

		//ambient

		Shader* shader = ShaderManager::GetInstance()->UseShader("forward-ambient");
		shader->SetUniform1f("ambientIntensity", _ambientLight->intensity);
		shader->SetUniform4f("ambientColor", _ambientLight->color);
		
		for (MeshRenderer* renderer : _meshRenderers)
		{
			modelMatrix = renderer->gameObject->transform->GetTransformation();
			shader->SetUniformMatrix4fv("mvp", viewProjection * modelMatrix);
			renderer->Render();
		}

		//point
		
		shader = ShaderManager::GetInstance()->UseShader("forward-point");
		for (PointLight* point : _pointLights)
		{
			if (!point->IsEnabled()) continue;

			shader->SetUniform3f("lightPosition", point->gameObject->transform->GetTransformedPosition());
			shader->SetUniform4f("lightColor", point->color);
			shader->SetUniform1f("lightIntensity", point->intensity);
			shader->SetUniform1f("constant", point->constant);
			shader->SetUniform1f("linear", point->linear);
			shader->SetUniform1f("exponent", point->exponent);

			for (MeshRenderer* renderer : _meshRenderers)
			{
				modelMatrix = renderer->gameObject->transform->GetTransformation();
				shader->SetUniformMatrix4fv("mvp", viewProjection * modelMatrix);
				renderer->Render();
			}
		}
		
		//directional
		
		shader = ShaderManager::GetInstance()->UseShader("forward-directional");
		for (DirectionalLight* dirLight : _directionalLights)
		{
			if (!dirLight->IsEnabled()) continue;

			shader->SetUniform3f("lightDirection", dirLight->gameObject->transform->GetForward());
			shader->SetUniform4f("lightColor", dirLight->color);
			shader->SetUniform1f("lightIntensity", dirLight->intensity);

			for (MeshRenderer* renderer : _meshRenderers)
			{
				modelMatrix = renderer->gameObject->transform->GetTransformation();
				shader->SetUniformMatrix4fv("mvp", viewProjection * modelMatrix);
				renderer->Render();
			}
		}
	}

	void RenderingEngine::Deferred(GameObject* gameObject)
	{
		GeometryPass(gameObject);
		BeginLightingPasses();
		AmbientLightPass(gameObject);
		//PointLightPass(gameObject);
		//DirectionalLightPass(gameObject);
	}

	void RenderingEngine::GeometryPass(GameObject* gameObject)
	{
		Shader* shader = ShaderManager::GetInstance()->UseShader("geometry");

		_gbuffer->BindForWriting();

		glDepthMask(GL_TRUE);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		_renderingComponents.clear();
		_meshRenderers.clear();
		gameObject->GetComponentsInChildren(EComponentType::MESH_RENDERER, _renderingComponents);

		for (auto it = _renderingComponents.begin(); it != _renderingComponents.end(); it++)
			_meshRenderers.push_back(dynamic_cast<MeshRenderer*>(*it));

		glm::mat4 mvp;
		glm::mat4 viewProjection = _camera->GetViewProjectionMatrix();

		for (MeshRenderer* renderer : _meshRenderers)
		{
			shader->SetUniformMatrix4fv("mvp", viewProjection * renderer->gameObject->transform->GetTransformation());
			renderer->Render();
		}

		glDepthMask(GL_FALSE);
		
		glDisable(GL_DEPTH_TEST);
	}

	void RenderingEngine::BeginLightingPasses()
	{
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		_gbuffer->BindForReading();
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void RenderingEngine::AmbientLightPass(GameObject* gameObject)
	{
		Shader* ambientShader = ShaderManager::GetInstance()->UseShader("ambientLight");
		ambientShader->SetUniform1f("intensity", _ambientLight->intensity);
		ambientShader->SetUniform4f("color", _ambientLight->color);
		ambientShader->SetUniform2f("screenSize", _windowWidth, _windowHeight);
		ambientShader->SetUniformMatrix4fv("mvp", glm::mat4());
		ambientShader->SetUniform1i("colors", GBuffer::GBufferTextureType::Diffuse);

		_quad->Render();
	}

	void RenderingEngine::PointLightPass(GameObject* gameObject)
	{
		//ShaderManager::GetInstance()->UseShader("pointLight");


	}

	void RenderingEngine::DirectionalLightPass(GameObject* gameObject)
	{
		//ShaderManager::GetInstance()->UseShader("directionalLight");


	}

	void RenderingEngine::SetCamera(Camera* camera)
	{
		_camera = camera;
	}

	Camera* RenderingEngine::GetCamera()
	{
		return _camera;
	}

	Light* RenderingEngine::GetAmbientLight()
	{
		return _ambientLight;
	}
}
