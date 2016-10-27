#pragma once
#include <string>
#include <vector>
#include <glm\glm.hpp>

class Utils
{
public:
	static std::vector<std::string>& Split(const std::string& string, char delimiter, std::vector<std::string>& elements);
	static std::vector<std::string> Split(const std::string& string, char delimiter);

	static void PrintMatrix(const glm::mat4& matrix);
};

