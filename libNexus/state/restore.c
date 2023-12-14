#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <camera.h>

// These functions will read/write nexus.state
// They operate on libNexus.h::ProgramVariables{} to restore program state
// according to the autoSave settings in nexus.cfg

int saveState (char *filename) {
	printf ("Saving state, filename: %s\n", filename);
	return 0;
}

int loadState (char *filename) {
	printf ("Loading state, filename: %s\n", filename);
	return 0;
}

void saveLastState (void) {
	FILE *file = fopen ("/root/.nexus/.nexus.state", "wb");
	if (file == NULL) {
		printf ("saveState(): cannot open ~/.nexus/.nexus.state\n");
		perror ("perror");
		return;
	}

	printf ("saveLastState(): Saving to ~/.nexus/.nexus.state\n");
//hudUpdateClock ();
	time_t saveTime = time (NULL);
	char *tmpbuf = ctime (&saveTime);
	char *tmpbuf2 = malloc (strlen (tmpbuf) + strlen ("last save: ") +1);
	sprintf (tmpbuf2, "last save: %s\n", tmpbuf);
	fputs (tmpbuf2, file);
	free (tmpbuf2);
	
	tmpbuf2 = malloc (1024);
	sprintf (tmpbuf2, "%.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f", 
					cam->x, cam->y, cam->z,
					cam->lx, cam->ly, cam->lz,  cam->step, cam->angle, cam->angleX,
					cam->minSpeed, cam->maxSpeed);
	fputs (tmpbuf2, file);
	free (tmpbuf2);

	fclose (file);
}

void loadLastState (void) {
	printf ("loadLastState(): Loading from ~/.nexus/.nexus.state\n");
	
	FILE *file = fopen ("/root/.nexus/.nexus.state", "rb");
	if (file == NULL) {
		printf ("loadState(): cannot open nexus.state\n");
		perror ("perror");
		return;
	}
	
	
	
	fclose (file);
}

