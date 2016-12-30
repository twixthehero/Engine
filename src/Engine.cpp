#include "Engine.h"
#include "Logger.h"
#include "Window\Window.h"
#include "Window\WindowManager.h"
#include "Input.h"
#include "Draw\RenderingEngine.h"
#include "TextureManager.h"
#include "Scene.h"
#include "Core\GameObject.h"
#include "Component\Camera.h"
#include "MeshManager.h"
#include <iostream>
#include "Core\Material.h"
#include "Draw\Shader.h"
#include "Component\MeshRenderer.h"
#include "Component\Transform.h"
#include "Component\FlyMove.h"
#include "EngineTime.h"
#include "Component\FreeLook.h"
#include "Component\Oscillate.h"

namespace VoxEngine
{
	Engine::Engine()
	{
		if (Init() != 0)
		{
			Logger::WriteLine("Error initializing engine!");
			return;
		}

		Run();

		Shutdown();
	}

	Engine::~Engine()
	{
	}

	int Engine::Init()
	{
		Logger::Init();

		if (!glfwInit())
		{
			return -1;
		}

		WindowManager::Init();
		_windowManager = WindowManager::GetInstance();

		_window = _windowManager->CreateNewWindow(EWindowMode::WINDOWED, 800, 600, "Engine");

		if (_window == nullptr)
		{
			Logger::WriteLine("Failed to create window");

			Shutdown();
			return -1;
		}

		_window->MakeContextCurrent();

		if (gl3wInit())
		{
			Logger::WriteLine("Failed to initialize gl3w");

			Shutdown();
			return -1;
		}

		Logger::WriteLine("OpenGL Version: " + std::string((const char*)glGetString(GL_VERSION)));
		Logger::WriteLine("GLSL Version: " + std::string((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION)));

		Input::Init();
		RenderingEngine::Init();
		TextureManager::Init();
		MeshManager::Init();

		_renderingEngine = RenderingEngine::GetInstance();

		return 0;
	}

	void Engine::Run()
	{
		_running = true;

		_scene = new Scene();
		GameObject* cameraObject = new GameObject("Camera");
		cameraObject->transform->position.z = 10;
		cameraObject->name = "MainCamera";
		cameraObject->tag = "MainCamera";
		Camera* camera = new Camera();
		camera->SetFOV(60);
		camera->SetAspectRatio(800.0f / 600);
		camera->SetNearClipping(0.01f);
		camera->SetFarClipping(500.0f);
		cameraObject->AddComponent(camera);
		FlyMove* flyMove = new FlyMove();
		cameraObject->AddComponent(flyMove);
		FreeLook* freeLook = new FreeLook();
		cameraObject->AddComponent(freeLook);
		_scene->AddObject(cameraObject);

		Mesh* mesh = MeshManager::GetInstance()->GetMesh("cube");
		Shader* shader = new Shader(1, "default");

		GameObject* emmaCube = new GameObject("EmmaCube");
		emmaCube->transform->position.x = 3;
		emmaCube->transform->position.y = 1;
		Material* mat_emma = new Material(shader, TextureManager::GetInstance()->GetTexture("emma.png"));
		MeshRenderer* meshRenderer = new MeshRenderer(mesh, mat_emma);
		emmaCube->AddComponent(meshRenderer);

		GameObject* danielCube = new GameObject("DanielCube");
		Material* mat_daniel = new Material(shader, TextureManager::GetInstance()->GetTexture("daniel.png"));
		MeshRenderer* meshRenderer2 = new MeshRenderer(mesh, mat_daniel);
		danielCube->AddComponent(meshRenderer2);
		Oscillate* oscillate = new Oscillate(Oscillate::EAxis::Y, 3);
		danielCube->AddComponent(oscillate);
		//emmaCube->SetParent(danielCube);
		_scene->AddObject(danielCube);

		while (_running)
		{
			Update();
			Render();

			_window->SwapBuffers();

			glfwPollEvents();
		}
	}

	void Engine::Shutdown()
	{
		MeshManager::Shutdown();
		TextureManager::Shutdown();
		RenderingEngine::Shutdown();
		Input::Shutdown();
		WindowManager::Shutdown();
		Logger::Shutdown();
	}

	void Engine::Update()
	{
		double time = glfwGetTime();

		Time::time = (float)time;
		Time::deltaTime = (float)(time - _lastTime);
		_lastTime = time;

		Input::Update();

		if (_window->ShouldClose() || Input::GetKeyUp(KeyCode::Escape))
			_running = false;

		_scene->Update();
	}

	void Engine::Render()
	{
		_scene->Render(_renderingEngine);
	}
}
