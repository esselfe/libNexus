#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#include "config.h"
#include "drawing.h"
#include "events.h"
#include "Nsignals.h"
#include "util.h"

char *parentcmd = NULL;		// Use by parentProcessRuncmd()
//char *cwd = NULL;
const int CWDSIZE = 0;

void msg64 (char *text) {
	if (once64) printf ("%s", text);
}

void vmsg (int verboseLevel, char *text) {
	if (verboseLevel > options.verbose) return;
	printf ("vmsg(%d): %s\n", verboseLevel, text);
}

void dbgFuncMsg (int setval, char *funcname) {
	printf ("%s(): %s\n", funcname, setval ? "started":"done");
}

int dbgmsgcnt = 0;
void dbgmsg (char *str) {
	++dbgmsgcnt;
	printf ("\t%s %d\n", str, dbgmsgcnt);
}

void beep (int vol, int pitch, int duration) {
	system ("xset q |grep bell | gawk '{ print $3,$6,$9 }' > /root/.nexus/.bell.output");
	FILE *file = fopen ("/root/.nexus/.bell.output", "r");
	if (file == NULL) {
		printf ("beep(): \aCannot save original bell state from /root/.nexus/.bell.output\n");
		perror ("perror");
		return;
	}
	// rem Should restore to system's default
	int origBellVol, origBellPitch, origBellDuration;
	char w[8];
	int cnt = 0, wcnt = 0;
	while (1) {
		w[cnt] = fgetc (file);
		if (w[cnt] == ' ') {
			w[cnt] = '\0';
			cnt = 0;
			switch (wcnt) {
				case 0:
					origBellVol = atoi (w);
					printf ("\tdbg: vol = %d\n", origBellVol);
					++wcnt;
					break;
				case 1:
					origBellPitch = atoi (w);
					printf ("\tdbg: pitch: %d\n", origBellPitch);
					++wcnt;
					break;
				case 2:
					origBellDuration = atoi (w);
					printf ("\tdbg: dur: %d\n", origBellDuration);
					++wcnt;
					break;
			}
		}
		else if (w[cnt] == EOF) {origBellDuration = atoi (w);
			printf ("\tdbg: dur: %d\n", origBellDuration);break;}
		else ++cnt;
	}
	
	char *tmpcmd = malloc (256);
	sprintf (tmpcmd, "xset b %d %d %d", options.bellVol, options.bellPitch, options.bellDuration);
	printf ("running %s\n", tmpcmd);
	system (tmpcmd);
	printf ("\a"); fflush (stdout);
	sprintf (tmpcmd, "xset b %d %d %d", origBellVol, origBellPitch, origBellDuration);
	printf ("running %s\n", tmpcmd);
	system (tmpcmd);
	free (tmpcmd);
}

void *terminalThreadFunc (void *arg) {
	//nice (-3);
	lowRedraw = 1;
	system (parentcmd);
	//nice (program_nice);
	lowRedraw = 0;
	return NULL;
}

void *terminalThreadFunc2 (void *arg) {
	system (parentcmd);
	return NULL;
}

void parentProcessRuncmd (char *str) {
	parentcmd = malloc (strlen(str)+1);
	sprintf (parentcmd, "%s", str);
	pthread_t parentTerminalThread;
	pthread_create (&parentTerminalThread, NULL, terminalThreadFunc, NULL);
	pthread_detach (parentTerminalThread);
}
/*
void setcwd (void) {
	cwd = malloc (CWDSIZE);
	memset (cwd, '>', CWDSIZE);
	
	char *parentcwd = get_current_dir_name();
	
	//char *parentdir = malloc (strlen (progname));
	//char w[128];
	char *cp = progname + strlen (progname)-1;
	//int cnt = 0;
 	while (1) {
 		if (*cp == '/') break;
}
	
	sprintf (cwd, "%s/%s", parentcwd, progname);
	
	free (parentcwd);
	printf ("cwd = %s\n", cwd);
}*/

char *getParentDirName (char *str) {
	char *parentdirname = NULL;
	
	char *cp = str + strlen (str)-1;
	char *cp2 = NULL;
	int cnt = 0, cnt2 = 0;
	while (1) {
		if (cp == str) {
			parentdirname = malloc (strlen(str)+1);
			sprintf (parentdirname, "%s", str);
			return parentdirname;
		}
		else if (*cp == '/') {
			parentdirname = malloc (cnt+1);
			cp = str;
			cp2 = parentdirname;
			while (1) {
				*cp2 = *cp;
				++cp;
				++cp2;
				++cnt2;
				if (cnt2 >= strlen(str)-cnt) {
					return parentdirname;
				}
			}
		}
		else {
			--cp;
			++cnt;
		}
	}
	return NULL;
}

void setTerminalColor (int newColor) {
	switch (newColor) {
		case NNORMAL:
			printf ("\033[00m");
			break;
		case NRED:
			printf ("\033[00;31m");
			break;
		case NGREEN:
			printf ("\033[00;32m");
			break;
		case NBLUE:
			printf ("\033[00;34m");
			break;
		case NBRED:
			printf ("\033[01;31m");
			break;
		case NBGREEN:
			printf ("\033[01;32m");
			break;
		case NBBLUE:
			printf ("\033[01;34m");
			break;
		default:
			printf ("setTerminalColor(): no color\n");
			break;
	} fflush (stdout);
}

void NLUinitSignals (void (*signalHandler)(int signum)) {
	int cnt = 1;
	while (1) {
		signal (cnt, NLsignalHandler);
		++cnt;
		if (cnt >= 32) break;
	}
}
