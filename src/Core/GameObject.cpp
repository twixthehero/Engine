#include "Core\GameObject.h"
#include "Component\Transform.h"

GameObject::GameObject(std::string name)
{
	_children = std::vector<GameObject*>();
	_components = std::vector<Component*>();

	transform = new Transform();
	AddComponent(transform);

	this->name = name;
	_active = true;
}

GameObject::~GameObject()
{
	for (int i = 0; i < _children.size(); i++)
		delete _children[i];

	for (int i = 0; i < _components.size(); i++)
		delete _components[i];
}

void GameObject::Update()
{
	if (!_active) return;

	for (int i = 0; i < _components.size(); i++)
		if (_components[i]->IsEnabled())
			_components[i]->Update();

	for (int i = 0; i < _children.size(); i++)
		_children[i]->Update();
}

void GameObject::AddChild(GameObject* child)
{
	child->transform->parent = transform;
	_children.push_back(child);
}

void GameObject::AddComponent(Component* component)
{
	component->gameObject = this;
	_components.push_back(component);
}

bool GameObject::IsActive()
{
	return _active;
}

void GameObject::SetActive(bool active)
{
	_active = active;
}
