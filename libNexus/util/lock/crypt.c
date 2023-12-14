#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "lock.h"
#include "lockUtil.h"

unsigned char *encodeString (unsigned char *string) {
	
	return NULL;
}

unsigned char *decodeString (unsigned char *string) {
	
	return NULL;
}

int encryptChest (char *filename) {
	printf ("Locking\n");
	char *tmppwd = askNewPassword ();
	
	sprintf (runcmd, "tar cf %s.tar %s", filename, filename);
	runit ();
	
	sprintf (runcmd, "rm -rf %s", filename);
	runit ();
	
	sprintf (runcmd, "bzip2 %s.tar", filename);
	runit ();
	
	char *srcname = malloc (CHESTNAMESIZE);
	sprintf (srcname, "%s.tar.bz2", filename);
	FILE *chestsrc = fopen (srcname, "rb");
	if (chestsrc == NULL) {
		printf ("lockUtil.hest: err src fopen(%s, r)\n", srcname);
		perror ("perror");
		free (srcname);
		return 1;
	}
	free (srcname);
	
	fseek (chestsrc, 0, SEEK_END);
	chestsize = ftell (chestsrc);
	fseek (chestsrc, 0, SEEK_SET);
	
	char *dstname = malloc (CHESTNAMESIZE);
	sprintf (dstname, "%s.chest", filename);
	FILE *chestdst = fopen (dstname, "wb");
	if (chestdst == NULL) {
		printf ("lockUtil.hest(): err dst fopen(%s, w)", dstname);
		perror ("perror");
		free (dstname);
		fclose (chestsrc);
		return 1;
	}
	free (dstname);
	
	if (verbose) printf ("Encrypting chest");
	waiting = 1;
	char *cp = tmppwd;
	while (1) {
		if (*cp == '\0') {
			*cp = '\n';
			*cp += 16;
			fputc (*cp, chestdst);
			break;
		}
		*cp += 16;
		fputc (*cp, chestdst);
		++cp;
	}
	cp = NULL;
	free (tmppwd);
	
	char c = ' ';
	unsigned long cnt = 0;
	while (1) {
		c = fgetc (chestsrc);
		c += 16;
		fputc (c, chestdst);
		++cnt;
		if (cnt >= chestsize) break;
	} if (verbose) printf ("\n");
	fclose (chestsrc);
	fclose (chestdst);
	waiting = 0;
			
	sprintf (runcmd, "rm %s.tar.bz2", filename);
	runit ();
	if (verbose) printf ("Locked\n");
	return 0;
}

int decryptChest (char *filename) {
	printf ("Unlocking\n");
	
	char *srcname = malloc (CHESTNAMESIZE);
	sprintf (srcname, "%s.chest", filename);
	FILE *chestsrc = fopen (srcname, "rb");
	if (chestsrc == NULL) {
		printf ("writeChest(): err src fopen(%s, r)\n", srcname);
		perror ("perror");
		free (srcname);
		return 1;
	}
	free (srcname);
			
	fseek (chestsrc, 0, SEEK_END);
	chestsize = ftell (chestsrc);
	fseek (chestsrc, 0, SEEK_SET);
	
	char *tmppwd = chkPassword (chestsrc);
	if (tmppwd == NULL) {
		printf ("Bad password, aborting\n");
		fclose (chestsrc);
		return 1;
	}
	chestsize -= strlen (tmppwd)+1;
	free (tmppwd);
	
	char *dstname = malloc (CHESTNAMESIZE);
	sprintf (dstname, "%s.tar.bz2", filename);
	FILE *chestdst = fopen (dstname, "wb");
	if (chestdst == NULL) {
		printf ("unlockUtil.hest(): err dst fopen(%s, w)\n", dstname);
		perror ("perror");
		fclose (chestsrc);
		free (srcname);
		free (dstname);
		return 1;
	}
	free (dstname);
	
	char c = ' ';
	unsigned long cnt = 0;
	if (verbose) printf ("Decrypting chest");
	waiting = 1;
	while (1) {
		c = fgetc (chestsrc);
				//if (c == EOF) break;
		c -= 16;
		fputc (c, chestdst);
		++cnt;
		if (cnt >= chestsize) break;
	} if (verbose) printf ("\n");
	waiting = 0;
			
	fclose (chestsrc);
	fclose (chestdst);
	
	sprintf (runcmd, "rm %s.chest", filename);
	runit ();
	sprintf (runcmd, "tar xf %s.tar.bz2", filename);
	runit ();
	sprintf (runcmd, "rm %s.tar.bz2", filename);
	runit ();
	if (verbose) printf ("Unlocked\n");
	return 0;
}
