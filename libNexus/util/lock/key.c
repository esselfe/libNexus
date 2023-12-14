//#include <stdio.h>
#include <string.h>

#include "key.h"
#include "lock.h"
#include "lockUtil.h"

int readKey (FILE *file) {
	memset (pwd2, '@', PASSWDSIZE-1);
	char *cp = pwd2;
	while (1) {
		*cp = fgetc (file);
		*cp -= 16;
		if (*cp == '\n') {
			*cp = '\0';
			break;
		}
		++cp;
	}
	
	if (strcmp (pwd, pwd2) != 0) {
		printf ("Password don't match\n");
		return 1;
	}
	
	return 0;
}

int writeKey (FILE *file) {
	askPasswd ();
	
	char *cp = pwd;
	while (1) {
		if (*cp == '\0') {
			*cp = '\n';
			*cp += 16;
			fputc (*cp, file);
			break;
		}
		*cp += 16;
		fputc (*cp, file);
		++cp;
	}
	return 0;
}
