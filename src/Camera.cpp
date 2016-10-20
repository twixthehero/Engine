#include "Camera.h"
#include "GameObject.h"
#include "Transform.h"
#include "Window\Window.h"
#include "Window\WindowManager.h"
#include <glm\gtc\matrix_transform.hpp>

Camera* Camera::main = nullptr;

Camera::Camera() : Camera(ECameraMode::PERSPECTIVE)
{
}

Camera::Camera(ECameraMode cameraMode)
{
	if (main == nullptr)
	{
		main = this;
	}

	SetCameraMode(cameraMode);
}


Camera::~Camera()
{
}

ECameraMode Camera::GetCameraMode()
{
	return _cameraMode;
}

void Camera::SetCameraMode(ECameraMode cameraMode)
{
	_cameraMode = cameraMode;
}

float Camera::GetFOV()
{
	return _fov;
}

void Camera::SetFOV(float fov)
{
	if (fov <= 0)
		fov = 60;

	_fov = fov;
}

float Camera::GetAspectRatio()
{
	return _aspectRatio;
}

void Camera::SetAspectRatio(float aspectRatio)
{
	if (aspectRatio <= 0)
		aspectRatio = 4.0f / 3;

	_aspectRatio = aspectRatio;
}

float Camera::GetNearClipping()
{
	return _nearClipping;
}

void Camera::SetNearClipping(float near)
{
	if (near <= 0)
		near = 0.1f;

	_nearClipping = near;
}

float Camera::GetFarClipping()
{
	return _farClipping;
}

void Camera::SetFarClipping(float far)
{
	if (far <= 0)
		far = 500.0f;

	_farClipping = far;
}

glm::mat4 Camera::GetProjectionMatrix()
{
	Window* window = WindowManager::GetInstance()->GetMainWindow();

	switch (_cameraMode)
	{
	case ECameraMode::ORTHOGRAPHIC:
		return glm::ortho(0.0f, (float)window->GetWidth(), (float)window->GetHeight(), 0.0f, _nearClipping, _farClipping);
	case ECameraMode::PERSPECTIVE:
		return glm::perspective(_fov, _aspectRatio, _nearClipping, _farClipping);
	default:
		return glm::mat4();
	}
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::translate(glm::mat4(), -gameObject->transform->position);
}
