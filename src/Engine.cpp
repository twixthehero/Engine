#include "Engine.h"
#include "Window\Window.h"
#include "Window\WindowManager.h"
#include "Input.h"
#include "Draw\RenderingEngine.h"
#include "TextureManager.h"
#include "Scene.h"
#include "Core\GameObject.h"
#include "Component\Camera.h"
#include "MeshManager.h"
#include <iostream>
#include "Core\Material.h"
#include "Draw\Shader.h"
#include "Component\MeshRenderer.h"
#include "Component\Transform.h"
#include "Time.h"

Engine::Engine()
{
	if (Init() != 0)
	{
		std::cout << "Error initializing engine!" << std::endl;
		return;
	}
	
	Run();

	Shutdown();
}

Engine::~Engine()
{
}

int Engine::Init()
{
	if (!glfwInit())
	{
		return -1;
	}

	WindowManager::Init();
	_windowManager = WindowManager::GetInstance();

	_window = _windowManager->CreateWindow(EWindowMode::WINDOWED, 800, 600, "Engine");

	if (_window == nullptr)
	{
		Shutdown();
		return -1;
	}
	
	_window->MakeContextCurrent();

	Input::Init();
	RenderingEngine::Init();
	TextureManager::Init();
	MeshManager::Init();

	_renderingEngine = RenderingEngine::GetInstance();

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		Shutdown();
		return -1;
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	float data[] =
	{
		-1, -1, -1,
		1, -1, -1,
		0, 1, -1
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);

	_shader = new Shader(2, "rawPosition");

	return 0;
}

void Engine::Run()
{
	_running = true;
	
	_scene = new Scene();
	GameObject* camera = new GameObject("Camera");
	camera->transform->position.x = 5;
	camera->transform->position.z = 5;
	camera->transform->Rotate(glm::vec3(0.0f, 45.0f, 0.0f));
	camera->tag = "MainCamera";
	Camera* cam = new Camera();
	cam->SetFOV(60);
	cam->SetAspectRatio(800.0f / 600);
	cam->SetNearClipping(0.01f);
	cam->SetFarClipping(500.0f);
	camera->AddComponent(cam);
	_scene->AddObject(camera);
	
	Mesh* mesh = MeshManager::GetInstance()->GetMesh("cube");
	Shader* shader = new Shader(1, "default");
	Material* material = new Material(shader, TextureManager::GetInstance()->GetTexture("emma.png"));
	MeshRenderer* meshRenderer = new MeshRenderer(mesh, material);
	GameObject* cube = new GameObject("Cube");
	cube->transform->position.z = 5;
	cube->AddComponent(meshRenderer);
	_scene->AddObject(cube);

	while (_running)
	{
		Update();
		Render();

		_window->SwapBuffers();

		glfwPollEvents();
	}
}

void Engine::Shutdown()
{
	MeshManager::Shutdown();
	TextureManager::Shutdown();
	RenderingEngine::Shutdown();
	Input::Shutdown();
	WindowManager::Shutdown();
}

void Engine::Update()
{
	double time = glfwGetTime();
	
	Time::time = (float)time;
	Time::deltaTime = (float)(time - _lastTime);
	_lastTime = time;

	Input::Update();

	if (_window->ShouldClose() || Input::GetKeyUp(KeyCode::Escape))
		_running = false;

	_scene->Update();
}

void Engine::Render()
{
	_scene->Render(_renderingEngine);

	/*_shader->Bind();
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);*/
}