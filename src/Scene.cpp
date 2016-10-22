#include "Scene.h"
#include "Core\GameObject.h"
#include "Draw\RenderingEngine.h"


Scene::Scene()
{
	root = new GameObject("Root");
}


Scene::~Scene()
{
	delete root;
}

void Scene::AddObject(GameObject* object)
{
	root->AddChild(object);
}

void Scene::RemoveObject(GameObject* object)
{

}

GameObject* Scene::Find(std::string name)
{
	return nullptr;
}

void Scene::Update()
{
	root->Update();
}

void Scene::Render(RenderingEngine* renderingEngine)
{
	renderingEngine->Render(root);
}
