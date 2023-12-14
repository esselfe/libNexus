#include <stdio.h>
//#include <linux/pid.h>
//#include <linux/sched.h>
#include <sys/resource.h>

#include "memoryViewer.h"
#include "objects.h"

VerticalMeter ramMeter, swapMeter, usbMeter;

//static void readProcFile (void);

void initMemoryViewer (void) {
	ramMeter.value = 1.0;
	ramMeter.valuePrev = 0.0;
	ramMeter.valueMin = 0.0;
	ramMeter.valueMax = 0.0;
	ramMeter.width = 1.0;
	ramMeter.height = 3.9;//2.56;
	ramMeter.x = -1.25;
	ramMeter.y = 1.28;
	ramMeter.z = 0.0;
	ramMeter.color[0] = 0.0;
	ramMeter.color[1] = 0.5;
	ramMeter.color[2] = 0.0;
	ramMeter.color[3] = 0.3;
	ramMeter.trimColor[0] = 0.0;
	ramMeter.trimColor[1] = 0.7;
	ramMeter.trimColor[2] = 0.0;
	ramMeter.trimColor[3] = 0.7;
	ramMeter.label = addTextObject ("RAM", ramMeter.x - ramMeter.width/2 +0.05, 
									ramMeter.y + ramMeter.height/2 -0.15,
		 ramMeter.z + ramMeter.width/2 +0.1);
	ramMeter.svalue = addTextObject ("256000000/256000000", ramMeter.x - ramMeter.width/2 +0.05, 
									 ramMeter.y + ramMeter.height/2 -0.3,
		  ramMeter.z + ramMeter.width/2 +0.1);
	ramMeter.svalueDiff = addTextObject ("+256000000", ramMeter.x - ramMeter.width/2 +0.05, 
										 ramMeter.y + ramMeter.height/2 -0.45,
		   ramMeter.z + ramMeter.width/2 +0.1);
	
	swapMeter.value = 1.0;
	swapMeter.valuePrev = 0.0;
	swapMeter.valueMin = 0.0;
	swapMeter.valueMax = 0.0;
	swapMeter.width = 1.0;
	swapMeter.height = 2.56;
	swapMeter.x = 0.0;
	swapMeter.y = 1.28;
	swapMeter.z = 0.0;
	swapMeter.color[0] = 0.0;
	swapMeter.color[1] = 0.5;
	swapMeter.color[2] = 0.0;
	swapMeter.color[3] = 0.3;
	swapMeter.trimColor[0] = 0.0;
	swapMeter.trimColor[1] = 0.7;
	swapMeter.trimColor[2] = 0.0;
	swapMeter.trimColor[3] = 0.7;
	swapMeter.label = addTextObject ("Swap", swapMeter.x - swapMeter.width/2 +0.05,
									 swapMeter.y + swapMeter.height/2 - 0.15,
		  swapMeter.z + swapMeter.width/2 + 0.1);
	swapMeter.svalue = addTextObject ("256000000/256000000", swapMeter.x - swapMeter.width/2 +0.05,
									  swapMeter.y + swapMeter.height/2 - 0.3,
		   swapMeter.z + swapMeter.width/2 +0.1);
	swapMeter.svalueDiff = addTextObject ("+256000000", swapMeter.x - swapMeter.width/2 +0.05,
										  swapMeter.y + swapMeter.height/2 -0.45,
			swapMeter.y + swapMeter.width/2 +0.1);

	usbMeter.value = 1.0;
	usbMeter.valuePrev = 0.0;
	usbMeter.valueMin = 0.0;
	usbMeter.valueMax = 0.0;
	usbMeter.width = 1.0;
	usbMeter.height = 0.647;
	usbMeter.x = 2.0;
	usbMeter.y = 1.28;
	usbMeter.z = 0.0;
	usbMeter.color[0] = 0.0;
	usbMeter.color[1] = 0.5;
	usbMeter.color[2] = 0.0;
	usbMeter.color[3] = 0.3;
	usbMeter.trimColor[0] = 0.0;
	usbMeter.trimColor[1] = 0.7;
	usbMeter.trimColor[2] = 0.0;
	usbMeter.trimColor[3] = 0.7;
	usbMeter.label = addTextObject ("USB", usbMeter.x - usbMeter.width/2 +0.05, 
									usbMeter.y + usbMeter.height/2 -0.15,
		 usbMeter.z + usbMeter.width/2 +0.1);
	usbMeter.svalue = addTextObject ("256000000/256000000", usbMeter.x - usbMeter.width/2 +0.05, 
									 usbMeter.y + usbMeter.height/2 -0.3,
		  usbMeter.z + usbMeter.width/2 +0.1);
	usbMeter.svalueDiff = addTextObject ("+256000000", usbMeter.x - usbMeter.width/2 +0.05, 
										 usbMeter.y + usbMeter.height/2 -0.45,
		   usbMeter.z + usbMeter.width/2 +0.1);
}

void showMem (void) {
	printf ("mem:\n");
}

//struct rusage progusage;
//static void readProcFile (void) {
	//pid_task (1, PIDTYPE_PID);
	
	//struct timeval ru_utime
	//getrusage (RUSAGE_SELF, &progusage);
	
	//printf ("utime: %ld  stime: %ld    \n", 
			//progusage.ru_utime.tv_sec, progusage.ru_stime.tv_sec);
	//fflush (NULL);
	
//	return;
//}

