#pragma once
#include <glew\glew.h>

class WindowManager;
class Window;
class RenderingEngine;
class Scene;
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

	bool _running;

	WindowManager* _windowManager;
	Window* _window;

	RenderingEngine* _renderingEngine;
	
	Scene* _scene;
};

