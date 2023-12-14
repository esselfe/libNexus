#include <stdio.h>

int main (void) {
	
	unsigned char c = 0;
	int cnt = 0;
	while (1) {
		printf ("%d=:%c:\n", (int)c, c);
		++c;
		++cnt;
		if (cnt >= 255) break;
	}
	
	return 0;
}
