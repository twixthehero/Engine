#include "Core\Component.h"



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