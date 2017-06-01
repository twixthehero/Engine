#pragma once
#include <map>
#include <GL\gl3w.h>

namespace VoxEngine
{
	class Texture;
	class Material
	{
	public:
		Material();
		virtual ~Material();

		virtual void Use() = 0;

		GLuint GetTexture(std::string name);

		bool operator<(const Material& otherMaterial) const
		{
			return _hash < otherMaterial._hash;
		}

		bool operator==(const Material& otherMaterial) const
		{
			return _hash == otherMaterial._hash;
		}

		bool operator!=(const Material& otherMaterial) const
		{
			return _hash != otherMaterial._hash;
		}
	protected:
		void Finalize();

		std::map<std::string, Texture*> _textures = std::map<std::string, Texture*>();
		std::size_t _hash;
	};
}
