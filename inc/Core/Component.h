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

		virtual void Update();
		virtual void Render();

		bool IsEnabled();
		void SetEnabled(bool active);

		GameObject* gameObject;
	protected:
		EComponentType _componentType;

		bool _enabled = true;
	};
}
