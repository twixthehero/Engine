#pragma once
#include "Component\Servant.h"

namespace VoxEngine
{
	class Rotate :
		public Servant
	{
	public:
		Rotate(float rotateSpeed);
		~Rotate();

		void Update() override;

		float rotateSpeed;
	};
}