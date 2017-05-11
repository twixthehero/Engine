#pragma once
#include <vector>

namespace VoxEngine
{
	class GBuffer;
	class GameObject;
	class Light;
	class PointLight;
	class DirectionalLight;
	class SpotLight;
	class Camera;
	class Component;
	class MeshRenderer;
	class Window;
	class Material;
	class RenderingEngine
	{
	public:
		enum ERenderingMode { NONE, FORWARD, DEFERRED };

		static void Init();
		static RenderingEngine* GetInstance();
		static void Shutdown();

		bool GetShowLightingDebug();
		void SetShowLightingDebug(bool show);

		ERenderingMode GetRenderingMode();
		void SetRenderingMode(ERenderingMode mode);
		void Render(GameObject* gameObject);

		void SetCamera(Camera* camera);
		Camera* GetCamera();
		Light* GetAmbientLight();
		void SetSkybox(Material* skybox);
	private:
		RenderingEngine();
		~RenderingEngine();

		void Forward(GameObject* gameObject);
		void Deferred(GameObject* gameObject);

		void GeometryPass(GameObject* gameObject);
		void StencilPassPoint(PointLight* pointLight);
		void PointLightPass(PointLight* pointLight);
		void StencilPassSpot(SpotLight* spotLight);
		void SpotLightPass(SpotLight* spotLight);
		void DirectionalLightPass(GameObject* gameObject);
		void FinalPass();

		void UpdateComponents(GameObject* gameObject);

		float CalculateSpotLightScale(SpotLight* light);

		void ShowLightingDebug();
		void RenderSkybox();

		static RenderingEngine* _instance;

		GBuffer* _gbuffer;
		Window* _window;
		int _windowWidth;
		int _windowHeight;
		bool _showLightingDebug = false;

		ERenderingMode _renderingMode = ERenderingMode::DEFERRED;
		Camera* _camera;
		MeshRenderer* _quad;
		MeshRenderer* _sphere;
		MeshRenderer* _cone;
		MeshRenderer* _arrow;
		MeshRenderer* _skyboxMesh = nullptr;
		std::vector<Component*> _renderingComponents;

		Light* _ambientLight;
		Material* _skybox;
		std::vector<PointLight*> _pointLights;
		std::vector<DirectionalLight*> _directionalLights;
		std::vector<SpotLight*> _spotLights;

		std::vector<MeshRenderer*> _meshRenderers;
	};
}
