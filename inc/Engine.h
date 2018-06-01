#pragma once
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

namespace VoxEngine
{
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

		static void DebugCallback(unsigned int source, unsigned int type,
			unsigned int id, unsigned int severity,
			int length, const char* message,
			const void* userParam);
	private:
		int Init();
		void Run();
		void Shutdown();

		void Update();
		void Render();

		bool _running;
		double _lastTime;

		float _secondTime;
		int _fps;
		int _frameCounter;

		WindowManager* _windowManager;
		Window* _window;

		RenderingEngine* _renderingEngine;

		Scene* _scene;
	};
}
