#include "Core\Component.h"

namespace VoxEngine
{
	Component::Component()
	{
	}

	Component::~Component()
	{
	}

	EComponentType Component::GetType()
	{
		return _componentType;
	}

	void Component::Update()
	{

	}

	void Component::Render()
	{

	}

	bool Component::IsEnabled()
	{
		return _enabled;
	}

	void Component::SetEnabled(bool enabled)
	{
		_enabled = enabled;
	}
}
