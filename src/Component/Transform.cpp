#include "Component\Transform.h"
#include <glm\gtc\matrix_transform.hpp>
#include "Core\GameObject.h"

namespace VoxEngine
{
	Transform::Transform()
	{
		_componentType = EComponentType::TRANSFORM;

		_position = glm::vec3(0);
		_rotation = glm::quat();
		_scale = glm::vec3(1);
	}

	Transform::~Transform()
	{
	}

	glm::vec3 Transform::GetForward()
	{
		return glm::normalize(GetTransformedRotation() * glm::vec3(0, 0, -1));
	}

	glm::vec3 Transform::GetRight()
	{
		return glm::normalize(GetTransformedRotation() * glm::vec3(1, 0, 0));
	}

	glm::vec3 Transform::GetUp()
	{
		return glm::normalize(GetTransformedRotation() * glm::vec3(0, 1, 0));
	}

	inline glm::mat4 Transform::GetModelMatrix()
	{
		return _model;
	}

	inline glm::mat4 Transform::GetTransformation()
	{
		return _transformation;
	}

	glm::vec3 Transform::GetTransformedPosition()
	{
		return _parentModel * glm::vec4(_position, 1);
	}

	glm::quat Transform::GetTransformedRotation()
	{
		GameObject* parent = gameObject->GetParent();

		if (parent != nullptr)
			return parent->transform->GetTransformedRotation() * _rotation;

		return _rotation;
	}

	glm::mat4 Transform::GetParentTransformation()
	{
		return _parentModel;
	}

	void Transform::Update()
	{
		//update parent model matrix
		GameObject* parent = gameObject->GetParent();

		//parent is only null on the Scene's root object
		if (parent != nullptr)
		{
			//recalc matrices if dirty
			if (gameObject->IsDirty())
			{
				CalcModelMatrix();
			}

			if (parent->IsDirty())
			{
				_parentModel = parent->transform->GetTransformation();
				CalcTransformation();
			}
		}
	}

	void Transform::Rotate(glm::vec3 axis, float angle, bool isRadians)
	{
		if (!isRadians)
		{
			angle = glm::radians(angle);
		}

		_rotation = glm::normalize(glm::angleAxis(angle, axis) * _rotation);
		CalcModelMatrix();
	}

	void Transform::Translate(glm::vec3 translation)
	{
		_position += translation;
		CalcModelMatrix();
	}

	glm::vec3 Transform::GetPosition()
	{
		return _position;
	}

	glm::quat Transform::GetRotation()
	{
		return _rotation;
	}

	glm::vec3 Transform::GetScale()
	{
		return _scale;
	}

	void Transform::SetPosition(glm::vec3 position)
	{
		_position = position;
		CalcModelMatrix();
	}

	void Transform::SetRotation(glm::quat rotation)
	{
		_rotation = rotation;
		CalcModelMatrix();
	}

	void Transform::SetScale(glm::vec3 scale)
	{
		_scale = scale;
		CalcModelMatrix();
	}

	inline void Transform::CalcModelMatrix()
	{
		_model = glm::translate(glm::mat4(), _position) *
			glm::mat4_cast(_rotation) * glm::scale(glm::mat4(), _scale);
		CalcTransformation();

		if (!gameObject->IsDirty())
		{
			gameObject->MarkDirty();
		}
	}

	inline void Transform::CalcTransformation()
	{
		_transformation = _parentModel * _model;
	}
}
