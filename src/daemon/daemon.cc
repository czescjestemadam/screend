#include "daemon.hh"

#include "../utils/utils.hh"
#include "../screen/screen.hh"

#include <csignal>
#include <sys/syslog.h>

pid_t pid;
Config* cfg;
bool isRunning = true;

void start();
void run();
void reload(int signum);
void stop(int signum);

void createDaemon(Config *config)
{
	cfg = config;
	start();
}

void start()
{
	pid = fork();

	if (pid < 0)
		exit(1);
	if (pid > 0)
		exit(0);

	if (setsid() < 0)
		exit(1);

	signal(SIGHUP, reload);
	signal(SIGTERM, stop);

	pid = fork();

	if (pid < 0)
		exit(1);
	if (pid > 0)
		exit(0);

	savePidToFile(getpid());

	for (int i = sysconf(_SC_OPEN_MAX); i >= 0; i--)
		close(i);

	openlog(std::string("screend[" + cfg->getModel().name + ']').c_str(), LOG_PID, LOG_DAEMON);
	syslog(LOG_NOTICE, "started daemon");

	run();

	syslog(LOG_NOTICE, "stopping daemon");
	closelog();

	remove(PIDFILE);
}

void run()
{
	while (isRunning)
	{
		if (screenCheck(cfg))
			screenOpen(cfg);

		sleepms(cfg->getModel().interval);
	}
}

void reload(int signum)
{
	syslog(LOG_NOTICE, "reloading config...");
	cfg->load();
	syslog(LOG_NOTICE, "reloaded config");
}

void stop(int signum)
{
	isRunning = false;
}
