#pragma once
#include <vector>

class GameObject;
class Light;
class Camera;
class RenderingEngine
{
public:
	static void Init();
	static RenderingEngine* GetInstance();
	static void Shutdown();

	void Render(GameObject* gameObject);

	void SetCamera(Camera* camera);
	Camera* GetCamera();
	Light* GetAmbientLight();
private:
	RenderingEngine();
	~RenderingEngine();

	static RenderingEngine* _instance;

	Camera* _camera;

	Light* _ambientLight;
	std::vector<Light*> _lights;

	Light* _currentLight;
};

