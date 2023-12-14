#include <stdio.h>

void showBuffer (void) {
	int cnt = 0;
	while (1) {
		putc ('~', stdout); fflush (stdout);
		++cnt;
		if (cnt >= 1024) break;
		else if (cnt % 64 == 0) putc ('\n', stdout);
	}putc ('\n', stdout);
}

void NLinitLexic (void) {
	return;
}
