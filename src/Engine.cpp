#include "Engine.h"
#include "Input.h"
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
	while (!Input::GetKey(KeyCode::Escape))
	{
		Update();
		Render();

		_window->SwapBuffers();

		glfwPollEvents();
	}
}

void Engine::Shutdown()
{
	Input::Shutdown();
	WindowManager::Shutdown();
}

void Engine::Update()
{
	Input::Update();
}

void Engine::Render()
{

}