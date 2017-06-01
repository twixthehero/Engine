#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>
#include "Core\Component.h"

namespace VoxEngine
{
	class Transform :
		public Component
	{
	public:
		Transform();
		~Transform();

		glm::vec3 GetForward();
		glm::vec3 GetRight();
		glm::vec3 GetUp();

		glm::mat4 GetModelMatrix();
		glm::mat4 GetTransformation();
		glm::vec3 GetTransformedPosition();
		glm::quat GetTransformedRotation();

		void Update() override;

		void Rotate(glm::vec3 xyz, float angle, bool isRadians = false);
		void Translate(glm::vec3 translation);

		glm::vec3 GetPosition();
		glm::quat GetRotation();
		glm::vec3 GetScale();
		void SetPosition(glm::vec3 position);
		void SetRotation(glm::quat rotation);
		void SetScale(glm::vec3 scale);
	private:
		glm::mat4 GetParentTransformation();

		void CalcModelMatrix();
		void CalcTransformation();

		glm::vec3 _position;
		glm::quat _rotation;
		glm::vec3 _scale;

		glm::mat4 _model;
		glm::mat4 _parentModel;
		glm::mat4 _transformation;
	};
}
