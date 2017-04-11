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

		void Rotate(glm::vec3 xyz, float angle, bool isRadians = false);
		void Translate(glm::vec3 translation);

		glm::vec3 position;
		glm::quat rotation;
		glm::vec3 scale;
	private:
		glm::mat4 GetParentTransformation();
	};
}
