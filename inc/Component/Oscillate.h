#pragma once
#include "Component\Servant.h"

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

		void Update() override;

		EAxis axis = EAxis::NONE;
		float distance;
	};
}
