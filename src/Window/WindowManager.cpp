#include "Window\WindowManager.h"
#include <iostream>
#include "Window\Window.h"
#include "Logger.h"

namespace VoxEngine
{
	WindowManager* WindowManager::_instance = nullptr;

	WindowManager::WindowManager()
	{
		_windows = std::map<int, Window*>();
	}

	WindowManager::~WindowManager()
	{
		for (std::map<int, Window*>::iterator it = _windows.begin(); it != _windows.end(); it++)
			delete it->second;
	}

	void WindowManager::Init()
	{
		if (!glfwInit())
			Logger::WriteLine("Failed to initialize GLFW!");

		_instance = new WindowManager();
	}

	WindowManager* WindowManager::GetInstance()
	{
		return _instance;
	}

	void WindowManager::Shutdown()
	{
		if (_instance != nullptr)
		{
			delete _instance;
			_instance = nullptr;
		}

		glfwTerminate();
	}

	Window* WindowManager::CreateNewWindow(unsigned int windowMode, int width, int height)
	{
		return CreateNewWindow(windowMode, width, height, "Engine");
	}

	Window* WindowManager::CreateNewWindow(unsigned int windowMode, int width, int height, const char* title)
	{
		return CreateNewWindow(windowMode, width, height, title, 0);
	}
	
	Window* WindowManager::CreateNewWindow(unsigned int windowMode, int width, int height, const char* title, int samples)
	{
		Window* newWindow = new Window(_nextWindowId++, windowMode, width, height, title);
		newWindow->SetMultiSamples(samples);

		if (!newWindow->Create())
			return nullptr;

		_mainWindow = newWindow;
		_windows.insert(std::pair<int, Window*>(newWindow->GetID(), newWindow));

		return newWindow;
	}

	void WindowManager::DestroyWindow(int id)
	{
		std::map<int, Window*>::iterator it;

		for (it = _windows.begin(); it != _windows.end(); it++)
			if (it->second->GetID() == id)
			{
				it->second->Destroy();
				delete it->second;
				break;
			}

		_windows.erase(it);
	}

	void WindowManager::DestroyWindow(Window* window)
	{
		std::map<int, Window*>::iterator it;

		for (it = _windows.begin(); it != _windows.end(); it++)
			if (it->second->GetID() == window->GetID())
			{
				it->second->Destroy();
				delete it->second;
				break;
			}

		_windows.erase(it);
	}

	Window* WindowManager::GetWindow(int id)
	{
		for (std::map<int, Window*>::iterator it = _windows.begin(); it != _windows.end(); it++)
			if (it->second->GetID() == id)
				return it->second;
	}

	Window* WindowManager::GetMainWindow()
	{
		return _mainWindow;
	}
}
