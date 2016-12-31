#pragma once
#include <map>

namespace VoxEngine
{
	class Shader;
	class Texture;
	class Transform;
	class Material
	{
	public:
		Material();
		Material(Shader* shader);
		Material(Shader* shader, Texture* texture);
		~Material();

		void Use(Transform* transform);

		Texture* GetTexture();
	private:
		Shader* _shader;
		std::map<std::string, Texture*> _textures = std::map<std::string, Texture*>();
	};
}
