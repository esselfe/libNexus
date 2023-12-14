#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "Ninit.h"
#include "state.h"
#include "util.h"

void NLsignalHandler (int signum) {
	NLsetState (STATE_SIGNAL, STATE_OK);
	if (signum == SIGUSR1) {
		printf ("Sigusr1"); fflush (stdout);
		return;
	}
	
	char *sigstr = malloc (128);
	sprintf (sigstr, "NLsignalHandler(): Got signal %d", signum);
	printf ("\033[01;31m%s!\033[00m\n", sigstr);
	free (sigstr);
	
	setTerminalColor (NRED);
	psignal (signum, "signal description:");
	setTerminalColor (NNORMAL);
	
	if (signum == SIGINT) //exit (0);
	
	exitNexus (STATE_SIGNAL, STATE_OK, __FUNCTION__);
}
