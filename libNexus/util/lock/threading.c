#include <stdio.h>
#include <unistd.h>

#include "lock.h"
#include "threading.h"

pthread_t timerThread;

void *timerThreadFunc (void *arg) {
	if (enablenice) nice (prognice);
	while (1) {
		if (waiting) {
			printf (".");
			fflush (stdout);
			sleep (1);
			continue;
		}
		sleep (1);
	}
	return NULL;
}
