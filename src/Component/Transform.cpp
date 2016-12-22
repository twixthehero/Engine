#include "Component\Transform.h"
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
	return rotation * glm::vec3(0, 0, -1);
}

glm::vec3 Transform::GetRight()
{
	return rotation * glm::vec3(1, 0, 0);
}

glm::vec3 Transform::GetUp()
{
	return rotation * glm::vec3(0, 1, 0);
}

glm::mat4 Transform::GetModelMatrix()
{
	glm::mat4 transMatrix = glm::translate(glm::mat4(), position);
	glm::mat4 rotMatrix = glm::mat4_cast(rotation);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(), scale);

	return transMatrix * rotMatrix * scaleMatrix;
}

void Transform::Rotate(glm::vec3 axis, float angle, bool isRadians)
{
	if (!isRadians)
	{
		angle = glm::radians(angle);
	}

	rotation = glm::normalize(glm::angleAxis(angle, axis) * rotation);
}

void Transform::Translate(glm::vec3 translation)
{
	position += translation;
}