/* libNexus program argument local processing */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <pthread.h>

#include "config.h"
#include "drawing.h"
#include "libNexus.h"
#include "Ninit.h"
#include "state.h"
#include "tester.h"
#include "memfiller.h"

static char const short_options[] = "bd:eFf:hm:v:";
static struct option const long_options[] =
{
	{"browser",		no_argument, 0, 'b'},
 //{"debug",		no_argument, 0, 'd'},
 {"debug",		required_argument, 0, 'd'},
 //{"debug",		optional_argument, 0, 'd'},
 {"editor",		no_argument, 0, 'e'},
 {"fullscreen",	no_argument, 0, 'F'},
 {"function",	required_argument, 0, 'f'},
 {"help",		no_argument, 0, 'h'},
 {"mode",		required_argument, 0, 'm'},
 {"verbose",		required_argument, 0, 'v'},
 {0, 0, 0, 0}
		//{NULL, NULL, NULL, NULL}
};

void NLusage (void) {
	printf ("Usage: %s \n \
			\t-b: Start in file browser mode\n \
			\t-F --fullscreen: Start fullscreen\n \
			\t-h | --help: Shows this help \n \
			\t-m --mode: Preset rendering mode\n \
			\t-v n: Verbose level between 0 and 4\n \
			\t-V | --version: Shows program version", program_name);
}

int NLgetargs (void) {
	NLsetState (STATE_ARGS, STATE_OK);
	int c;
	while ((c = getopt_long (*program_argc, program_argv, short_options, long_options, NULL))	!= -1) {
		switch (c) {
			case 0:
				// Certain long options cause getopt_long to return 0. (stuff about using 'NULL' as terminator instead)
				break;
			case 'b':
				mode = MODE_BROWSER;
				renderingModeFunc = renderBrowser;
				break;
			case 'd':
				if (optarg != NULL) {
					if (strcmp (optarg, "browser") == 0)
						debug.browser = 1;
					else if (strcmp (optarg, "buffer") == 0)
						debug.buffer = 1;
					else if (strcmp (optarg, "drawing") == 0)
						debug.drawing = 1;
					else if (strcmp (optarg, "events") == 0)
						debug.events = 1;
					else if (strcmp (optarg, "mesh") == 0)
						debug.mesh = 1;
					else if (strcmp (optarg, "prompt") == 0)
						debug.prompt = 1;
					else printf ("getargs(): available: buffer,drawing,events,mesh,prompt\n");
				}
				else printf ("getargs(): optarg is NULL\n");
				break;
			case 'e':
				mode = MODE_EDITOR;
				renderingModeFunc = renderEditor;
				break;
			case 'f':
				if (strcmp (optarg, "funcList") == 0) {
					printf ("\tmemfill: Free RAM space by saturating it.\n");
					printf ("\ttester: call /nexus_src/src/tester/tester.c::tester()\n");
				}
				else if (strcmp (optarg, "tester") == 0) {
					tester ();
					exitNexus (STATE_TESTER, STATE_OK, __FUNCTION__);
				}
				else {
					printf ("no functions named %s\n", optarg);
					exitNexus (STATE_FUNCTION, STATE_ERROR, __FUNCTION__);
				}
				break;
			case 'h':
				NLusage ();
				exitNexus (STATE_ARGS, STATE_OK, __FUNCTION__);
				break;
			case 'm':
				if (strcmp (optarg, "browser") == 0) {
					mode = MODE_BROWSER;
					renderingModeFunc = renderBrowser;
				}
				else if (strcmp (optarg, "editor") == 0) {
					mode = MODE_EDITOR;
					renderingModeFunc = renderEditor;
				}
				else if (strcmp (optarg, "memory") == 0) {
					mode = MODE_MEM;
					renderingModeFunc = renderMem;
				}
				else if (strcmp (optarg, "codeParse") == 0) {
					mode = MODE_CODEPARSE;
					renderingModeFunc = renderCodeParse;
				}
				else
					printf ("getargs(): Possible modes are: browser,editor,memory,codeParse\n");
				break;
			case 'v':
				options.verbose = atoi (optarg);
				break;
			default:
				NLusage ();
				exitNexus (STATE_ARGS, STATE_OK, __FUNCTION__);
				break;
		}
	}
	return 0;
}

