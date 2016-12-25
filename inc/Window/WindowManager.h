#pragma once
#include <map>
#include "Window\Window.h"

enum EWindowMode;
class WindowManager
{
public:
	static void Init();
	static WindowManager* GetInstance();
	static void Shutdown();

	Window* CreateWindow(Window::EWindowMode windowMode, int width, int height);
	Window* CreateWindow(Window::EWindowMode windowMode, int width, int height, const char* title);
	void DestroyWindow(Window* window);
	void DestroyWindow(int id);

	Window* GetWindow(int id);
	Window* GetMainWindow();
private:
	WindowManager();
	~WindowManager();

	static WindowManager* _instance;

	int _nextWindowId = 1;
	Window* _mainWindow = nullptr;
	std::map<int, Window*> _windows;
};

