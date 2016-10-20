#pragma once
#include "GameObject.h"

class Component
{
public:
	Component();
	virtual ~Component();

	virtual void Update();

	bool IsEnabled();
	void SetEnabled(bool active);

	GameObject* gameObject;
protected:
	bool _enabled = true;
};

