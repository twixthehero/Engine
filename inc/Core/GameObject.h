#pragma once
#include <vector>
#include <string>
#include "EComponentType.h"

namespace VoxEngine
{
	class Component;
	class Transform;
	class GameObject
	{
	public:
		GameObject(std::string name);
		~GameObject();

		void Update();

		GameObject* GetParent();
		void SetParent(GameObject* parent);

		void AddComponent(Component* component);
		Component* GetComponent(EComponentType type);
		int GetComponents(EComponentType type, std::vector<Component*>& components);
		Component* GetComponentInChildren(EComponentType type);
		int GetComponentsInChildren(EComponentType type, std::vector<Component*>& components);

		int GetChildCount();
		GameObject* Find(std::string name);
		GameObject* GetChild(int index);

		bool IsActive();
		void SetActive(bool active);

		bool IsDirty();
		void MarkDirty();
		void MarkClean();

		std::string name = "Default Name";
		std::string tag = "Untagged";

		Transform* transform;
	private:
		void AddChild(GameObject* child);
		void RemoveChild(std::string name);

		GameObject* _parent;

		std::vector<GameObject*> _children;
		std::vector<Component*> _components;

		bool _active;
		bool _dirty = false;
	};
}
