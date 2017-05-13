#ifndef DAEMONIZER_HELPER
#define DAEMONIZER_HELPER

#include <string.h>

#include "../classes/Keyboard.h"

namespace daemonizer {
	
	void start(char *arg0, std::string scenarioFile);
	//void stop();
	//void stop(int signum);
	
	void process(LedKeyboard &kbd, std::string scenarioFile);
	
}

#endif
