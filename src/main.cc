#include "config/config.hh"
#include "daemon/daemon.hh"
#include "utils/utils.hh"
#include <csignal>
#include <cstdio>

void printHelp();
void printConfig();
void daemon(std::string file);
void reload();
void stop();

int main(int argc, char** argv)
{
	bool showhelp = argc < 2;
	bool showconfig = false;
	bool freload = false;
	bool fstop = false;
	bool fdaemon = false;
	std::string config = ".screend.json";

	for (int i = 1; i < argc; i++)
	{
		char* arg = argv[i];

		if (!strcmp(arg, "-h") || !strcmp(arg, "--help"))
			showhelp = true;
		else if (!strcmp(arg, "-c") || !strcmp(arg, "--config"))
			showconfig = true;
		else if (!strcmp(arg, "-r") || !strcmp(arg, "--reload"))
			freload = true;
		else if (!strcmp(arg, "-s") || !strcmp(arg, "--stop"))
			fstop = true;
		else if (!strcmp(arg, "-d") || !strcmp(arg, "--daemon"))
			fdaemon = true;
		else
			config = arg;
	}

	if (showhelp)
		printHelp();
	else if (showconfig)
		printConfig();
	else if (fdaemon)
		daemon(config);
	else if (freload)
		reload();
	else if (fstop)
		stop();
	else
		printHelp();

	return 0;
}

void printHelp()
{
	auto opt = [](const char* flag, const char* longflag, const char* description)
	{
		printf("  %s  %s\t%s\n", flag, longflag, description);
	};

	printf("Usage: screend -d [config]\n");
	printf("Usage: screend [options]\n\n");
	printf("options:\n");
	opt("-h", "--help", "show help");
	opt("-c", "--config", "show example config");
	opt("-d", "--daemon", "start daemon");
	opt("-r", "--reload", "reload config");
	opt("-s", "--stop", "stop daemon");
}

void printConfig()
{
	Config config;
	config.print();
}

void daemon(std::string file)
{
	int pid = getPidFromFile();
	if (pid)
	{
		if (checkPidRunning(pid))
		{
			printf("daemon already running in this directory\n");
			return;
		}
		else
			remove(PIDFILE);
	}

	Config config;
	
	try
	{
		config = Config(file);
	} catch (nlohmann::detail::parse_error& e)
	{
		printf("error parsing config '%s': %s\n", file.c_str(), e.what());
		return;
	} catch (std::exception& e)
	{
		printf("error loading config '%s'\n", file.c_str());
		return;
	}

	createDaemon(&config);
}

void sigAction(int signal, const char* message)
{
	int pid = getPidFromFile();
	if (pid)
	{
		if (checkPidRunning(pid))
		{
			kill(pid, signal);
			printf(message, pid);
		}
		else
		{
			printf("process %i not found\n", pid);
			remove(PIDFILE);
		}
	}
	else
		printf("no daemons are running in this directory\n");
}

void reload()
{
	sigAction(SIGHUP, "sent reload signal to pid %i\n");
}

void stop()
{
	sigAction(SIGTERM, "sent stop signal to pid %i\n");
}
