#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <pthread.h>
#include <termios.h>
#define __need_timeval 1
#include <time.h>
#include <errno.h>

#include "crypt.h"
#include "threading.h"
#include "lockUtil.h"
#include "libNexus.h"

const int LOCK_VERSION = 0x04;

int prognice = 0;
short enablenice = 0;
const int CHESTNAMESIZE = 128;
const int PASSWDSIZE = 64;
const int STRSIZE = 128;
const char *EXTSTR = ".chest";

const char programPassword[3] = {14,16,32};

int verbose = 0;
int waiting = 0;
time_t clkstart;
char *dstname = NULL;
char *srcname = NULL;
char *cwd = NULL;
unsigned long chestsize;
pthread_t timerThread2;

void cleanup (void) {
	printf ("Done\n");
}


typedef char _N128bits[16];
typedef char _N512bits[512/8];

int chkProgramPassword (void) {
	FILE *file = fopen ("/root/.lock.passwd", "r");
	if (file == NULL) {
		printf ("Need proper password file, exiting\n");
		return 2;
	}
	char wSafe[32];
	char c;
	int cnt = 0;
	while (1) {
		c = fgetc (file);
		if (c == EOF)
			break;
		wSafe[cnt] = c;
		++cnt;
	}
	fclose (file);
	
	//int err = 0;
	//cnt = 0;
	//while (1) {
	//	if (programPassword[cnt] == 0) break;
	//	if (programPassword[cnt] != wSafe[cnt]) { err = 1; break; }
	//	++cnt;
	//}
	if (wSafe[0] == 14 && wSafe[1] == 16 && wSafe[2] == 32) {
		return 0;
	}
	else {//if (err) { 
		printf ("chkProgramPassword(): err\n");
		return 1;
	}
	return 0;
}

void lockusage (void) {
	printf ("Usage: lock { filename | -V | --version | -h | --help }\n");
	printf ("\t'filename' is the name of a directory to lock or the name of a chest without the '.chest' extension\n");
}

int main (int argc, char **argv) {
	if (argc == 1) { lockusage (); return 1; }
	if (argc >= 2) {
		if (strcmp ("--version", argv[1]) == 0 || strcmp ("-V", argv[1]) == 0) {
			printf ("lock version %X\n", LOCK_VERSION);
			return 0;
		}
		else if (strcmp ("--help", argv[1]) == 0 || strcmp ("-h", argv[1]) == 0) {
			lockusage ();
			return 1;
		}
	}
	atexit (cleanup);
	
	int ret = chkProgramPassword ();
	if (ret != 0) return 1;
	
	if (argc != 2) {
		printf ("Need secret args, exiting\n");
		
		sleep (3);
		printf (": %d bits\n", 128/8);
		printf ("int: %d bits\n", sizeof (int)*8);
		printf ("_128bits: %d bits\n", sizeof (_N128bits)*8);
		exit (0);
		return 1;
	}
	else {
		if (enablenice) nice (prognice);
		
		pthread_create (&timerThread2, NULL, timerThreadFunc, NULL);
		pthread_detach (timerThread2);
		
		str = malloc (STRSIZE);
		memset (str, 0, STRSIZE);
		
		runcmd = malloc (STRSIZE);
		memset (runcmd, 0, STRSIZE);
		
		if (strcmp (argv[1], "fill") == 0) {
			vmsg2 ("Opening tmp.fill\n");
			FILE *file = fopen ("tmp.fill", "wb");
			char c = '*';
			while (1) {
				if (fputc (c, file) < 0) break;
			}
			chestsize = ftell (file);
			fclose (file);
			printf ("Removing tmp.fill, %lu bytes\n", chestsize);
			sprintf (runcmd, "rm tmp.fill");
			runit ();
			exit(0);
		}
		else {
			char *cwd = get_current_dir_name ();
			DIR *dir = opendir (cwd);
			if (dir == NULL) {
				printf ("lock:%d: opendir(\"%s\") returns NULL", __LINE__, cwd);
				perror ("perror");
				return 1;
			}
			
			struct dirent *entry;
			char *chestname2 = malloc (CHESTNAMESIZE);
			sprintf (chestname2, "%s%s", argv[1], EXTSTR);
			while (1) {
				entry = readdir (dir);
				if (entry == NULL) {
					printf ("Cannot find %s nor %s.chest in %s, aborting\n",
							argv[1], argv[1], cwd);
					return 1;
				}
				
				if (strcmp (argv[1], entry->d_name) == 0) {
					encryptChest (argv[1]);
					break;
				}
				else if (strcmp (chestname2, entry->d_name) == 0) {
					decryptChest (argv[1]);
					break;
				}
			}
			
			closedir (dir);
			exit(0);
		}
	}
	
	printf ("lock.x ended abnormally\n");
	return 1;
}
