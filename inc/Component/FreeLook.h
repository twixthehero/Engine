#pragma once
#include "Component/Servant.h"
#include <glm/glm.hpp>

namespace VoxEngine
{
	class FreeLook :
		public Servant
	{
	public:
		FreeLook();
		~FreeLook() override;

		void Update() override;
	private:
		glm::vec2 _screenCenter;
		glm::vec2 _lastMousePosition;

		float _sensitivity = 10;
	};
}
