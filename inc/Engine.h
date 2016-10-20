#pragma once
#include <glew\glew.h>
#include <GLFW\glfw3.h>

class Engine
{
public:
    Engine();
    ~Engine();
private:
	int Init();
	void Run();
	void Shutdown();

	GLFWwindow* _window;
};

