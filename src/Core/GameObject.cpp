#include "Core/GameObject.h"
#include "Component/Transform.h"

namespace VoxEngine
{
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

		MarkClean();
	}

	void GameObject::AddChild(GameObject* child)
	{
		_children.push_back(child);
	}

	void GameObject::RemoveChild(std::string name)
	{
		for (int i = 0; i < _children.size(); i++)
			if (_children[i]->name == name)
			{
				_children.erase(_children.begin() + i);
				return;
			}
	}

	GameObject* GameObject::GetParent()
	{
		return _parent;
	}

	void GameObject::SetParent(GameObject* parent)
	{
		if (_parent != nullptr)
			_parent->RemoveChild(name);

		if (parent != nullptr)
			parent->AddChild(this);

		_parent = parent;
	}

	void GameObject::AddComponent(Component* component)
	{
		component->gameObject = this;
		_components.push_back(component);

		component->Init();
	}

	Component* GameObject::GetComponent(EComponentType type)
	{
		for (Component* component : _components)
			if (component->GetType() == type)
				return component;

		return nullptr;
	}

	int GameObject::GetComponents(EComponentType type, std::vector<Component*>& components)
	{
		int count = 0;

		for (Component* component : _components)
			if (component->GetType() == type)
			{
				components.push_back(component);
				count++;
			}

		return count;
	}

	Component* GameObject::GetComponentInChildren(EComponentType type)
	{
		Component* component = GetComponent(type);

		if (component == nullptr)
			for (GameObject* child : _children)
			{
				component = child->GetComponent(type);

				if (component != nullptr)
					break;
			}

		return component;
	}

	int GameObject::GetComponentsInChildren(EComponentType type, std::vector<Component*>& components)
	{
		int count = 0;

		count += GetComponents(type, components);

		for (GameObject* child : _children)
			count += child->GetComponentsInChildren(type, components);

		return count;
	}

	int GameObject::GetChildCount()
	{
		return _children.size();
	}

	GameObject* GameObject::Find(std::string name)
	{
		for (GameObject* child : _children)
			if (child->name == name)
				return child;

		return nullptr;
	}

	GameObject* GameObject::GetChild(int index)
	{
		if (index < 0 || index >= _children.size())
			return nullptr;

		return _children[index];
	}

	bool GameObject::IsActive()
	{
		return _active;
	}

	void GameObject::SetActive(bool active)
	{
		_active = active;
	}

	bool GameObject::IsDirty()
	{
		return _dirty;
	}

	void GameObject::MarkDirty()
	{
		_dirty = true;
	}

	void GameObject::MarkClean()
	{
		_dirty = false;
	}
}
