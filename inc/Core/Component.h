#pragma once
#include "EComponentType.h"

namespace VoxEngine
{
	class GameObject;
	class Component
	{
	public:
		Component();
		virtual ~Component();

		EComponentType GetType();

		virtual void Init();
		virtual void Update();

		bool IsEnabled();
		void SetEnabled(bool active);

		GameObject* gameObject;
	protected:
		EComponentType _componentType = EComponentType::NO_COMPONENT_TYPE;

		bool _enabled = true;
	};
}
