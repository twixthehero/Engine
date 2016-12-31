#pragma once
#include <vector>

namespace VoxEngine
{
	class GBuffer;
	class GameObject;
	class Light;
	class Camera;
	class Component;
	class MeshRenderer;
	class Window;
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

		void RenderGeometry(GameObject* gameObject);
		void RenderLighting(GameObject* gameObject);

		static RenderingEngine* _instance;

		GBuffer* _gbuffer;
		Window* _window;
		int _windowWidth;
		int _windowHeight;

		Camera* _camera;

		Light* _ambientLight;
		std::vector<Light*> _lights;

		std::vector<Component*> renderingComponents;
		std::vector<MeshRenderer*> meshRenderers;
		Light* _currentLight;
	};
}
