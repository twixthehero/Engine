#pragma once

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

	bool _enabled = true;
};

