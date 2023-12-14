#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main (int argc, char **argv) {
	if (argc == 2) {
		if (strcmp (argv[1],"-a") == 0){
			int x = 0;
			char str[45];
			for (;;) {
				errno = x;
				sprintf (str, "%d", x);
				perror (str);
				++x;
				if (x >= 256) return 0;
			}
		}
		errno = atoi (argv[1]);
		perror ("errno");
	}
	else {
		printf ("Usage: errno n\n");
		printf ("       errno -a\n");
	}
	return 0;
}
