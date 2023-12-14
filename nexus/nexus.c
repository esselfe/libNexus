/* nexus.c */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <sensors/sensors.h>
#include <GL/glut.h>
//#include <X11/Xlib.h>

#include "browser.h"
#include "buffer.h"
#include "camera.h"
#include "commands.h"
#include "config.h"
#include "osdata.h"
#include "displayList.h"
#include "drawing.h"
#include "events.h"
#include "getargs.h"
#include "hud.h"
#include "Ninit.h"
#include "memfiller.h"
#include "memoryViewer.h"
#include "menu.h"
#include "messages.h"
#include "libNexus.h"
#include "nexus.h"
#include "Nsignals.h"
#include "objects.h"
#include "restore.h"
#include "selectID.h"
#include "state.h"
#include "tester.h"
#include "threading.h"
#include "util.h"

const char *NEXUS_VERSION = "0.0.4.4a-007";
/*
int winWidth = 800, winHeight = 600;
int program_nice = 0;
int program_pid;
int program_ppid;
int program_argc;
time_t program_start;
char **program_argv;
char *program_name;
char *program_pwd;
int exitProgram = 0;
*/
//Display *display;

void nexusUsage (void) {
	printf ("Usage: %s \n \
			\t-b: Start in file browser mode\n \
			\t-d | --debug CONFIGKEYWORD (see nexus.cfg) \n \
			\t-e: Start in mesh editor mode\n  \
			\t-f tester: Run Nexus for a special function.\n \
			\t-h | --help: Shows this help \n \
			\t-m { browser | editor | memory | codeParse }: Specifies mode\n \
			\t-v n: Verbose level between 0 and 4\n \
			\t-V | --version: Shows program version", program_name);
}

int main (int argc, char **argv) {
	printf ("\n\033[00;32mNexus v%s started\033[00m\n", NEXUS_VERSION);
	
	atexit (atExitNexus);
	
	if (argc > 1) {
		if (strcmp (argv[1], "--help") == 0 
				  || strcmp (argv[1], "-h") == 0) {
			nexusUsage ();
			return 0;
				  }
				  else if (strcmp (argv[1], "--version") == 0
									   || strcmp (argv[1], "-V") == 0) {
					  return 0;
									   }
									   else if (argc == 2 && strcmp (argv[1], "tester") == 0) {
										   int ret = tester ();
										   printf ("tester %d\n", ret);
										   exitNexus (STATE_TESTER, STATE_OK, __FUNCTION__);
									   }
	}
	
	NLinit (&argc, argv, "Nexus");
	
	return 0;
}

