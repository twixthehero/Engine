#pragma once
#include <vector>

namespace VoxEngine
{
	class GBuffer;
	class GameObject;
	class Light;
	class PointLight;
	class DirectionalLight;
	class Camera;
	class Component;
	class MeshRenderer;
	class Window;
	class RenderingEngine
	{
	public:
		enum ERenderingMode { NONE, FORWARD, DEFERRED };

		static void Init();
		static RenderingEngine* GetInstance();
		static void Shutdown();

		void SetRenderingMode(ERenderingMode mode);
		void Render(GameObject* gameObject);

		void SetCamera(Camera* camera);
		Camera* GetCamera();
		Light* GetAmbientLight();
	private:
		RenderingEngine();
		~RenderingEngine();

		void Forward(GameObject* gameObject);
		void Deferred(GameObject* gameObject);

		void GeometryPass(GameObject* gameObject);
		void BeginLightingPasses();
		void AmbientLightPass(GameObject* gameObject);
		void DirectionalLightPass(GameObject* gameObject);
		void PointLightPass(GameObject* gameObject);

		static RenderingEngine* _instance;

		GBuffer* _gbuffer;
		Window* _window;
		int _windowWidth;
		int _windowHeight;

		ERenderingMode _renderingMode = ERenderingMode::FORWARD;
		Camera* _camera;
		MeshRenderer* _quad;
		std::vector<Component*> _renderingComponents;

		Light* _ambientLight;
		std::vector<PointLight*> _pointLights;
		std::vector<DirectionalLight*> _directionalLights;

		std::vector<MeshRenderer*> _meshRenderers;
	};
}
