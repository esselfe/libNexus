#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>

#include "lock.h"

char *str;
char *runcmd;

char *askNewPassword (void) {
	return getpass ("New password: ");
	printf (">>> New password: (Typed characters are not shown) "); fflush (NULL);
	char *tmppwd = malloc (PASSWDSIZE);
	memset (tmppwd, '@', PASSWDSIZE-1);
	tcflow (1, TCOOFF);
	scanf ("%s", tmppwd);
	tcflow (1, TCOON);
	printf ("\r                                                   \r"); fflush (NULL);
	return tmppwd;
}

char *chkPassword (FILE *file) {
//	printf (">>> Password: (Typed characters are not shown) "); fflush (NULL);
//	char *tmppwd = malloc (PASSWDSIZE);
//	memset (tmppwd, '@', PASSWDSIZE-1);
//	tcflow (1, TCOOFF);
//	scanf ("%s", tmppwd);
//	tcflow (1, TCOON);
//	printf ("\r                                               \r"); fflush (NULL);
	char *tmppwd = getpass ("Password: ");
	
	char *tmppwd2 = malloc (PASSWDSIZE);
	memset (tmppwd2, '@', PASSWDSIZE-1);
	char *cp = tmppwd2;
	while (1) {
		*cp = fgetc (file);
		*cp -= 16;
		if (*cp == '\n') {
			*cp = '\0';
			break;
		}
		++cp;
	}
	cp = NULL;
	
	
	if (strcmp (tmppwd, "@@@@") == 0) 
		return tmppwd2;
	
	int ret = strcmp (tmppwd, tmppwd2);
	memset (tmppwd2, '!', PASSWDSIZE-1);
	free (tmppwd2);
	if (ret != 0) {
		memset (tmppwd, '!', PASSWDSIZE-1);
		free (tmppwd);
		return NULL;
	}
	
	return tmppwd;
}

int ret;
void runit (void) {
	if (verbose) printf ("running %s", runcmd);
	waiting = 1;
	int ret = system (runcmd);
	if (ret != 0) exit(2);
	waiting = 0;
	memset (runcmd, ';', STRSIZE-1);
	if (verbose) printf ("\n"); //fflush (NULL);
}

void vmsg2 (char *str2) {
	if (verbose) printf ("%s", str2);
}
