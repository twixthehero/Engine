#pragma once
#include "Component\Servant.h"

class FlyMove :
	public Servant
{
public:
	FlyMove();
	~FlyMove() override;

	void Update() override;
private:
	float _moveSpeed = 5.0f;
};

