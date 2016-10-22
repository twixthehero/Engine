#pragma once

class RenderingEngine;
class GameObject;
class Component
{
public:
	Component();
	virtual ~Component();

	virtual void Update();
	virtual void Render(RenderingEngine* renderingEngine);

	bool IsEnabled();
	void SetEnabled(bool active);

	GameObject* gameObject;
protected:
	bool _enabled = true;
};

