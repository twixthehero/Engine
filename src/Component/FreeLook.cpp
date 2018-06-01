#include "Component/FreeLook.h"
#include "Input.h"
#include "EngineTime.h"
#include "Core/GameObject.h"
#include "Component/Transform.h"
#include "Window/WindowManager.h"
#include "Window/Window.h"

namespace VoxEngine
{
	FreeLook::FreeLook()
	{
		Window* window = WindowManager::GetInstance()->GetMainWindow();
		_screenCenter = glm::vec2(window->GetWidth() / 2, window->GetHeight() / 2);
	}

	FreeLook::~FreeLook()
	{
	}

	void FreeLook::Update()
	{
		if (Input::GetMouse(ButtonCode::Right))
		{
			glm::vec2 delta = Input::GetMousePosition() - _screenCenter;

			bool rotX = delta.x != 0;
			bool rotY = delta.y != 0;

			if (rotX)
				gameObject->transform->Rotate(glm::vec3(0, 1, 0), -delta.x * _sensitivity * Time::deltaTime, false);
			if (rotY)
				gameObject->transform->Rotate(gameObject->transform->GetRight(), -delta.y * _sensitivity * Time::deltaTime, false);

			if (rotX || rotY)
				Input::SetMousePosition(_screenCenter.x, _screenCenter.y);
		}
	}
}
