#include "Engine.h"


Engine::Engine()
{
	Init();
	
	Run();

	Shutdown();
}

Engine::~Engine()
{
}

int Engine::Init()
{
	_window = glfwCreateWindow(800, 600, "Engine", NULL, NULL);

	if (!_window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(_window);

	glfwTerminate();
}

void Engine::Run()
{

}

void Engine::Shutdown()
{

}