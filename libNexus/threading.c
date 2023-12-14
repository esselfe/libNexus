#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <sensors/sensors.h>

//#include "camera.h"
#include "config.h"
#include "osdata.h"
#include "drawing.h"
#include "events.h"
#include "hud.h"
#include "memoryViewer.h"
#include "menu.h"
#include "messages.h"
#include "libNexus.h"
#include "state.h"
#include "threading.h"

int curTime = 0;
int autoclearLastTime = 0;
int updatedelayLastTime = 0;
mbool once = 1;
short hudUpdateFPSDelaycnt = 1;
const int TMPSTRSIZE = 64;
int program_ticks = 0;

void initThreading (void) {
	pthread_attr_init (&timerThreadAttr);
	pthread_attr_setdetachstate (&timerThreadAttr, PTHREAD_CREATE_DETACHED);
	pthread_attr_setschedpolicy (&timerThreadAttr, PTHREAD_INHERIT_SCHED);
	pthread_create (&timerThread, &timerThreadAttr, timerThreadFunc, NULL);
	pthread_detach (timerThread);
	pthread_create (&drawingThread, NULL, drawingThreadFunc, NULL);
	pthread_detach (drawingThread);
	return;
}

void *timerThreadFunc (void *arg) {
//	int ret = 0;
	int lastSecond = 0;
	int lastFPS = 0;
	
//nice (-3);
	while (1) {
		program_ticks = glutGet (GLUT_ELAPSED_TIME);
//		printf ("T"); fflush (stdout);
		curTime = program_ticks;
		if (fps != lastFPS) {
			if (options.showFPS) hudUpdateFPS ();
			if (options.showTermFPS) printf ("%d fps\n", fps);
			lastFPS = fps;
		}
		if (curTime >= lastSecond +1000) {
			if (options.showClock) hudUpdateClock ();
			lastSecond = curTime;
		}
		if (options.autoclear && curTime >= autoclearLastTime + options.autoclear) {
			if (msgPending) {
				if (autoclearwait)
					--autoclearwait;
				else {
					msgPauseDrawing = 1;
					decayMsg ();
					msgPauseDrawing = 0;
				}
			}
			autoclearLastTime = curTime;
		}
		if (curTime >= updatedelayLastTime + options.updateDelay) {
		//	if (options.debug) { printf ("thr() "); fflush (stdout); }
			//readProcFile ();
			if (options.showSensors) {
				sensors_get_feature (sensorsChipName3, 54, &CPUtemp);
				sprintf (CPUtempstr, "%s%.F%s", "CPU: ", CPUtemp, "*C");
			}
			if (mode == MODE_MEM)
				updateOSData ();
			
			updatedelayLastTime = curTime; 
		}

		needRedraw = 1;
//		printf ("_"); fflush (stdout);
		usleep (125000);
	}
	return NULL;
}

void *drawingThreadFunc (void *arg) {
	nice (-10);
	while (1) {
		
		delta += deltaStep;
		if (delta >= 360.0)
			delta = 0.0;
//		else 
		//printf ("_"); fflush (stdout);
//		needRedraw = 1;
//		raise (SIGUSR1);
		usleep (1000);
//		if (debug.drawing) 
	}
	return NULL;
}

