#pragma once

#define PIDFILE ".screend.pid"

void sleepms(unsigned long ms);

int getPidFromFile();
void savePidToFile(int pid);
bool checkPidRunning(int pid);
