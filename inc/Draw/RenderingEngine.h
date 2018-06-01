#pragma once
#include <vector>
#include <glm/glm.hpp>

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
	class Mesh;
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

		void AddRenderer(MeshRenderer* renderer);
		void RemoveRenderer(MeshRenderer* renderer);
		void AddPointLight(PointLight* light);
		void RemovePointLight(PointLight* light);
		void AddDirectionalLight(DirectionalLight* light);
		void RemoveDirectionalLight(DirectionalLight* light);
		void AddSpotLight(SpotLight* light);
		void RemoveSpotLight(SpotLight* light);

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
		Mesh* _quad;
		Mesh* _sphere;
		Mesh* _cone;
		Mesh* _arrow;
		Mesh* _skyboxMesh = nullptr;
		std::vector<Component*> _renderingComponents;

		Light* _ambientLight;
		Material* _skybox;
		std::vector<PointLight*> _pointLights;
		std::vector<DirectionalLight*> _directionalLights;
		std::vector<SpotLight*> _spotLights;
		float _falloff = 2.5f;

		std::vector<MeshRenderer*> _meshRenderers;
		std::vector<glm::mat4> _modelMatrices;
	};
}
