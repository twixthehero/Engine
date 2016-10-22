#include "Engine.h"
#include "Window\Window.h"
#include "Window\WindowManager.h"
#include "Input.h"
#include "Draw\RenderingEngine.h"
#include "Scene.h"
#include "Core\GameObject.h"
#include "Component\Camera.h"
#include <iostream>

Engine::Engine()
{
	if (Init() != 0)
	{
		std::cout << "Error initializing engine!" << std::endl;
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
	if (!glfwInit())
	{
		return -1;
	}

	WindowManager::Init();
	_windowManager = WindowManager::GetInstance();

	_window = _windowManager->CreateWindow(EWindowMode::WINDOWED, 800, 600, "Engine");

	if (_window == nullptr)
	{
		Shutdown();
		return -1;
	}
	
	_window->MakeContextCurrent();

	Input::Init();
	RenderingEngine::Init();

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		Shutdown();
		return -1;
	}

	return 0;
}

void Engine::Run()
{
	_running = true;

	_scene = new Scene();
	GameObject* camera = new GameObject("Camera");
	camera->tag = "MainCamera";
	camera->AddComponent(new Camera());
	_scene->AddObject(camera);

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
	RenderingEngine::Shutdown();
	Input::Shutdown();
	WindowManager::Shutdown();
}

void Engine::Update()
{
	Input::Update();

	if (_window->ShouldClose() || Input::GetKeyUp(KeyCode::Escape))
		_running = false;

	_scene->Update();
}

void Engine::Render()
{
	_scene->Render(_renderingEngine);
}