

// TODO: write setDefaultConfig()

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>

#include "config.h"
#include "getargs.h"
#include "menu.h"
#include "libNexus.h"
#include "osdata.h"
#include "state.h"
#include "threading.h"

char *configFileName = "/root/.nexus/nexus.cfg";

int bellOrigVol = 100;
int bellOrigPitch = 120;
int bellVol = 100;
int bellPitch = 60;
char *xsetcmd;	// malloc'd in initConfig()
const int XSETCMDSIZE = 256;

/* Default configuration */
Options options = { 0, 	// autoclear messageboard
						0,		// cutMessages
						1,		// autoHideMenu
						1,		// autoSaveState	(closing state)
						100,	// bellVol
						60,		// bellPitch
						100,	// bellDuration
						0,		// niceLevel
						0,		// showClock
						0,		// showFPS
						1,		// showTermFPS
						0,		// showFloor
						1,		// showFloorLines
						1,		// showUsb
						0,		// showSensors
						1000,	// updateDelay
						0,		// useAlpha
						0,		// Debug
						0		// verbose level
};

Debug debug = { 0,	// showids
	0,	// show camera position
	0,	// showPoint
	1,	// showMeshLines
	0,	// debugBrowser
	0,	// debugBuffer
	0,	// debugDisplayList
	0,	// debugDrawing
	0,	// debugEvents
	0,	// debugMesh
	0	// debugPrompt
};

void NLinitConfig (void) {
	NLsetState (STATE_CONFIG, STATE_OK);
	xsetcmd = malloc (XSETCMDSIZE);
	NLloadConfig ();
}

void NLloadConfig (void) {
	FILE *file = fopen (configFileName, "r");
	if (file == NULL) {
		printf ("NLloadConfig(): Cannot open configuration file: %s\n", configFileName);
		perror ("perror");
		NLsetState (STATE_CONFIG, STATE_ERROR);
		exit (0);
	}
	
	NLsetState (STATE_CONFIG, STATE_OK);
	
	//struct Entry *entry = NULL;
	char c;
	char w[64]; // first word on the line
	char w2[64];
	memset (w, '!', 64);
	memset (w2, '!', 64);
	int cnt = 0;
	while (1) {
		c = fgetc (file);
		if (c == ' ' || c == '\n') continue;
		else if (c == '#') {
			while (1) {
				c = fgetc (file);
				if (c == '\n') break;
			}
			cnt = 0;
			continue;
		}
		else if (c == '=') {
			w[cnt] = '\0';
			cnt = 0;
			while (1) {	// Get the value
				c = fgetc (file);
				if (c == ' ') continue;
				else if (c == '\n') {
					w2[cnt] = '\0';
					if (strcmp (w, "autoclear") == 0)
						options.autoclear = atoi (w2);
					else if (strcmp (w, "cutMessages") == 0)
						options.cutMessages = atoi (w2);
					else if (strcmp (w, "autoSaveState") == 0)
						options.autoSaveState = atoi (w2);
					else if (strcmp (w, "autoHideMenu") == 0)
						options.autoHideMenu = atoi (w2);
					else if (strcmp (w, "bellVol") == 0)
						options.bellVol = atoi (w2);
					else if (strcmp (w, "bellPitch") == 0)
						options.bellPitch = atoi (w2);
					else if (strcmp (w, "bellDuration") == 0)
						options.bellDuration = atoi (w2);
					else if (strcmp (w, "niceLevel") == 0) {
						program_nice = atoi (w2);
						if (program_nice != 0) {
							printf ("Set program nice to %d\n", program_nice);
							nice (program_nice);
						}
					}
					else if (strcmp (w, "showClock") == 0)
						options.showClock = atoi (w2);
					else if (strcmp (w, "showFPS") == 0)
						options.showFPS = atoi (w2);
					else if (strcmp (w, "showTermFPS") == 0)
						options.showTermFPS = atoi (w2);
					else if (strcmp (w, "showFloor") == 0)
						options.showFloor = atoi (w2);
					else if (strcmp (w, "showFloorLines") == 0)
						options.showFloorLines = atoi (w2);
					else if (strcmp (w, "showUsb") == 0)
						options.showUsb = atoi (w2);
					else if (strcmp (w, "showSensors") == 0) {
						options.showSensors = atoi (w2);
						//if (options.showSensors) {
						//	pthread_create (&sensThread, NULL, sensThreadFunc, NULL);
						//	pthread_detach (sensThread);
						//}
					}
					else if (strcmp (w, "updateDelay") == 0)
						options.updateDelay = atoi (w2);
					else if (strcmp (w, "useAlpha") == 0)
						options.useAlpha = atoi (w2);
					else if (strcmp (w, "debug") == 0)
						options.debug = atoi (w2);
					else if (strcmp (w, "verbose") == 0)
						options.verbose = atoi (w2);
					else if (strcmp (w, "showids") == 0)
						debug.showids = atoi (w2);
					else if (strcmp (w, "showPosition") == 0)
						debug.showPosition = atoi (w2);
					else if (strcmp (w, "showPoint") == 0)
						debug.showPoint = atoi (w2);
					else if (strcmp (w, "debugBrowser") == 0)
						debug.browser = atoi (w2);
					else if (strcmp (w, "debugBuffer") == 0)
						debug.buffer = atoi (w2);
					else if (strcmp (w, "debugDisplayList") == 0)
						debug.displayList = atoi (w2);
					else if (strcmp (w, "debugDrawing") == 0)
						debug.drawing = atoi (w2);
					else if (strcmp (w, "debugEvents") == 0)
						debug.events = atoi (w2);
					else if (strcmp (w, "debugMesh") == 0)
						debug.mesh = atoi (w2);
					else if (strcmp (w, "debugPrompt") == 0)
						debug.prompt = atoi (w2);
					
					memset (w, '$', 63);
					memset (w2, '$', 63);
					break;
				}
				else if (c == '"') {
					while (1) {
						c = fgetc (file);
						if (c == '"') {
							break;
						}
						else {
							w2[cnt] = c;
							++cnt;
						}
					}
				}
				else {
					w2[cnt] = c;
					++cnt;
				}
			}
			cnt = 0;
			continue;
		}
		else w[cnt] = c;
		
		if (c == EOF) break;
		++cnt;
	}
	
	fclose (file);
}

void NLsaveConfig (void) {
	return;
}

// todo: write setOptionByName (char *optionName, int value);
int NLsetOptionByID (int optionID, int value) {
 return 0;
}

