#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Input.h"

namespace VoxEngine
{
	enum EWindowMode
	{
		NO_WINDOW_MODE = 0,
		FULLSCREEN = 1,
		BORDERLESS = 2,
		WINDOWED = 3
	};

	class Window
	{
	public:
		Window(int id, unsigned int windowMode);
		Window(int id, unsigned int windowMode, int width, int height);
		Window(int id, unsigned int windowMode, int width, int height, const char* title);
		~Window();

		int GetID();
		int GetWidth();
		int GetHeight();

		bool Create();
		void Destroy();

		void MakeContextCurrent();
		bool ShouldClose();
		void SwapBuffers();

		void SetMultiSamples(int samples);
		void SetOpenGLVersion(int major, int minor);
		void SetTitle(const char* title);

		int GetKey(unsigned int key);
		int GetMouse(unsigned int button);
		glm::vec2 GetMousePosition();
		void SetMousePosition(double x, double y);
	private:
		int _id;
		GLFWwindow* _window = nullptr;
		unsigned int _windowMode;
		double* _mouseX;
		double* _mouseY;

		int _width;
		int _height;
		const char* _title;

		int _contextVersionMajor = 4;
		int _contextVersionMinor = 0;

		int _samples = 0;
	};
}