#ifndef HELP_HELPER
#define HELP_HELPER

#include <iostream>

namespace help {
	
	void usage(char *arg0);
	void keys(char *arg0);
	void effects(char *arg0);
	void samples(char *arg0);
	
}

#ifndef VERSION
#define VERSION "unspecified"
#endif

#endif
