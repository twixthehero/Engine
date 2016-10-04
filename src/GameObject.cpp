#include "GameObject.h"
#include "Transform.h"


GameObject::GameObject()
{
	_components.push_back(new Transform());
}


GameObject::~GameObject()
{
	for (int i = 0; i < _components.size(); i++)
		delete _components[i];
}

void GameObject::AddComponent(Component* component)
{
	_components.push_back(component);
}

template<Component T>
T GameObject::GetComponent()
{

}