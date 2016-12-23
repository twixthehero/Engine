#pragma once
#include <glew\glew.h>

class Shader;
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
	double _lastTime;

	WindowManager* _windowManager;
	Window* _window;

	RenderingEngine* _renderingEngine;
	
	Scene* _scene;
};

