#include "Draw\RenderingEngine.h"
#include <glew\glew.h>
#include "Component\Light.h"
#include "Core\GameObject.h"
#include "Component\Camera.h"
#include <iostream>

RenderingEngine* RenderingEngine::_instance = nullptr;

RenderingEngine::RenderingEngine()
{
	_lights = std::vector<Light*>();

	_ambientLight = new Light();
	_ambientLight->color.r = 0.0f;
	_ambientLight->color.g = 0.0f;
	_ambientLight->color.b = 0.25f;
	_ambientLight->color.a = 1.0f;
}


RenderingEngine::~RenderingEngine()
{
}

void RenderingEngine::Init()
{
	_instance = new RenderingEngine();

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_BLEND);	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_CULL_FACE);	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST); glDepthFunc(GL_LEQUAL);
}

RenderingEngine* RenderingEngine::GetInstance()
{
	return _instance;
}

void RenderingEngine::Shutdown()
{
	if (_instance != nullptr)
	{
		delete _instance;
		_instance = nullptr;
	}
}

void RenderingEngine::Render(GameObject* gameObject)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SetCamera(Camera::main);
	_currentLight = _ambientLight;

	gameObject->Render(this);

	for (Light* light : _lights)
	{
		_currentLight = light;
		gameObject->Render(this);
	}
}

void RenderingEngine::SetCamera(Camera* camera)
{
	_camera = camera;
}

Camera* RenderingEngine::GetCamera()
{
	return _camera;
}

Light* RenderingEngine::GetLight()
{
	return _currentLight;
}