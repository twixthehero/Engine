#include "Draw\RenderingEngine.h"
#include <GL\gl3w.h>
#include "ShaderManager.h"
#include "Component\Light.h"
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
		_ambientLight->color.r = 0.0f;
		_ambientLight->color.g = 0.0f;
		_ambientLight->color.b = 0.25f;
		_ambientLight->color.a = 1.0f;
	}

	RenderingEngine::~RenderingEngine()
	{
	}

	void RenderingEngine::Init()
	{
		_instance = new RenderingEngine();

		glClearColor(0.2f, 0.4f, 0.85f, 1.0f);

		glEnable(GL_CULL_FACE);	glCullFace(GL_BACK);
		glEnable(GL_DEPTH_TEST); glDepthFunc(GL_LEQUAL);
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

	void RenderingEngine::Render(GameObject* gameObject)
	{
		if (_camera != Camera::main)
			SetCamera(Camera::main);

		GeometryPass(gameObject);
		//BeginLightingPasses();
		//AmbientLightPass(gameObject);
		//PointLightPass(gameObject);
		DirectionalLightPass(gameObject);

		/*for (Light* light : _lights)
		{
			_currentLight = light;
			gameObject->Render(this);
		}*/
	}

	void RenderingEngine::GeometryPass(GameObject* gameObject)
	{
		Shader* shader = ShaderManager::GetInstance()->UseShader("geometry");

		_gbuffer->BindForWriting();

		//glDepthMask(GL_TRUE);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glEnable(GL_DEPTH_TEST);
		//glDisable(GL_BLEND);

		renderingComponents.clear();
		meshRenderers.clear();
		gameObject->GetComponentsInChildren(EComponentType::MESH_RENDERER, renderingComponents);

		for (auto it = renderingComponents.begin(); it != renderingComponents.end(); it++)
			meshRenderers.push_back(dynamic_cast<MeshRenderer*>(*it));

		Camera* main = Camera::main;
		glm::mat4 mvp;
		glm::mat4 viewProjection = main->GetViewProjectionMatrix();

		for (MeshRenderer* renderer : meshRenderers)
		{
			shader->SetUniformMatrix4fv("mvp", viewProjection * renderer->gameObject->transform->GetTransformation());
			renderer->Render();
		}

		//glDepthMask(GL_FALSE);
		
		//glDisable(GL_DEPTH_TEST);
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

		_quad->Render();
	}

	void RenderingEngine::PointLightPass(GameObject* gameObject)
	{
		ShaderManager::GetInstance()->UseShader("pointLight");


	}

	void RenderingEngine::DirectionalLightPass(GameObject* gameObject)
	{
		//ShaderManager::GetInstance()->UseShader("directionalLight");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		_gbuffer->BindForReading();

		GLsizei halfWidth = _windowWidth / 2.0f;
		GLsizei halfHeight = _windowHeight / 2.0f;

		_gbuffer->SetReadBuffer(GBuffer::GBufferTextureType::Position);
		glBlitFramebuffer(0, 0, _windowWidth, _windowHeight, 0, 0, halfWidth, halfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		_gbuffer->SetReadBuffer(GBuffer::GBufferTextureType::Diffuse);
		glBlitFramebuffer(0, 0, _windowWidth, _windowHeight, 0, halfHeight, halfWidth, _windowHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		_gbuffer->SetReadBuffer(GBuffer::GBufferTextureType::Normal);
		glBlitFramebuffer(0, 0, _windowWidth, _windowHeight, halfWidth, halfHeight, _windowWidth, _windowHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		_gbuffer->SetReadBuffer(GBuffer::GBufferTextureType::TextureCoordinate);
		glBlitFramebuffer(0, 0, _windowWidth, _windowHeight, halfWidth, 0, _windowWidth, halfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
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
