#include "Window\Window.h"
#include <iostream>

Window::Window(int id, EWindowMode windowMode) : Window(id, windowMode, 800, 600)
{
}

Window::Window(int id, EWindowMode windowMode, int width, int height) : Window(id, windowMode, width, height, "Default Title")
{
}

Window::Window(int id, EWindowMode windowMode, int width, int height, const char* title)
{
	_id = id;
	_windowMode = windowMode;

	_width = width;
	_height = height;
	_title = title;
}


Window::~Window()
{
}

int Window::GetID()
{
	return _id;
}

bool Window::Create()
{
	if (_window != nullptr)
	{
		Destroy();
		_window = nullptr;
	}

	GLFWmonitor* monitor = _windowMode == EWindowMode::FULLSCREEN ? glfwGetPrimaryMonitor() : NULL;

	if (_windowMode == EWindowMode::BORDERLESS)
	{
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);

		_width = mode->width;
		_height = mode->height;
		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	}

	glfwWindowHint(GLFW_SAMPLES, _samples);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, _contextVersionMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, _contextVersionMinor);

	_window = glfwCreateWindow(_width, _height, _title, monitor, NULL);

	if (!_window)
	{
		std::cout << "Failed to create window!" << std::endl;
		return false;
	}

	return true;
}

void Window::Destroy()
{
	glfwDestroyWindow(_window);
}

void Window::MakeContextCurrent()
{
	glfwMakeContextCurrent(_window);
}

bool Window::ShouldClose()
{
	return glfwWindowShouldClose(_window) == 1;
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(_window);
}

void Window::SetMultiSamples(int samples)
{
	_samples = samples;
}

void Window::SetOpenGLVersion(int major, int minor)
{
	_contextVersionMajor = major;
	_contextVersionMinor = minor;
}

void Window::SetTitle(const char* title)
{
	glfwSetWindowTitle(_window, title);
}

int Window::GetKey(KeyCode key)
{
	return glfwGetKey(_window, key);
}

int Window::GetMouse(ButtonCode button)
{
	return glfwGetMouseButton(_window, button);
}