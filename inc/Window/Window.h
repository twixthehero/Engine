#pragma once
#include <glfw\glfw3.h>
#include "Input.h"

enum EWindowMode
{
	NONE, FULLSCREEN, BORDERLESS, WINDOWED
};

enum KeyCode;
enum ButtonCode;
class Window
{
public:
	Window(int id, EWindowMode windowMode);
	Window(int id, EWindowMode windowMode, int width, int height);
	Window(int id, EWindowMode windowMode, int width, int height, const char* title);
	~Window();

	int GetID();

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
private:
	int _id;
	GLFWwindow* _window = nullptr;
	EWindowMode _windowMode;

	int _width;
	int _height;
	const char* _title;

	int _contextVersionMajor = 4;
	int _contextVersionMinor = 0;

	int _samples = 0;
};

