#include "Draw\RenderingEngine.h"
#include <GL\gl3w.h>
#include "Component\Light.h"
#include "Core\GameObject.h"
#include "Component\Camera.h"
#include "Component\MeshRenderer.h"
#include "Draw\GBuffer.h"
#include "Window\WindowManager.h"
#include "Window\Window.h"
#include "Logger.h"
#include <iostream>

namespace VoxEngine
{
	RenderingEngine* RenderingEngine::_instance = nullptr;

	RenderingEngine::RenderingEngine()
	{
		_gbuffer = new GBuffer();

		Window* window = WindowManager::GetInstance()->GetMainWindow();

		if (!_gbuffer->Init(window->GetWidth(), window->GetHeight()))
		{
			Logger::WriteLine("Failed to initialize the GBuffer!");
		}

		_lights = std::vector<Light*>();

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

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		glEnable(GL_BLEND);	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderingComponents.clear();
		meshRenderers.clear();

		if (_camera != Camera::main)
			SetCamera(Camera::main);

		_currentLight = _ambientLight;

		gameObject->GetComponentsInChildren(EComponentType::MESH_RENDERER, renderingComponents);

		for (auto it = renderingComponents.begin(); it != renderingComponents.end(); it++)
			meshRenderers.push_back(dynamic_cast<MeshRenderer*>(*it));

		for (MeshRenderer* renderer : meshRenderers)
			renderer->Render();

		/*for (Light* light : _lights)
		{
			_currentLight = light;
			gameObject->Render(this);
		}*/
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
