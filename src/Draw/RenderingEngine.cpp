#include "Draw/RenderingEngine.h"
#include <GL/gl3w.h>
#include <glm/gtc/matrix_transform.hpp>
#include "ShaderManager.h"
#include "Component/Light.h"
#include "Component/PointLight.h"
#include "Component/DirectionalLight.h"
#include "Component/SpotLight.h"
#include "Core/GameObject.h"
#include "Core/MaterialSkybox.h"
#include "Component/Transform.h"
#include "Component/Camera.h"
#include "Component/MeshRenderer.h"
#include "MeshManager.h"
#include "Draw/GBuffer.h"
#include "Draw/Shader.h"
#include "Draw/Mesh.h"
#include "Window/WindowManager.h"
#include "Window/Window.h"
#include "Logger.h"
#include "Utils.h"
#include <iostream>
#include <algorithm>

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

		_quad = MeshManager::GetInstance()->GetMesh("quad");
		_sphere = MeshManager::GetInstance()->GetMesh("sphere");
		_cone = MeshManager::GetInstance()->GetMesh("coneSpotlight");
		_arrow = MeshManager::GetInstance()->GetMesh("arrow");

		_ambientLight = new Light();
		_ambientLight->color.r = 0.2f;
		_ambientLight->color.g = 0.2f;
		_ambientLight->color.b = 0.2f;
		_ambientLight->intensity = 0.2f;
	}

	RenderingEngine::~RenderingEngine()
	{
		delete _gbuffer;
		delete _ambientLight;
	}

	void RenderingEngine::Init()
	{
		_instance = new RenderingEngine();

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

	void RenderingEngine::AddRenderer(MeshRenderer* renderer)
	{
		//todo - change to dictionary with ids
		_meshRenderers.push_back(renderer);
	}

	void RenderingEngine::RemoveRenderer(MeshRenderer* renderer)
	{
		for (auto it = _meshRenderers.begin(); it != _meshRenderers.end(); it++)
		{
			if (*it == renderer)
			{
				_meshRenderers.erase(it);
				break;
			}
		}
	}

	void RenderingEngine::AddPointLight(PointLight* light)
	{
		_pointLights.push_back(light);
	}

	void RenderingEngine::RemovePointLight(PointLight* light)
	{
		for (auto it = _pointLights.begin(); it != _pointLights.end(); it++)
		{
			if (*it == light)
			{
				_pointLights.erase(it);
				break;
			}
		}
	}

	void RenderingEngine::AddDirectionalLight(DirectionalLight* light)
	{
		_directionalLights.push_back(light);
	}

	void RenderingEngine::RemoveDirectionalLight(DirectionalLight* light)
	{
		for (auto it = _directionalLights.begin(); it != _directionalLights.end(); it++)
		{
			if (*it == light)
			{
				_directionalLights.erase(it);
				break;
			}
		}
	}

	void RenderingEngine::AddSpotLight(SpotLight* light)
	{
		_spotLights.push_back(light);
	}

	void RenderingEngine::RemoveSpotLight(SpotLight* light)
	{
		for (auto it = _spotLights.begin(); it != _spotLights.end(); it++)
		{
			if (*it == light)
			{
				_spotLights.erase(it);
				break;
			}
		}
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

		if (_meshRenderers.size() > 0)
		{
			_modelMatrices.clear();
			//ambient

			Shader* shader = ShaderManager::GetInstance()->UseShader("forward-ambient");
			shader->SetUniform1f("ambientIntensity", _ambientLight->intensity);
			shader->SetUniform3f("ambientColor", _ambientLight->color);

			glm::mat4 viewProjection = _camera->GetViewProjectionMatrix();
			shader->SetUniformMatrix4fv("vp", viewProjection);

			//sort meshrenderers into groups;
			std::sort(_meshRenderers.begin(), _meshRenderers.end());

			//get first enabled renderer
			int firstEnabled;
			for (firstEnabled = 0; firstEnabled < _meshRenderers.size(); firstEnabled++)
				if (_meshRenderers[firstEnabled]->IsEnabled())
					break;

			Mesh* currentMesh = _meshRenderers[firstEnabled]->mesh;
			Material* currentMaterial = _meshRenderers[firstEnabled]->material;
			MeshRenderer* renderer;
			int instanceCount = 1;

			for (int i = firstEnabled + 1; i < _meshRenderers.size(); i++)
			{
				if (!_meshRenderers[i]->IsEnabled()) continue;

				renderer = _meshRenderers[i];

				//if we changed meshes, we can now call draw on the previous one
				//since we buffered all its info
				if (currentMesh != renderer->mesh || currentMaterial != renderer->material || instanceCount >= 1000)
				{
					const glm::mat4* data = &_modelMatrices[0];
					currentMesh->Render(instanceCount, currentMaterial, data);
					_modelMatrices.clear();

					currentMesh = renderer->mesh;
					currentMaterial = renderer->material;
					instanceCount = 1;
				}
				else
				{
					//increase how many we'll be instancing
					instanceCount++;
				}

				_modelMatrices.push_back(renderer->gameObject->transform->GetTransformation());
			}

			const glm::mat4* data = &_modelMatrices[0];
			currentMesh->Render(instanceCount, currentMaterial, data);

			//point

			shader = ShaderManager::GetInstance()->UseShader("forward-point");
			shader->SetUniformMatrix4fv("vp", viewProjection);
			for (PointLight* point : _pointLights)
			{
				if (!point->IsEnabled()) continue;

				shader->SetUniform3f("lightPosition", point->gameObject->transform->GetTransformedPosition());
				shader->SetUniform3f("lightColor", point->color);
				shader->SetUniform1f("lightIntensity", point->intensity);
				shader->SetUniform1f("range", point->range);

				currentMesh = _meshRenderers[firstEnabled]->mesh;
				currentMaterial = _meshRenderers[firstEnabled]->material;
				renderer;
				instanceCount = 1;

				for (int i = firstEnabled + 1; i < _meshRenderers.size(); i++)
				{
					if (!_meshRenderers[i]->IsEnabled()) continue;

					renderer = _meshRenderers[i];

					//if we changed meshes, we can now call draw on the previous one
					//since we buffered all its info
					if (currentMesh != renderer->mesh || currentMaterial != renderer->material || instanceCount >= 1000)
					{
						const glm::mat4* data = &_modelMatrices[0];
						currentMesh->Render(instanceCount, currentMaterial, data);
						_modelMatrices.clear();

						currentMesh = renderer->mesh;
						currentMaterial = renderer->material;
						instanceCount = 1;
					}
					else
					{
						//increase how many we'll be instancing
						instanceCount++;
					}

					_modelMatrices.push_back(renderer->gameObject->transform->GetTransformation());
				}

				const glm::mat4* data = &_modelMatrices[0];
				currentMesh->Render(instanceCount, currentMaterial, data);
			}

			//directional

			shader = ShaderManager::GetInstance()->UseShader("forward-directional");
			shader->SetUniformMatrix4fv("vp", viewProjection);
			for (DirectionalLight* dirLight : _directionalLights)
			{
				if (!dirLight->IsEnabled()) continue;

				shader->SetUniform3f("lightDirection", dirLight->gameObject->transform->GetForward());
				shader->SetUniform3f("lightColor", dirLight->color);
				shader->SetUniform1f("lightIntensity", dirLight->intensity);

				currentMesh = _meshRenderers[firstEnabled]->mesh;
				currentMaterial = _meshRenderers[firstEnabled]->material;
				renderer;
				instanceCount = 1;

				for (int i = firstEnabled + 1; i < _meshRenderers.size(); i++)
				{
					if (!_meshRenderers[i]->IsEnabled()) continue;

					renderer = _meshRenderers[i];

					//if we changed meshes, we can now call draw on the previous one
					//since we buffered all its info
					if (currentMesh != renderer->mesh || currentMaterial != renderer->material || instanceCount >= 1000)
					{
						const glm::mat4* data = &_modelMatrices[0];
						currentMesh->Render(instanceCount, currentMaterial, data);
						_modelMatrices.clear();

						currentMesh = renderer->mesh;
						currentMaterial = renderer->material;
						instanceCount = 1;
					}
					else
					{
						//increase how many we'll be instancing
						instanceCount++;
					}

					_modelMatrices.push_back(renderer->gameObject->transform->GetTransformation());
				}

				const glm::mat4* data = &_modelMatrices[0];
				currentMesh->Render(instanceCount, currentMaterial, data);
			}
		}

		RenderSkybox();

		if (_showLightingDebug)
			ShowLightingDebug();
	}

	void RenderingEngine::Deferred(GameObject* gameObject)
	{
		_gbuffer->StartFrame();

		GeometryPass(gameObject);

		glEnable(GL_STENCIL_TEST);

		for (PointLight* pointLight : _pointLights)
		{
			if (!pointLight->IsEnabled()) continue;

			StencilPassPoint(pointLight);
			PointLightPass(pointLight);
		}

		for (SpotLight* spotLight : _spotLights)
		{
			if (!spotLight->IsEnabled()) continue;

			StencilPassSpot(spotLight);
			SpotLightPass(spotLight);
		}

		glDisable(GL_STENCIL_TEST);

		DirectionalLightPass(gameObject);

		FinalPass();
	}

	void RenderingEngine::GeometryPass(GameObject* gameObject)
	{
		Shader* shader = ShaderManager::GetInstance()->UseShader("geometry");

		_gbuffer->BindForGeometryPass();

		glDepthMask(GL_TRUE);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);

		if (_meshRenderers.size() > 0)
		{
			_modelMatrices.clear();

			//sort meshrenderers into groups
			std::sort(_meshRenderers.begin(), _meshRenderers.end());

			glm::mat4 viewProjection = _camera->GetViewProjectionMatrix();
			shader->SetUniformMatrix4fv("vp", viewProjection);

			//get first enabled renderer
			int firstEnabled;
			for (firstEnabled = 0; firstEnabled < _meshRenderers.size(); firstEnabled++)
				if (_meshRenderers[firstEnabled]->IsEnabled())
					break;

			Mesh* currentMesh = _meshRenderers[firstEnabled]->mesh;
			Material* currentMaterial = _meshRenderers[firstEnabled]->material;
			MeshRenderer* renderer;
			int instanceCount = 1;

			for (int i = firstEnabled + 1; i < _meshRenderers.size(); i++)
			{
				if (!_meshRenderers[i]->IsEnabled()) continue;

				renderer = _meshRenderers[i];

				//if we changed meshes, we can now call draw on the previous one
				//since we buffered all its info
				if (currentMesh != renderer->mesh || currentMaterial != renderer->material || instanceCount >= 1000)
				{
					const glm::mat4* data = &_modelMatrices[0];
					currentMesh->Render(instanceCount, currentMaterial, data);

					_modelMatrices.clear();
					currentMesh = renderer->mesh;
					currentMaterial = renderer->material;
					instanceCount = 1;
				}
				else
				{
					//increase how many we'll be instancing
					instanceCount++;
				}

				_modelMatrices.push_back(renderer->gameObject->transform->GetTransformation());
			}

			const glm::mat4* data = &_modelMatrices[0];
			currentMesh->Render(instanceCount, currentMaterial, data);
		}

		glDepthMask(GL_FALSE);
	}

	void RenderingEngine::StencilPassPoint(PointLight* light)
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
			(glm::translate(glm::mat4(), light->gameObject->transform->GetTransformedPosition()) *
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
		pointShader->SetUniform1f("shininess", 32.0f);

		pointShader->SetUniform1i("positionMap", GBuffer::GBufferTextureType::Position);
		pointShader->SetUniform1i("albedoMap", GBuffer::GBufferTextureType::Diffuse);
		pointShader->SetUniform1i("normalMap", GBuffer::GBufferTextureType::Normal);
		pointShader->SetUniform1i("specularMap", GBuffer::GBufferTextureType::Specular);

		pointShader->SetUniform3f("pointLight.light.color", light->color);
		pointShader->SetUniform1f("pointLight.light.intensity", light->intensity);
		pointShader->SetUniform3f("pointLight.position", light->gameObject->transform->GetTransformedPosition());
		pointShader->SetUniform1f("pointLight.range", light->range);

		glm::mat4 viewProjection = _camera->GetViewProjectionMatrix();

		pointShader->SetUniformMatrix4fv("mvp", viewProjection *
			(glm::translate(glm::mat4(), light->gameObject->transform->GetTransformedPosition()) *
				glm::scale(glm::mat4(), glm::vec3(light->range))
				)
		);

		_sphere->Render();

		glCullFace(GL_BACK);

		glDisable(GL_BLEND);
	}

	void RenderingEngine::StencilPassSpot(SpotLight* light)
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

		float xyScaleOuter = CalculateSpotLightScale(light);
		nullShader->SetUniformMatrix4fv("mvp", viewProjection *
			(glm::translate(glm::mat4(), light->gameObject->transform->GetTransformedPosition()) *
				glm::mat4_cast(light->gameObject->transform->GetTransformedRotation()) *
				glm::scale(glm::mat4(), glm::vec3(xyScaleOuter, xyScaleOuter, light->range))
			)
		);

		_cone->Render();
	}

	void RenderingEngine::SpotLightPass(SpotLight* light)
	{
		_gbuffer->BindForLightPass();

		glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		Shader* spotLight = ShaderManager::GetInstance()->UseShader("spotLight");
		spotLight->SetUniform2f("screenSize", _windowWidth, _windowHeight);
		spotLight->SetUniform1f("ambientIntensity", _ambientLight->intensity);
		spotLight->SetUniform3f("ambientColor", _ambientLight->color);
		spotLight->SetUniform3f("eyeWorldPos", _camera->gameObject->transform->GetTransformedPosition());
		spotLight->SetUniform1f("shininess", 32.0f);

		spotLight->SetUniform1i("positionMap", GBuffer::GBufferTextureType::Position);
		spotLight->SetUniform1i("albedoMap", GBuffer::GBufferTextureType::Diffuse);
		spotLight->SetUniform1i("normalMap", GBuffer::GBufferTextureType::Normal);
		spotLight->SetUniform1i("specularMap", GBuffer::GBufferTextureType::Specular);

		spotLight->SetUniform3f("spotLight.light.color", light->color);
		spotLight->SetUniform1f("spotLight.light.intensity", light->intensity);
		spotLight->SetUniform3f("spotLight.position", light->gameObject->transform->GetTransformedPosition());
		spotLight->SetUniform3f("spotLight.direction", light->gameObject->transform->GetForward());
		spotLight->SetUniform1f("spotLight.range", light->range);
		spotLight->SetUniform1f("spotLight.angle", glm::cos(glm::radians((light->angle - _falloff) / 2)));
		spotLight->SetUniform1f("spotLight.cutoffAngle", glm::cos(glm::radians((light->angle + _falloff) / 2)));

		glm::mat4 viewProjection = _camera->GetViewProjectionMatrix();

		float xyScaleOuter = CalculateSpotLightScale(light);
		spotLight->SetUniformMatrix4fv("mvp", viewProjection *
			(glm::translate(glm::mat4(), light->gameObject->transform->GetTransformedPosition()) *
				glm::mat4_cast(light->gameObject->transform->GetTransformedRotation()) *
				glm::scale(glm::mat4(), glm::vec3(xyScaleOuter, xyScaleOuter, light->range))
			)
		);

		_cone->Render();

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

		Shader* directional = ShaderManager::GetInstance()->UseShader("directionalLight");
		directional->SetUniform2f("screenSize", _windowWidth, _windowHeight);
		directional->SetUniform1f("ambientIntensity", _ambientLight->intensity);
		directional->SetUniform3f("ambientColor", _ambientLight->color);
		directional->SetUniform3f("eyeWorldPos", _camera->gameObject->transform->GetTransformedPosition());
		directional->SetUniform1f("shininess", 32.0f);

		directional->SetUniform1i("positionMap", GBuffer::GBufferTextureType::Position);
		directional->SetUniform1i("albedoMap", GBuffer::GBufferTextureType::Diffuse);
		directional->SetUniform1i("normalMap", GBuffer::GBufferTextureType::Normal);
		directional->SetUniform1i("specularMap", GBuffer::GBufferTextureType::Specular);

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

		RenderSkybox();

		if (_showLightingDebug)
			ShowLightingDebug();
	}

	float RenderingEngine::CalculateSpotLightScale(SpotLight* light)
	{
		return 2 * light->range * glm::tan(glm::radians((light->angle + _falloff) / 2));
	}

	void RenderingEngine::ShowLightingDebug()
	{
		Shader* lightingDebug = ShaderManager::GetInstance()->UseShader("lightingDebug");
		glm::mat4 viewProjection = _camera->GetViewProjectionMatrix();
		lightingDebug->SetUniform3f("color", glm::vec3(1.0f, 1.0f, 0.0f));

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glEnable(GL_DEPTH_TEST);
		//glDisable(GL_CULL_FACE);

		for (PointLight* light : _pointLights)
		{
			if (!light->IsEnabled()) continue;

			lightingDebug->SetUniformMatrix4fv("mvp", viewProjection *
				(glm::translate(glm::mat4(), light->gameObject->transform->GetPosition()) *
					glm::scale(glm::mat4(), glm::vec3(light->range))
					)
			);

			_sphere->Render();

			//render center point

			lightingDebug->SetUniformMatrix4fv("mvp", viewProjection *
				(glm::translate(glm::mat4(), light->gameObject->transform->GetPosition()) *
					glm::scale(glm::mat4(), glm::vec3(0.1f))
					)
			);

			_sphere->Render();
		}

		for (DirectionalLight* light : _directionalLights)
		{
			if (!light->IsEnabled()) continue;

			lightingDebug->SetUniformMatrix4fv("mvp", viewProjection * light->gameObject->transform->GetTransformation());

			_arrow->Render();
		}

		for (SpotLight* light : _spotLights)
		{
			if (!light->IsEnabled()) continue;

			lightingDebug->SetUniform3f("color", glm::vec3(1.0f, 1.0f, 0.0f));

			float xyScaleInner = 2 * light->range * glm::tan(glm::radians((light->angle - _falloff) / 2));
			lightingDebug->SetUniformMatrix4fv("mvp", viewProjection *
				(glm::translate(glm::mat4(), light->gameObject->transform->GetTransformedPosition()) *
					glm::mat4_cast(light->gameObject->transform->GetTransformedRotation()) *
					glm::scale(glm::mat4(), glm::vec3(xyScaleInner, xyScaleInner, light->range))
					)
			);

			_cone->Render();

			lightingDebug->SetUniform3f("color", glm::vec3(1.0f, 0.0f, 0.0f));

			float xyScaleOuter = CalculateSpotLightScale(light);
			lightingDebug->SetUniformMatrix4fv("mvp", viewProjection *
				(glm::translate(glm::mat4(), light->gameObject->transform->GetTransformedPosition()) *
					glm::mat4_cast(light->gameObject->transform->GetTransformedRotation()) *
					glm::scale(glm::mat4(), glm::vec3(xyScaleOuter, xyScaleOuter, light->range))
					)
			);

			_cone->Render();
		}

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_DEPTH_TEST);
		//glEnable(GL_CULL_FACE);
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
		_skyboxMesh = MeshManager::GetInstance()->GetMesh("cubeInvert");
	}
}
