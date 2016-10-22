#include "Draw\RenderingEngine.h"
#include <GLFW\glfw3.h>
#include "Component\Light.h"
#include "Core\GameObject.h"

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

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_BLEND);	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_CULL_FACE);	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST); glDepthFunc(GL_EQUAL);
	glEnable(GL_TEXTURE_2D);
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

	_currentLight = _ambientLight;

	gameObject->Render(this);

	for (Light* light : _lights)
	{
		_currentLight = light;
		gameObject->Render(this);
	}
}

Light* RenderingEngine::GetLight()
{
	return _currentLight;
}