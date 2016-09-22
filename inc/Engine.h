#pragma once
#include <GLFW/glfw3.h>

class Engine
{
public:
    Engine();
    ~Engine();

	int Run();
private:
	bool CreateWindow();
	int Init();
	void Exit();
	void Update();
	void Render();

	GLFWwindow* window;
};

