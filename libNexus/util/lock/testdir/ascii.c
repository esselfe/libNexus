#include <stdio.h>

unsigned char charArray[256];

void initCharArray (void) {
	int cnt = 0;
	while (1) {
		charArray[cnt] = (unsigned char)cnt;
		++cnt;
		if (cnt >= 255) break;
	}
}
