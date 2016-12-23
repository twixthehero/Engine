#include "Component\FlyMove.h"
#include "Input.h"
#include "Core\GameObject.h"
#include "Component\Transform.h"
#include "EngineTime.h"

FlyMove::FlyMove()
{
}


FlyMove::~FlyMove()
{
}

void FlyMove::Update()
{
	float speed = Input::GetKey(KeyCode::LeftShift) ? _moveSpeed * 2 : _moveSpeed;

	if (Input::GetKey(KeyCode::W))
	{
		gameObject->transform->Translate(gameObject->transform->GetForward() * speed * Time::deltaTime);
	}
	if (Input::GetKey(KeyCode::S))
	{
		gameObject->transform->Translate(-gameObject->transform->GetForward() * speed * Time::deltaTime);
	}
	if (Input::GetKey(KeyCode::A))
	{
		gameObject->transform->Translate(-gameObject->transform->GetRight() * speed * Time::deltaTime);
	}
	if (Input::GetKey(KeyCode::D))
	{
		gameObject->transform->Translate(gameObject->transform->GetRight() * speed * Time::deltaTime);
	}
	if (Input::GetKey(KeyCode::Q))
	{
		gameObject->transform->Translate(gameObject->transform->GetUp() * speed * Time::deltaTime);
	}
	if (Input::GetKey(KeyCode::E))
	{
		gameObject->transform->Translate(-gameObject->transform->GetUp() * speed * Time::deltaTime);
	}
}