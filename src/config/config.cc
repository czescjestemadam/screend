#include "config.hh"

#include <exception>
#include <fstream>
#include <iostream>
#include <iomanip>

Config::Config()
{
	model.name = "top";
	model.command = "top";
	model.multiusers.push_back("wheel");
	model.interval = 2500L;

	j["name"] = model.name;
	j["command"] = model.command;
	j["multiusers"] = model.multiusers;
	j["interval"] = model.interval;
}

Config::Config(std::string file)
{
	this->file = file;
	load();
}

void Config::load()
{
	std::ifstream ifs(file);
	if (!ifs.good())
	{
		ifs.close();
		throw std::exception();
	}

	ifs >> j;
	ifs.close();

	model = {
		j["name"].get<std::string>(),
		j["command"].get<std::string>(),
		j["multiusers"].get<std::vector<std::string>>(),
		j["interval"].get<long>()
	};
}

ConfigModel Config::getModel()
{
	return model;
}

void Config::print()
{
	std::cout << std::setw(4) << j << std::endl;
}
