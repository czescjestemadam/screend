#pragma once
#include <string>
#include <vector>

struct ConfigModel
{
	std::string name;
	std::string command;
	std::vector<std::string> multiusers;
	long interval;
};
