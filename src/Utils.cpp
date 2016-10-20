#include "Utils.h"
#include <sstream>

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
