#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char binbuf[8];

char *showBinstr (int value) {
	int x = 0, mask = 0x80;
	while (x < 8) {
		//printf ("%d",(value & mask) == mask ? 1 : 0);
		binbuf[x] = (value & mask) == mask ? '1' : '0';
		value = value << 1;
		++x;
	}
	return binbuf;
}

void showBin (int value) {
	int x = 0, mask = 0x80;
	while (x < 8) {
		printf ("%d",(value & mask) == mask ? 1 : 0);
		value = value << 1;
		++x;
	}
}

void dummy (void) {
	int val1 = 4, val2 = 32;
	printf ("%s", showBinstr (val1));
	printf (" ");
	printf ("%s", showBinstr (val2));
	printf (" ");
	printf ("%s\n", showBinstr ((val1 | val2)));
}

int main (int argc, char **argv) {
	dummy ();
	//return 0;
	while (1) {
		char *str;// = malloc (1024);
		int val;
		printf ("Give a numeric value (0 quit): \n");
		scanf ("%s", str);
		val = atoi(str);
		showBin (val);
		printf ("\n\n");
		if (val == 0) break;
	}
	
	return 0;
}
