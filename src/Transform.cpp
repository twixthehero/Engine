#include "Transform.h"
#include <glm\gtc\matrix_transform.hpp>


Transform::Transform()
{
	position = glm::vec3(0);
	rotation = glm::quat();
	scale = glm::vec3(1);
}


Transform::~Transform()
{
}

glm::vec3 Transform::GetForward()
{
	return glm::vec3(0, 0, 1) * rotation;
}

glm::vec3 Transform::GetRight()
{
	return glm::vec3(1, 0, 0) * rotation;
}

glm::vec3 Transform::GetUp()
{
	return glm::vec3(0, 1, 0) * rotation;
}

glm::mat4 Transform::GetModelMatrix()
{
	glm::mat4 transMatrix = glm::translate(glm::mat4(), position);
	glm::mat4 rotMatrix = glm::mat4_cast(rotation);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(), scale);

	return transMatrix * rotMatrix * scaleMatrix;
}