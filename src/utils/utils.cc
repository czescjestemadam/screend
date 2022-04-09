#include "utils.hh"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sys/syslog.h>
#include <thread>
#include <chrono>

void sleepms(unsigned long ms)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

int getPidFromFile()
{
	int pid = 0;

	std::ifstream ifs(PIDFILE);
	if (ifs.good())
		ifs >> pid;
	ifs.close();

	return pid;
}

void savePidToFile(int pid)
{
	std::ofstream ofs(PIDFILE);
	if (ofs.good())
		ofs << pid;
	ofs.close();
}

bool checkPidRunning(int pid)
{
	return system("pgrep --pidfile " PIDFILE " > /dev/null") == 0;
}
