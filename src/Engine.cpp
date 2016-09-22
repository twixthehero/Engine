#include "Engine.h"


Engine::Engine()
{
}


Engine::~Engine()
{
}

int Engine::Run()
{
	if (Init() == -1)
		return -1;

	while (!glfwWindowShouldClose(window))
	{
		Update();
		Render();

		glfwPollEvents();
	}

	Exit();

	return 0;
}

int Engine::Init()
{
	if (!glfwInit())
		return -1;

	if (!CreateWindow())
		return -1;

	glfwMakeContextCurrent(window);

	return 0;
}

void Engine::Exit()
{
	if (window)
		glfwTerminate();
}

void Engine::Update()
{

}

void Engine::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	glfwSwapBuffers(window);
}

bool Engine::CreateWindow()
{
	window = glfwCreateWindow(800, 600, "GameEngine", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		return false;
	}

	return true;
}
