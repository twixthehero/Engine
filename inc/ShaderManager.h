#pragma once
#include <map>
#include <string>

namespace VoxEngine
{
	class Shader;
	class ShaderManager
	{
	public:
		static void Init();
		static ShaderManager* GetInstance();
		static void Shutdown();

		void AddShader(std::string name, Shader* shader);
		void UnloadShader(std::string name);
		
		Shader* GetShader(std::string name);
		void UseShader(std::string name);
	private:
		ShaderManager();
		~ShaderManager();

		static ShaderManager* _instance;

		unsigned int _nextShaderIndex = 0;
		std::map<std::string, Shader*> _shaders;
	};
}
