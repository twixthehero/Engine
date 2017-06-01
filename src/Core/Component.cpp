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

	void Component::Init()
	{

	}

	void Component::Update()
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
