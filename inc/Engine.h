#pragma once
#include <glew\glew.h>
#include <Window\Window.h>
#include "Window\WindowManager.h"

class Engine
{
public:
    Engine();
    ~Engine();
private:
	int Init();
	void Run();
	void Shutdown();

	void Update();
	void Render();

	WindowManager* _windowManager;
	Window* _window;
};

