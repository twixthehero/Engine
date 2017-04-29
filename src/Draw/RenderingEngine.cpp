#include "Draw\RenderingEngine.h"
#include <GL\gl3w.h>
#include <glm\gtc\matrix_transform.hpp>
#include "ShaderManager.h"
#include "Component\Light.h"
#include "Component\PointLight.h"
#include "Component\DirectionalLight.h"
#include "Core\GameObject.h"
#include "Core\MaterialSkybox.h"
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
		_sphere = new MeshRenderer(MeshManager::GetInstance()->GetMesh("sphere"), nullptr);

		_ambientLight = new Light();
		_ambientLight->color.r = 0.2f;
		_ambientLight->color.g = 0.2f;
		_ambientLight->color.b = 0.2f;
		_ambientLight->intensity = 0.2f;
	}

	RenderingEngine::~RenderingEngine()
	{
	}

	void RenderingEngine::Init()
	{
		_instance = new RenderingEngine();

		//glClearColor(0.2f, 0.4f, 0.85f, 1.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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

	bool RenderingEngine::GetShowLightingDebug()
	{
		return _showLightingDebug;
	}

	void RenderingEngine::SetShowLightingDebug(bool show)
	{
		if (_showLightingDebug == show)
			return;

		Logger::WriteLine("_showLightingDebug: " + std::to_string(_showLightingDebug) + " => " + std::to_string(show));
		_showLightingDebug = show;
	}

	RenderingEngine::ERenderingMode RenderingEngine::GetRenderingMode()
	{
		return _renderingMode;
	}

	void RenderingEngine::SetRenderingMode(ERenderingMode mode)
	{
		if (_renderingMode == mode)
			return;

		Logger::WriteLine("_renderingMode: " + std::to_string(_renderingMode) + " => " + std::to_string(mode));
		_renderingMode = mode;
	}

	void RenderingEngine::Render(GameObject* gameObject)
	{
		if (_camera != Camera::main)
			SetCamera(Camera::main);

		switch (_renderingMode)
		{
			case ERenderingMode::FORWARD:
				Forward(gameObject);
				break;
			case ERenderingMode::DEFERRED:
				Deferred(gameObject);
				break;
			default:
				Logger::WriteLine("Error! Unknown rendering mode: " + std::to_string(_renderingMode));
				break;
		}
	}

	void RenderingEngine::Forward(GameObject* gameObject)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST); glDepthFunc(GL_LEQUAL);
		glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_CULL_FACE); glCullFace(GL_BACK);
		glDepthMask(GL_TRUE);

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
		shader->SetUniform3f("ambientColor", _ambientLight->color);

		for (MeshRenderer* renderer : _meshRenderers)
		{
			if (!renderer->IsEnabled()) continue;

			shader->SetUniformMatrix4fv("mvp", viewProjection * renderer->gameObject->transform->GetTransformation());
			renderer->Render();
		}

		//point
		
		shader = ShaderManager::GetInstance()->UseShader("forward-point");
		for (PointLight* point : _pointLights)
		{
			if (!point->IsEnabled()) continue;

			shader->SetUniform3f("lightPosition", point->gameObject->transform->GetTransformedPosition());
			shader->SetUniform3f("lightColor", point->color);
			shader->SetUniform1f("lightIntensity", point->intensity);
			shader->SetUniform1f("range", point->range);

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
			shader->SetUniform3f("lightColor", dirLight->color);
			shader->SetUniform1f("lightIntensity", dirLight->intensity);

			for (MeshRenderer* renderer : _meshRenderers)
			{
				modelMatrix = renderer->gameObject->transform->GetTransformation();
				shader->SetUniformMatrix4fv("mvp", viewProjection * modelMatrix);
				renderer->Render();
			}
		}

		if (_showLightingDebug)
			ShowLightingDebug();

		RenderSkybox();
	}

	void RenderingEngine::Deferred(GameObject* gameObject)
	{
		_gbuffer->StartFrame();

		GeometryPass(gameObject);

		glEnable(GL_STENCIL_TEST);

		_renderingComponents.clear();
		_pointLights.clear();
		gameObject->GetComponentsInChildren(EComponentType::LIGHT_POINT, _renderingComponents);
		for (auto it = _renderingComponents.begin(); it != _renderingComponents.end(); it++)
			_pointLights.push_back(dynamic_cast<PointLight*>(*it));

		for (PointLight* light : _pointLights)
		{
			if (!light->IsEnabled()) continue;

			StencilPass(light);
			PointLightPass(light);
		}

		glDisable(GL_STENCIL_TEST);

		DirectionalLightPass(gameObject);

		FinalPass();

		RenderSkybox();
	}

	void RenderingEngine::GeometryPass(GameObject* gameObject)
	{
		Shader* shader = ShaderManager::GetInstance()->UseShader("geometry");

		_gbuffer->BindForGeometryPass();

		glDepthMask(GL_TRUE);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);

		_renderingComponents.clear();
		_meshRenderers.clear();
		gameObject->GetComponentsInChildren(EComponentType::MESH_RENDERER, _renderingComponents);

		for (auto it = _renderingComponents.begin(); it != _renderingComponents.end(); it++)
			_meshRenderers.push_back(dynamic_cast<MeshRenderer*>(*it));

		glm::mat4 mvp;
		glm::mat4 viewProjection = _camera->GetViewProjectionMatrix();

		for (MeshRenderer* renderer : _meshRenderers)
		{
			if (!renderer->IsEnabled()) continue;

			shader->SetUniformMatrix4fv("mvp", viewProjection * renderer->gameObject->transform->GetTransformation());
			renderer->Render();
		}

		glDepthMask(GL_FALSE);
	}

	void RenderingEngine::StencilPass(PointLight* light)
	{
		Shader* nullShader = ShaderManager::GetInstance()->UseShader("null");

		_gbuffer->BindForStencilPass();

		glEnable(GL_DEPTH_TEST);

		glDisable(GL_CULL_FACE);

		glClear(GL_STENCIL_BUFFER_BIT);

		glStencilFunc(GL_ALWAYS, 0, 0);

		glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
		glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

		glm::mat4 viewProjection = _camera->GetViewProjectionMatrix();

		nullShader->SetUniformMatrix4fv("mvp", viewProjection *
			(glm::translate(glm::mat4(), light->gameObject->transform->position) *
				glm::scale(glm::mat4(), glm::vec3(light->range))
			)
		);

		_sphere->Render();
	}

	void RenderingEngine::PointLightPass(PointLight* light)
	{
		_gbuffer->BindForLightPass();

		glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		Shader* pointShader = ShaderManager::GetInstance()->UseShader("pointLight");
		pointShader->SetUniform2f("screenSize", _windowWidth, _windowHeight);
		pointShader->SetUniform1f("ambientIntensity", _ambientLight->intensity);
		pointShader->SetUniform3f("ambientColor", _ambientLight->color);
		pointShader->SetUniform3f("eyeWorldPos", _camera->gameObject->transform->GetTransformedPosition());

		pointShader->SetUniform1i("positionMap", GBuffer::GBufferTextureType::Position);
		pointShader->SetUniform1i("colorMap", GBuffer::GBufferTextureType::Diffuse);
		pointShader->SetUniform1i("normalMap", GBuffer::GBufferTextureType::Normal);

		pointShader->SetUniform3f("pointLight.light.color", light->color);
		pointShader->SetUniform1f("pointLight.light.intensity", light->intensity);
		pointShader->SetUniform3f("pointLight.position", light->gameObject->transform->position);
		pointShader->SetUniform1f("pointLight.range", light->range);

		glm::mat4 viewProjection = _camera->GetViewProjectionMatrix();

		pointShader->SetUniformMatrix4fv("mvp", viewProjection *
			(glm::translate(glm::mat4(), light->gameObject->transform->position) *
				glm::scale(glm::mat4(), glm::vec3(light->range))
			)
		);

		_sphere->Render();

		glCullFace(GL_BACK);

		glDisable(GL_BLEND);
	}

	void RenderingEngine::DirectionalLightPass(GameObject* gameObject)
	{
		_gbuffer->BindForLightPass();

		glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		_renderingComponents.clear();
		_directionalLights.clear();
		gameObject->GetComponentsInChildren(EComponentType::LIGHT_DIRECTIONAL, _renderingComponents);
		for (auto it = _renderingComponents.begin(); it != _renderingComponents.end(); it++)
			_directionalLights.push_back(dynamic_cast<DirectionalLight*>(*it));

		Shader* directional = ShaderManager::GetInstance()->UseShader("directionalLight");
		directional->SetUniform2f("screenSize", _windowWidth, _windowHeight);
		directional->SetUniform1f("ambientIntensity", _ambientLight->intensity);
		directional->SetUniform3f("ambientColor", _ambientLight->color);
		directional->SetUniform3f("eyeWorldPos", _camera->gameObject->transform->GetTransformedPosition());

		directional->SetUniform1i("positionMap", GBuffer::GBufferTextureType::Position);
		directional->SetUniform1i("colorMap", GBuffer::GBufferTextureType::Diffuse);
		directional->SetUniform1i("normalMap", GBuffer::GBufferTextureType::Normal);

		for (DirectionalLight* light : _directionalLights)
		{
			if (!light->IsEnabled()) continue;

			directional->SetUniform3f("directionalLight.light.color", light->color);
			directional->SetUniform1f("directionalLight.light.intensity", light->intensity);
			directional->SetUniform3f("directionalLight.direction", light->gameObject->transform->GetForward());

			directional->SetUniformMatrix4fv("mvp", glm::mat4());

			_quad->Render();
		}

		glDisable(GL_BLEND);
	}

	void RenderingEngine::FinalPass()
	{
		_gbuffer->BindForFinalPass();

		//also copy depth info for rendering the point light spheres
		glBlitFramebuffer(0, 0, _windowWidth, _windowHeight, 0, 0, _windowWidth, _windowHeight, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);

		if (_showLightingDebug)
			ShowLightingDebug();
	}

	void RenderingEngine::ShowLightingDebug()
	{
		Shader* lightingDebug = ShaderManager::GetInstance()->UseShader("lightingDebug");
		glm::mat4 viewProjection = _camera->GetViewProjectionMatrix();

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glEnable(GL_DEPTH_TEST);

		for (PointLight* light : _pointLights)
		{
			if (!light->IsEnabled()) continue;

			lightingDebug->SetUniform3f("color", glm::vec3(0.0f, 1.0f, 0.0f));

			lightingDebug->SetUniformMatrix4fv("mvp", viewProjection *
				(glm::translate(glm::mat4(), light->gameObject->transform->position) *
					glm::scale(glm::mat4(), glm::vec3(light->range))
				)
			);

			_sphere->Render();
		}

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_DEPTH_TEST);
	}

	void RenderingEngine::RenderSkybox()
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		Shader* skyboxShader = ShaderManager::GetInstance()->UseShader("skybox");

		skyboxShader->SetUniformMatrix4fv("projection", _camera->GetProjectionMatrix());
		skyboxShader->SetUniformMatrix4fv("view", glm::mat4(glm::mat3(_camera->GetViewMatrix())));

		_skyboxMesh->Render();

		glDisable(GL_DEPTH_TEST);
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

	void RenderingEngine::SetSkybox(Material* skybox)
	{
		if (_skyboxMesh != nullptr)
			delete _skyboxMesh;

		_skybox = skybox;
		_skyboxMesh = new MeshRenderer(MeshManager::GetInstance()->GetMesh("cubeInvert"), nullptr);
	}
}
