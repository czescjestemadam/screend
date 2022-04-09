#include "screen.hh"

#include <cstdio>
#include <cstdlib>
#include <string>
#include <sys/syslog.h>
#include <vector>

bool screenCheck(Config* config)
{
	return system(("screen -ls " + config->getModel().name).c_str()) == 256;
}

void screenOpen(Config *config)
{
	syslog(LOG_NOTICE, "starting screen");

	std::string name = config->getModel().name;
	std::string cmd = config->getModel().command;
	system(("screen -dmS " + name + " " + cmd).c_str());

	std::vector<std::string> users = config->getModel().multiusers;
	if (!users.empty())
	{
		system("screen -S mc -X multiuser on");

		for (int i = 0; i < users.size(); i++)
			system(("screen -S " + name + " -X acladd " + users.at(i)).c_str());
	}
}
