#pragma once
#include <vector>

class GameObject;
class Light;
class RenderingEngine
{
public:
	RenderingEngine();
	~RenderingEngine();

	static void Init();
	static RenderingEngine* GetInstance();
	static void Shutdown();

	void Render(GameObject* gameObject);

	Light* GetLight();
private:
	static RenderingEngine* _instance;

	Light* _ambientLight;
	std::vector<Light*> _lights;

	Light* _currentLight;
};

