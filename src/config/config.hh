#pragma once
#include "../lib/json.hpp"
#include "config_model.hh"

using nlohmann::json;

class Config
{
	std::string file;
	json j;
	ConfigModel model;

public:

	Config();
	Config(std::string file);

	void load();

	ConfigModel getModel();

	void print();
};
