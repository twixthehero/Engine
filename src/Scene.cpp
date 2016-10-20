#include "Scene.h"



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

}

void Scene::Render()
{

}
