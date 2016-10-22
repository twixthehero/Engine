#pragma once
#include "Core\Component.h"
#include <glm\glm.hpp>

class Light :
	public Component
{
public:
	Light();
	virtual ~Light();

	glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	float intensity = 1;
};

