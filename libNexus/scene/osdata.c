#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/statfs.h>
#include <sys/sysinfo.h>
#include <sensors/sensors.h>
#include <linux/sched.h>
#include <GL/glut.h>

#include <config.h>
#include <osdata.h>
#include <hud.h>
#include "memoryViewer.h"
#include <menu.h>
#include <objects.h>

char *str2;
char *tmpstr;// = "100.10*C";
char *CPUtempstr;
double CPUtemp;
struct statfs *tmpstat;
const sensors_chip_name *sensorsChipName = NULL, sensorsChipName2;
sensors_chip_name sensorsChipName3;


struct task_struct *task;

void initOSData (void) {/*
	task = find_task_by_pid (program_pid);
	if (task != NULL) printf ("got a task\n");
	else printf ("have no task\n");*/
	//printf ("initData(): started\n");
	str2 = malloc (1024);
	tmpstat = malloc (sizeof (struct statfs));
	
	tmpstr = malloc (TMPSTRSIZE);
	memset (tmpstr, ' ', TMPSTRSIZE);
	CPUtempstr = malloc (128);
	memset (CPUtempstr, ' ', 128);
//	updateSensorsInfo ();
	//printf ("0\n");
	FILE *file = fopen ("/etc/sensors.conf","r");
	if (file == NULL) {
		printf ("initData(): Cannot open /etc/sensors.conf\n");
		perror ("perror");
	}
	else {
		//printf ("1\n");
		sensors_init (file);
		int chipcnt = 0;
//		while (1) {
			//printf ("2\n");
			sensorsChipName = sensors_get_detected_chips (&chipcnt);
			if (sensorsChipName == 0) {
				printf ("nexus:initData():sensors:%d Cannot detect chip\n", __LINE__);
				return;
			}
//			sensorsChipName2.prefix = sensorsChipName->prefix;
//			sensorsChipName2.bus = sensorsChipName->bus;
//			sensorsChipName2.addr = sensorsChipName->addr;
//			sensorsChipName2.busname = sensorsChipName->busname;
//	printf ("\t3\n");
			sensorsChipName3.prefix = sensorsChipName->prefix;
//	printf ("\t4\n");
			sensorsChipName3.bus = sensorsChipName->bus;
//	printf ("\t5\n");
			sensorsChipName3.addr = sensorsChipName->addr;
//	printf ("\t6\n");
			sensorsChipName3.busname = sensorsChipName->busname;
//	printf ("\t7\n");
		sensors_get_feature (sensorsChipName3, 54, &CPUtemp);
//	printf ("\t8\n");
		sprintf (CPUtempstr, "%s%.0F%s", "CPU: ", CPUtemp, "°C");
//	printf ("\t9\n");
	}
	//printf ("3\n");
	updateOSData ();
	//printf ("4\n");
//	printf ("initData(): done\n");
}

void updateOSData (void) {
	sysinfo (&sinfo);
	
	ramMeter.value = (sinfo.totalram - sinfo.freeram); // / 100000000.0;
//	if (ramMeter.value < ramMeter.valueMin) {
//		ramMeter.valueMin = ramMeter.value;
//		sprintf (ramMeter.svalueMin, "%f", ramMeter.valueMin);
//	}
//	if (ramMeter.value > ramMeter.valueMax) {
//		ramMeter.valueMax = ramMeter.value;
//		sprintf (ramMeter.svalueMax, "%f", ramMeter.valueMax);
//	}
	sprintf (ramMeter.svalue->text, "%lu/%lu", (sinfo.totalram - sinfo.freeram), sinfo.totalram);
	if (ramMeter.value > ramMeter.valuePrev) {
		sprintf (ramMeter.svalueDiff->text, "+%9.lu", ramMeter.value - ramMeter.valuePrev);
		ramMeter.svalueDiff->textColor[0] = 0.8;
		ramMeter.svalueDiff->textColor[1] = 0.5;
		ramMeter.svalueDiff->textColor[2] = 0.5;
		ramMeter.svalueDiff->textColor[3] = 0.6;
	}
	else if (ramMeter.value < ramMeter.valuePrev) {
		sprintf (ramMeter.svalueDiff->text, "-%9.lu", ramMeter.valuePrev - ramMeter.value);
		ramMeter.svalueDiff->textColor[0] = 0.5;
		ramMeter.svalueDiff->textColor[1] = 0.5;
		ramMeter.svalueDiff->textColor[2] = 0.8;
		ramMeter.svalueDiff->textColor[3] = 0.6;
	}
	ramMeter.valuePrev = ramMeter.value;

	swapMeter.value = (sinfo.totalswap - sinfo.freeswap);
	sprintf (swapMeter.svalue->text, "%lu/%lu", (sinfo.totalswap - sinfo.freeswap), sinfo.totalswap);
	if (swapMeter.value > swapMeter.valuePrev) {
		sprintf (swapMeter.svalueDiff->text, "+%9.lu", swapMeter.value - swapMeter.valuePrev);
		swapMeter.svalueDiff->textColor[0] = 0.8;
		swapMeter.svalueDiff->textColor[1] = 0.5;
		swapMeter.svalueDiff->textColor[2] = 0.5;
		swapMeter.svalueDiff->textColor[3] = 0.6;
	}
	else if (swapMeter.value < swapMeter.valuePrev) {
		sprintf (swapMeter.svalueDiff->text, "-%9.lu", swapMeter.valuePrev - swapMeter.value);
		swapMeter.svalueDiff->textColor[0] = 0.8;
		swapMeter.svalueDiff->textColor[1] = 0.5;
		swapMeter.svalueDiff->textColor[2] = 0.5;
		swapMeter.svalueDiff->textColor[3] = 0.6;
	}
	swapMeter.valuePrev = swapMeter.value;

	statfs ("/mnt/sda1", tmpstat);
	usbMeter.value = tmpstat->f_bsize*tmpstat->f_blocks - tmpstat->f_bsize*tmpstat->f_bfree;
	sprintf (usbMeter.svalue->text, "%lu/%lu", 
			tmpstat->f_bsize*tmpstat->f_blocks - tmpstat->f_bsize*tmpstat->f_bfree,
			tmpstat->f_bsize*tmpstat->f_blocks);
	if (usbMeter.value > usbMeter.valuePrev) {
		sprintf (usbMeter.svalueDiff->text, "+%9.lu", usbMeter.value - usbMeter.valuePrev);
		usbMeter.svalueDiff->textColor[0] = 0.8;
		usbMeter.svalueDiff->textColor[1] = 0.5;
		usbMeter.svalueDiff->textColor[2] = 0.5;
		usbMeter.svalueDiff->textColor[3] = 0.6;
	}
	else if (usbMeter.value < usbMeter.valuePrev) {
		sprintf (usbMeter.svalueDiff->text, "-%9.lu", usbMeter.valuePrev - usbMeter.value);
		usbMeter.svalueDiff->textColor[0] = 0.5;
		usbMeter.svalueDiff->textColor[1] = 0.5;
		usbMeter.svalueDiff->textColor[2] = 0.8;
		usbMeter.svalueDiff->textColor[3] = 0.6;
	}
	usbMeter.valuePrev = usbMeter.value;
	
	//sprintf ("Free: %lu\n", tmpstat->f_bsize*tmpstat->f_bfree);
	//sprintf ("Used: %lu\n", tmpstat->f_bsize*tmpstat->f_blocks - tmpstat->f_bsize*tmpstat->f_bfree);
/*	loadMeter.value = sinfo.loads[1] / 100000.0;
	if (loadMeter.value < loadMeter.valueMin) {
		loadMeter.valueMin = loadMeter.value;
		sprintf (loadMeter.svalueMin, "%f", loadMeter.valueMin);
	}
	if (loadMeter.value > loadMeter.valueMax) {
		loadMeter.valueMax = loadMeter.value;
		sprintf (loadMeter.svalueMax, "%f", loadMeter.valueMax);
	}
	sprintf (loadMeter.svalue, "%lu", sinfo.loads[1]);
	
	swapMeter.value = (sinfo.totalswap - sinfo.freeswap) / 100000000.0;
	if (swapMeter.value < swapMeter.valueMin) {
		swapMeter.valueMin = swapMeter.value;
		sprintf (swapMeter.svalueMin, "%f", swapMeter.valueMin);
	}
	if (swapMeter.value > swapMeter.valueMax) {
		swapMeter.valueMax = swapMeter.value;
		sprintf (swapMeter.svalueMax, "%f", swapMeter.valueMax);
	}
	sprintf (swapMeter.svalue, "%lu / %lu", sinfo.totalswap - sinfo.freeswap, sinfo.totalswap);
	
	highmemMeter.value = (sinfo.totalhigh - sinfo.freehigh) / 100000000.0;
	if (highmemMeter.value < highmemMeter.valueMin) {
		highmemMeter.valueMin = highmemMeter.value;
		sprintf (highmemMeter.svalueMin, "%f", highmemMeter.valueMin);
	}
	if (highmemMeter.value > highmemMeter.valueMax) {
		highmemMeter.valueMax = highmemMeter.value;
		sprintf (highmemMeter.svalueMax, "%f", highmemMeter.valueMax);
	}
	sprintf (highmemMeter.svalue, "%lu / %lu", sinfo.totalhigh - sinfo.freehigh, sinfo.totalhigh);
*/
}

void updateSensorsInfo (void) {
	FILE *file = fopen ("/root/.nexus/.sensors.output", "r");
	if (file == NULL) {
		printf ("updateSensorsInfo(): Cannot open /root/.nexus/.sensors.output\n");
		perror ("perror");
	}
	
	char c;
	int cnt = 0;
	int readingtemp = 0;
	char *cp = tmpstr;
	while (1) {
		c = fgetc (file);
		if (c == EOF) break;
		if (!readingtemp && c == '+') {readingtemp = 1;continue;}
		if (readingtemp) {
			if (c == ' ') { *cp = '\0'; break; }
			*cp = c;
			++cp;
		}
		++cnt;
		if (cnt >= 63) {printf ("updateSensorsInfo(): forced loop to break");break;}
	}
	sprintf (CPUtempstr, "%s%s%s", "CPU: ", tmpstr, "*C");
	
	fclose (file);
}


void *sensThreadFunc (void *argp) {
	int ret = 0;
	while (1) {
		ret = system ("sensors |grep CPU >/root/.nexus/.sensors.output");
		if (ret) {
			printf ("system(\"sensors\") return %d\n", ret);
			perror ("perror");
			options.showSensors = 0;
			struct Entry *entry = entryByID (ENTRY_OPTIONS_SHOWSENSORS);
			if (entry != NULL) entry->enabled = 0;
			return NULL;
		}
		updateSensorsInfo ();
		sleep (1);
	}
	
	return NULL;
}

