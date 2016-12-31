#include "Utils.h"
#include "Logger.h"
#include <sstream>
#include <iostream>
#include <GL\gl3w.h>

namespace VoxEngine
{
	std::vector<std::string>& Utils::Split(const std::string& string, char delimiter, std::vector<std::string>& elements)
	{
		std::stringstream ss(string);
		std::string item;

		while (std::getline(ss, item, delimiter))
			elements.push_back(item);

		return elements;
	}

	std::vector<std::string> Utils::Split(const std::string& string, char delimiter)
	{
		std::vector<std::string> elements;
		Split(string, delimiter, elements);

		return elements;
	}

	void Utils::PrintMatrix(const glm::mat4& matrix)
	{
		for (int i = 0; i < 4; i++)
		{
			Logger::Write("[" + std::to_string(matrix[0][i]) + "]");
			Logger::Write("[" + std::to_string(matrix[1][i]) + "]");
			Logger::Write("[" + std::to_string(matrix[2][i]) + "]");
			Logger::WriteLine("[" + std::to_string(matrix[3][i]) + "]");
		}
	}

	bool Utils::CheckGLError(std::string message)
	{
		GLenum error = glGetError();
		bool result = false;

		if (error != GL_NO_ERROR)
		{
			result = true;
			Logger::WriteLine("CheckGLError: " + message);

			do
			{
				switch (error)
				{
					case GL_INVALID_ENUM:
						Logger::WriteLine("INVALID ENUM");
						break;
					case GL_INVALID_VALUE:
						Logger::WriteLine("INVALID VALUE");
						break;
					case GL_INVALID_OPERATION:
						Logger::WriteLine("INVALID OPERATION");
						break;
					case GL_INVALID_FRAMEBUFFER_OPERATION:
						Logger::WriteLine("INVALID FRAMEBUFFER OPERATION");
						break;
					case GL_OUT_OF_MEMORY:
						Logger::WriteLine("OUT OF MEMORY");
						break;
					default:
						Logger::WriteLine("Unknown error code: " + std::to_string(error));
						break;
				}
			}
			while ((error = glGetError()) != GL_NO_ERROR);
		}

		return result;
	}
}
