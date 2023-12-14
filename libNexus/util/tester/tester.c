#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/cdefs.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>

#include "config.h"
#include "libNexus.h"
#include "state.h"
#include "util.h"

unsigned long tmp = 0;
unsigned int tmp2 = 0;
unsigned long long tmp3 = 0;

int tmp4 = 0;
int testerDone = 1;
pthread_t testerThread;

void *testerThreadFunc (void *argp) {
	printf ("tester written to beep, press 'T' to stop\n");
	testerDone = 0;
	//nice (-3);
	//system (xsetcmd);
	char *teststr = malloc (128);
	sprintf (teststr, "top -p %d,%d", program_pid, program_ppid);
	system (teststr);
	while (!testerDone) {
		//printf ("\a"); fflush (NULL);
		
		usleep (1000000);
	}
	//nice (program_nice);
	free (teststr);
	return NULL;
}

void startTester (void) {
	pthread_create (&testerThread, NULL, testerThreadFunc, NULL);
	pthread_detach (testerThread);
}

int tester (void) {
	//printf ("tester():\n");
	
	
	
	/*setTerminalColor (NNORMAL);
	printf ("normalColor\n");
	setTerminalColor (NRED);
	printf ("redCOlor\n");
	setTerminalColor (NGREEN);
	printf ("greenColor\n");
	setTerminalColor (NBLUE);
	printf ("blueColor\n");
	setTerminalColor (NBRED);
	printf ("redvCOlor\n");
	setTerminalColor (NBGREEN);
	printf ("greenvColor\n");
	setTerminalColor (NBBLUE);
	printf ("bluevColor\n");
	setTerminalColor (NNORMAL);
	printf ("normalColor\n");*/
	/*char c = 2;
	printf ("\tchar \t@%p\n", (void *)&c);
	
	int i = 16;
	printf ("\tint \t@%p\n", (void *)&i);
	
	long l = 32;
	printf ("\tlong \t@%p\n", (void *)&l);
	
	long long ll = 64;
	printf ("\tlong long \t@%p\n", (void *)&ll);
	
	char c2 = 2;
	printf ("\tchar \t@%p\n", (void *)&c2);
	char c3 = 2;
	printf ("\tchar \t@%p\n", (void *)&c3);*/
	
	//int err = 0;
	//if (options.debug) {
		//printf ("Entry size: %d\n", sizeof (struct Entry));
	
	//}
	//printf ("%d\n", 1+12+13+4+3+10);
	
	
	/*int testint = 0;
	printf ("max unsigned int value: %u\n", (unsigned int)testint-1);
	*/
// 	printf ("GL_POINTS: %d\n", GL_POINTS);
// 	printf ("GL_LINE: %d\n", GL_LINE);
// 	printf ("GL_LINES: %d\n", GL_LINES);
// 	printf ("GL_LINE_STRIP: %d\n", GL_LINE_STRIP);
// 	printf ("GL_POLYGON: %d\n", GL_POLYGON);
	/*
#ifdef __USE_BSD
	printf ("__USE_BSD defined\n");
#else
	printf ("__USE_BSD not defined\n");
#endif
#ifdef __USE_XOPEN_EXTENDED
	printf ("__USE_XOPEN_EXTENDED defined\n");
#else
	printf ("__USE_XOPEN_EXTENDED not defined\n");
#endif
	*/
	/*char *tmp = malloc (128);
	sprintf (tmp, "%s",__FUNCTION__);
	printf ("%s:%d:%s():\n", __FILE__,__LINE__,tmp);
	free (tmp);*/
	/*
	int a = 12;
	printf ("typeof(int): %s\n", __typeof (a));
	++a;
	*/
	//printf ("tester(): done\n");
	return 0;
}
