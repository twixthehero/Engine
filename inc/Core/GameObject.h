#pragma once
#include <vector>
#include <string>

class Component;
class Transform;
class GameObject
{
public:
	GameObject(std::string name);
	~GameObject();

	void Update();

	void AddChild(GameObject* child);
	void AddComponent(Component* component);

	bool IsActive();
	void SetActive(bool active);

	std::string name;
	std::string tag = "Untagged";

	Transform* transform;
private:
	std::vector<GameObject*> _children;
	std::vector<Component*> _components;

	bool _active;
};

