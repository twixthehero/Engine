#pragma once
#include "Core\Component.h"

class FlyMove :
	public Component
{
public:
	FlyMove();
	~FlyMove();

	void Update() override;
private:
	float _moveSpeed = 5.0f;
};

