#include "Utils.h"
#include <sstream>
#include <iostream>

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
		std::cout << "[" << matrix[0][i] << "]";
		std::cout << "[" << matrix[1][i] << "]";
		std::cout << "[" << matrix[2][i] << "]";
		std::cout << "[" << matrix[3][i] << "]" << std::endl;
	}
}
