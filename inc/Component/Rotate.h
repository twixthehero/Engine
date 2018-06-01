#pragma once
#include "Component/Servant.h"

namespace VoxEngine
{
	class Rotate :
		public Servant
	{
	public:
		enum EAxis { NONE, X, Y, Z };

		Rotate(EAxis axis, float rotateSpeed);
		~Rotate();

		void Update() override;

		EAxis axis = EAxis::NONE;
		float rotateSpeed;
	};
}