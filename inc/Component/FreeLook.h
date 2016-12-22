#pragma once
#include "Core\Component.h"
#include <glm\glm.hpp>

class FreeLook :
	public Component
{
public:
	FreeLook();
	~FreeLook();

	void Update() override;
private:
	glm::vec2 _screenCenter;
	glm::vec2 _lastMousePosition;

	float _sensitivity = 90;
};

