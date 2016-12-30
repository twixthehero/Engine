#pragma once
#include <string>

namespace VoxEngine
{
	class GameObject;
	class RenderingEngine;
	class Scene
	{
	public:
		Scene();
		~Scene();

		void AddObject(GameObject* object);
		void RemoveObject(GameObject* object);

		GameObject* Find(std::string name);

		void Update();
		void Render(RenderingEngine* renderingEngine);
	private:
		GameObject* root;
	};
}
