#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
//#include <pthread.h>

#include <osdata.h>
#include <memfiller.h>

const char *MEMFILLER_VERSION = "0.1.1.0";
const int MEMFILLER_BUILD = 0x12;

long unsigned int totalbytes = 0;
pthread_t memfillThread;
int done = 0;

NodeTree *rootNode, *lastNode;

void *tmpFunc (void *argp) {
	while (!done) {
		printf ("totalBytes: %lu\n", totalbytes); //fflush (stdout);
		sleep (1);
	}
	//printf ("\n");
	return NULL;
}

void initNodeTree (void) {
	rootNode = malloc (sizeof (NodeTree));
	rootNode->next = NULL;
	lastNode = rootNode;
	totalbytes = sizeof (NodeTree);
}

void destroyNodeTree (void) {
	return;
}

int memfill (int memtype, char *fileLocation) {
	printf ("memfill(): started\n");
	int ret = 0;
	if (memtype == MEMFILL_TYPE_RAM) {
		printf ("\tNode size: %d bits\n", sizeof (NodeTree)*8);
		totalbytes = sizeof (NodeTree);
		//int qty = 0;
		//printf ("\tNOTE: Give 0 to quit\n");
		while (1) {
			// will try messageBox input instead...
			//printf ("\t>>> nodes to add (%dB): ", totalbytes); fflush (NULL);
			//scanf ("%d", &qty);
			//if (qty <= 0) return;
			ret = addNode (10000);
			if (ret != 0) {
				printf ("memfill(): had %lu bytes, returning %d\n", totalbytes, ret);
				return ret;
			}
		}
	}
	else if (memtype == MEMFILL_TYPE_DISK) {
		if (fileLocation == NULL) {
			printf ("\tCannot use NULL as second arg for disk filling\n");
			return -1;
		}
		char *filename = malloc (strlen (fileLocation) + strlen ("/memfill.fill"));
		sprintf (filename, "%s%s", fileLocation, "/memfill.fill");
		FILE *file = fopen (filename, "wb");
		if (file == NULL) {
			perror ("\tCannot open filling file");
			free (filename);
			return -1;
		}
		
		while (1) {
			ret = fputc ('F', file);
			if (ret != 0) {
				perror ("\tCannot write byte on media");
				fclose (file);
				free (filename);
				printf ("memfill(): had %llu bytes, returning %d\n",
						(unsigned long long int)totalbytes, ret);
				return ret;
			}
			totalbytes += 1;
		}
		
		fclose (file);
		free (filename);
	}
	
	printf ("memfill(): done\n");
	return 0;
}

void *memfillThreadFunc (void *argp) {
	sysinfo (&sinfo);
	printf ("memfillerThreadFunc(): started\n");
	printf ("\tTotal RAM: \t%lu\n", sinfo.totalram);
	printf ("\tUsed RAM: \t%lu\n", sinfo.totalram - sinfo.freeram);
	printf ("\tFree RAM: \t%lu\n", sinfo.freeram);
	unsigned long neededBytes = sinfo.freeram;
	printf ("\tallocating \t%lu bytes...\n", neededBytes);
		addNode (neededBytes+2000000000);// - 20000000);
	printf ("memfillerThreadFunc(): done\n");
	return NULL;
}

void thisFunc (void) {
	printf ("this is there\n");
	while (1) {
		printf ("%s %d\n", "once", 16);
	}
}

int addNode (unsigned long int qty) {
	printf ("addNode(%lu): started\n", qty);
	if (qty <= 0) return 0;
	
	initNodeTree ();
	NodeTree *node = lastNode;
	
	pthread_t tmpThread;
	pthread_create (&tmpThread, NULL, tmpFunc, NULL);
	pthread_detach (tmpThread);
	
	int nodeSize = sizeof (NodeTree);
	printf ("\tNode total size: %d bytes\n", nodeSize+sizeof(node->next)+sizeof(node->val));
	while (1) {
		node->next = malloc (nodeSize);
		if (node->next == NULL) {
			perror ("malloc perror");
			return -1;
		}
		node = node->next;
		node->next = NULL;
		totalbytes += nodeSize+sizeof(node->next)+sizeof(node->val);
		if (totalbytes >= qty) break;
		//usleep ();
	}
	lastNode = node;
	done = 1;
	//sleep (2);
	printf ("\taddNode(): done, %lu bytes\n", totalbytes);
	return 0;
}

void memfillAtexit (void) {
	printf ("memfill exited\n");
}

void memfillSignalHandler (int signum) {
	char *sigstr = malloc (128);
	sprintf (sigstr, "\033[01;31mmemfillSignalHandler(): got signal %d", signum);
	psignal (signum, "psignal()");
	printf ("\033[00m"); fflush(stdout);
	free (sigstr);
}

int main (int argc, char **argv) {
	if (argc == 2 && strcmp (argv[1], "--useXterm") == 0) {
		system ("xterm -g 60x20+1+300 -e memfiller 2>/dev/null &");
		return 0;
	}
	printf ("\033[01;36mMemfiller version %s, build 0x%2X started\033[00m\n", MEMFILLER_VERSION, MEMFILLER_BUILD);
	atexit (memfillAtexit);
	printf ("mfill:atexit() done\n");
	//int ret = daemon (0,0);
	//printf ("mfill:daemon() returns %d\n", ret);
	/* Register 'memfillSignalHandler()' for each signum */
	int cnt = 1;
	while (1) {
		signal (cnt, memfillSignalHandler);
		++cnt;
		if (cnt >= 32) break;
	}
	
	memfillThreadFunc (NULL);
	
	printf ("\033[01;36mMemfiller done\033[00m\n");
	
	return 0;
}
