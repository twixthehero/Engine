#pragma once
#include "Component\Servant.h"
#include <glm\glm.hpp>

namespace VoxEngine
{
	class Oscillate :
		public Servant
	{
	public:
		enum EAxis { NONE, X, Y, Z };

		Oscillate();
		Oscillate(EAxis axis, float distance);
		~Oscillate();

		void Init() override;
		void Update() override;

		EAxis axis = EAxis::NONE;
		float distance;
		glm::vec3 centerPosition;
	};
}
