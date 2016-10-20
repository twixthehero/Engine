#pragma once
#include <string>
#include <vector>

class Utils
{
public:
	static std::vector<std::string>& Split(const std::string& string, char delimiter, std::vector<std::string>& elements);
	static std::vector<std::string> Split(const std::string& string, char delimiter);
};

