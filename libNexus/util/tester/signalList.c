#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main (int argc, char **argv) {
	int cnt = 0;
	char *buf = malloc (128);
	while (1) {
		sprintf (buf, "%d", cnt);
		psignal (cnt, (const char *)buf);
		++cnt;
		if (cnt >= 32) break;
	}

	return 0;
}

