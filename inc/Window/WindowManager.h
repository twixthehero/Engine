#pragma once
#include <map>

enum EWindowMode;
class Window;
class WindowManager
{
public:
	WindowManager();
	~WindowManager();

	static void Init();
	static WindowManager* GetInstance();
	static void Shutdown();

	Window* CreateWindow(EWindowMode windowMode, int width, int height);
	Window* CreateWindow(EWindowMode windowMode, int width, int height, const char* title);
	void DestroyWindow(Window* window);
	void DestroyWindow(int id);

	Window* GetWindow(int id);
	Window* GetMainWindow();
private:
	static WindowManager* _instance;

	int _nextWindowId = 1;
	Window* _mainWindow = nullptr;
	std::map<int, Window*> _windows;
};

