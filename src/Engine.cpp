#include "Engine.h"
#include "Logger.h"
#include "Window\Window.h"
#include "Window\WindowManager.h"
#include "Input.h"
#include "Draw\RenderingEngine.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "Scene.h"
#include "Core\GameObject.h"
#include "Component\Camera.h"
#include "MeshManager.h"
#include <iostream>
#include "Core\Material.h"
#include "Component\MeshRenderer.h"
#include "Component\Transform.h"
#include "Component\FlyMove.h"
#include "EngineTime.h"
#include "Component\FreeLook.h"
#include "Component\Oscillate.h"
#include "Component\PointLight.h"
#include "Component\DirectionalLight.h"

namespace VoxEngine
{
	Engine::Engine()
	{
		if (Init() != 0)
		{
			Logger::WriteLine("Error initializing engine!");
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
		Logger::Init();

		if (!glfwInit())
		{
			return -1;
		}

		WindowManager::Init();
		_windowManager = WindowManager::GetInstance();

		_window = _windowManager->CreateNewWindow(EWindowMode::WINDOWED, 1280, 720, "Engine");

		if (_window == nullptr)
		{
			Logger::WriteLine("Failed to create window");

			Shutdown();
			return -1;
		}

		_window->MakeContextCurrent();

		if (gl3wInit())
		{
			Logger::WriteLine("Failed to initialize gl3w");

			Shutdown();
			return -1;
		}

		Logger::WriteLine("OpenGL Version: " + std::string((const char*)glGetString(GL_VERSION)));
		Logger::WriteLine("GLSL Version: " + std::string((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION)));

		//setup debug callback
		glDebugMessageCallbackARB(&Engine::DebugCallback, NULL);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);

		Input::Init();
		ShaderManager::Init();
		MeshManager::Init();
		RenderingEngine::Init();
		TextureManager::Init();

		_renderingEngine = RenderingEngine::GetInstance();

		return 0;
	}

	void Engine::Run()
	{
		_running = true;

		_scene = new Scene();
		GameObject* cameraObject = new GameObject("Camera");
		cameraObject->transform->position.z = 10;
		cameraObject->name = "MainCamera";
		cameraObject->tag = "MainCamera";
		Camera* camera = new Camera();
		camera->SetFOV(60);
		camera->SetAspectRatio((float)_window->GetWidth() / _window->GetHeight());
		camera->SetNearClipping(0.01f);
		camera->SetFarClipping(500.0f);
		cameraObject->AddComponent(camera);
		FlyMove* flyMove = new FlyMove();
		cameraObject->AddComponent(flyMove);
		FreeLook* freeLook = new FreeLook();
		cameraObject->AddComponent(freeLook);
		_scene->AddObject(cameraObject);

		Mesh* mesh_cube = MeshManager::GetInstance()->GetMesh("cube");

		GameObject* emmaCube = new GameObject("EmmaCube");
		Material* mat_emma = new Material(TextureManager::GetInstance()->GetTexture("emma.png"));
		MeshRenderer* meshRenderer = new MeshRenderer(mesh_cube, mat_emma);
		emmaCube->AddComponent(meshRenderer);
		_scene->AddObject(emmaCube);

		GameObject* danielCube = new GameObject("DanielCube");
		Material* mat_daniel = new Material(TextureManager::GetInstance()->GetTexture("daniel.png"));
		MeshRenderer* meshRenderer2 = new MeshRenderer(mesh_cube, mat_daniel);
		danielCube->AddComponent(meshRenderer2);
		Oscillate* oscillate = new Oscillate(Oscillate::EAxis::Y, 3);
		danielCube->AddComponent(oscillate);
		_scene->AddObject(danielCube);
		
		GameObject* pointLightObject = new GameObject("PointLight");
		pointLightObject->transform->position.x = -5;
		pointLightObject->transform->position.y = 5;
		pointLightObject->transform->position.z = 5;
		PointLight* pointLight = new PointLight();
		pointLightObject->AddComponent(pointLight);
		_scene->AddObject(pointLightObject);
		
		GameObject* directionalLightOrigin = new GameObject("DirectionalLight");
		directionalLightOrigin->transform->position.x = -3;
		directionalLightOrigin->transform->position.y = 3;
		directionalLightOrigin->transform->position.z = 3;
		directionalLightOrigin->transform->Rotate(glm::vec3(0, 1, 0), -45);
		directionalLightOrigin->transform->Rotate(glm::vec3(1, 0, 0), -45);
		Mesh* mesh_arrow = MeshManager::GetInstance()->GetMesh("arrow");
		Material* mat_missing = new Material(TextureManager::GetInstance()->GetTexture("missing.png"));
		MeshRenderer* meshRenderer3 = new MeshRenderer(mesh_arrow, mat_missing);
		directionalLightOrigin->AddComponent(meshRenderer3);
		DirectionalLight* directionalLight = new DirectionalLight();
		directionalLightOrigin->AddComponent(directionalLight);
		//_scene->AddObject(directionalLightOrigin);

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
		TextureManager::Shutdown();
		RenderingEngine::Shutdown();
		MeshManager::Shutdown();
		ShaderManager::Shutdown();
		Input::Shutdown();
		WindowManager::Shutdown();
		Logger::Shutdown();
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
	}

	void Engine::DebugCallback(unsigned int source, unsigned int type,
		unsigned int id, unsigned int severity,
		int length, const char* message, const void* userParam)
	{
		Logger::WriteLine("=========== glDebugCallback ===========");

		Logger::Write("Source: ");

		switch (source)
		{
			case GL_DEBUG_SOURCE_API_ARB:
				Logger::WriteLine("OpenGL");
				break;
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB:
				Logger::WriteLine("Windows");
				break;
			case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB:
				Logger::WriteLine("Shader Compiler");
				break;
			case GL_DEBUG_SOURCE_THIRD_PARTY_ARB:
				Logger::WriteLine("Third Party");
				break;
			case GL_DEBUG_SOURCE_APPLICATION_ARB:
				Logger::WriteLine("Application");
				break;
			case GL_DEBUG_SOURCE_OTHER_ARB:
				Logger::WriteLine("Other");
				break;
			default:
				Logger::WriteLine("Unknown (" + std::to_string(source) + ")");
				break;
		}

		Logger::Write("Type: ");

		switch (type)
		{
			case GL_DEBUG_TYPE_ERROR_ARB:
				Logger::WriteLine("Error");
				break;
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB:
				Logger::WriteLine("Deprecated behavior");
				break;
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:
				Logger::WriteLine("Undefined behavior");
				break;
			case GL_DEBUG_TYPE_PORTABILITY_ARB:
				Logger::WriteLine("Portability");
				break;
			case GL_DEBUG_TYPE_PERFORMANCE_ARB:
				Logger::WriteLine("Performance");
				break;
			case GL_DEBUG_TYPE_OTHER_ARB:
				Logger::WriteLine("Other");
				break;
			default:
				Logger::WriteLine("Unknown (" + std::to_string(type) + ")");
				break;
		}

		Logger::Write("Severity: ");

		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH_ARB:
				Logger::WriteLine("High");
				break;
			case GL_DEBUG_SEVERITY_MEDIUM_ARB:
				Logger::WriteLine("Medium");
				break;
			case GL_DEBUG_SEVERITY_LOW_ARB:
				Logger::WriteLine("Low");
				break;
			default:
				Logger::WriteLine("Unknown (" + std::to_string(severity) + ")");
				break;
		}

		Logger::WriteLine("Message: " + std::string(message));

		Logger::WriteLine("================= End =================");
	}
}
