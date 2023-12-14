#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "libNexus.h"

const char *TESTGAME_VERSION = "0.0.0.1";
const unsigned int TESTGAME_BUILD = 0x04;

void atexitTestGame (void);

void testGameUsage (void) {
	printf ("No program argument implemented\n");
}

int main (int argc, char **argv) {
	printf ("\n\033[00;33mtestGame version %s build 0x%.2X\033[00m\n", TESTGAME_VERSION, TESTGAME_BUILD);
	
	if (argc == 2) {
		if (strcmp (argv[1], "--help") == 0 
				  || strcmp (argv[1], "-h") == 0) {
			testGameUsage ();
			return 0; 
		}
		else if (strcmp (argv[1], "--version") == 0
							|| strcmp (argv[1], "-V") == 0) {
			return 0;
		}
		else if (strcmp (argv[1], "tester") == 0) {
			int ret = tester ();
			printf ("tester %d\n", ret);
			return ret;
		}
	}
	
	atexit (atexitTestGame);
	NLinit (&argc, argv, "testGame");
	return 0;
}

void atexitTestGame (void) {
	//NLshowState ();
	printf ("testGame exit\n");
}
