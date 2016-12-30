#include "Utils.h"
#include "Logger.h"
#include <sstream>
#include <iostream>

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
}
