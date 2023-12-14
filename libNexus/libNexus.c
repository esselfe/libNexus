#include <stdio.h>

const char *LIBNEXUS_VERSION = "0.1.0";
const unsigned int LIBNEXUS_BUILD = 0x01;
// 
void NLshowVersion (void) {
	printf ("libNexus version %s, build 0x%2.X\n", LIBNEXUS_VERSION, LIBNEXUS_BUILD);
}
