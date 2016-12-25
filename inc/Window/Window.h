#pragma once
#include <glfw\glfw3.h>
#include <glm\glm.hpp>
#include "Input.h"

enum KeyCode;
enum ButtonCode;
class Window
{
public:
	enum EWindowMode
	{
		NONE, FULLSCREEN, BORDERLESS, WINDOWED
	};

	Window(int id, EWindowMode windowMode);
	Window(int id, EWindowMode windowMode, int width, int height);
	Window(int id, EWindowMode windowMode, int width, int height, const char* title);
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

	int GetKey(KeyCode key);
	int GetMouse(ButtonCode button);
	glm::vec2 GetMousePosition();
	void SetMousePosition(double x, double y);
private:
	int _id;
	GLFWwindow* _window = nullptr;
	EWindowMode _windowMode;
	double* _mouseX;
	double* _mouseY;

	int _width;
	int _height;
	const char* _title;

	int _contextVersionMajor = 4;
	int _contextVersionMinor = 0;

	int _samples = 0;
};

