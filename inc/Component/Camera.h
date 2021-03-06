#pragma once
#include <glm/glm.hpp>
#include "Core/Component.h"

namespace VoxEngine
{
	enum ECameraMode
	{
		ORTHOGRAPHIC, PERSPECTIVE
	};

	class Camera :
		public Component
	{
	public:
		Camera();
		Camera(ECameraMode cameraMode);
		~Camera();

		void Update() override;

		ECameraMode GetCameraMode();
		void SetCameraMode(ECameraMode cameraMode);

		float GetFOV();
		void SetFOV(float fov, bool isRadians = false);
		float GetAspectRatio();
		void SetAspectRatio(float aspectRatio);

		float GetNearClipping();
		void SetNearClipping(float near);
		float GetFarClipping();
		void SetFarClipping(float far);

		glm::mat4 GetProjectionMatrix();
		glm::mat4 GetViewMatrix();
		glm::mat4 GetViewProjectionMatrix();

		static Camera* main;
	private:
		ECameraMode _cameraMode = ECameraMode::PERSPECTIVE;

		//PERSPECTIVE vars
		float _fov = 60;
		float _aspectRatio = 16.0f / 9;

		//ORTHOGRAPHIC vars


		float _nearClipping = 0.1f;
		float _farClipping = 500.0f;
	};
}
