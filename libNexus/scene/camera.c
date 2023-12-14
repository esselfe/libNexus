#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <GL/glut.h>

#include "camera.h"
#include "drawing.h"
#include "messages.h"
#include "libNexus.h"
#include "state.h"

Camera *cam = NULL;
								// x,y,z, 			lx,ly,lz,			step,angle,angleX
float camMainmenuDefaultPos[9] = {0.0, 10.0, 50.0, 	0.0, 0.0, 0.0,	0.3,0.0,0.0};
float camMainmenuLastPos[9] = {	0.0, 10.0, 50.0, 	0.0, 0.0, 0.0, 	0.3,0.0,0.0};
float camEditorDefaultPos[9] = {0.65, 0.6, 1.75, 	0.65, 0.6, -1.0, 	0.15,0.0,0.0};
float camEditorLastPos[9] = {	0.65, 0.6, 1.75, 	0.65, 0.6, -1.0, 	0.15,0.0,0.0};
float camMemoryDefaultPos[9] = {0.0,2.5,7.0, 		0.0,2.5,-7.0,		0.3,0.0,0.0};
float camMemoryLastPos[9] = {	0.0,2.5,7.0, 		0.0,2.5,-7.0,		0.3,0.0,0.0};
float camCodeparseDefaultPos[9] = {0.5,-9.5,1.75, 	0.5,-9.5,-1.75, 	0.3,0.0,0.0};
float camCodeparseLastPos[9] = {0.5,-9.5,1.75,		0.5,-9.5,-1.75, 	0.3,0.0,0.0};
float camBrowserDefaultPos[9] = {0.0,0.6,5.25, 		0.0,0.6,-5.25, 	0.3,0.0,0.0};
float camBrowserLastPos[9] = {0.0,0.6,5.25, 		0.0,0.6,-5.25, 	0.3,0.0,0.0};
float camRaceDefaultPos[9] = {0.0, 2.6, 8.25, 		0.0, 2.6, -8.25,	0.25,0.0,0.0};
float camRaceLastPos[9] = {	0.65, 2.6, 8.25, 		0.65, 2.6, -8.25, 	0.25,0.0,0.0};
float camBufferDefaultPos[9] = {0.0,2.6,7.0,			0.0,2.6,7.0,			0.25,0.0,0.0};
float camBufferLastPos[9] = {0.0,2.6,7.0,			0.0,2.6,7.0,			0.25,0.0,0.0};
signed short movingCamUD = 0;
signed short movingCamLR = 0;
signed short movingCamIO = 0;
signed short lookUD = 0;
signed short lookLR = 0;

void initCamera (void) {
//	cam = malloc (sizeof (struct Camera));
	cam = &pv.camera;
	cam->x = 0.0;
	cam->y = 2.0;
	cam->z = 6.5;
	cam->lx = 0.0;
	cam->ly = 1.5;
	cam->lz = -3.9;
	cam->step = 1.0;
	cam->angle = 0.0;
	cam->angleX = 0.0;
	cam->minSpeed = 0.05;
	cam->maxSpeed = 2.0;
	restoreCameraPosition (cam);
	
	pthread_create (&camThread, NULL, camThreadFunc, NULL);
	pthread_detach (camThread);
}

void increaseCameraSpeed (void) {
	if (cam->step <= 0.0) {
		cam->step += 0.1;
	}
	else if (cam->step <= 1.0)
		cam->step += 0.1;
	else if (cam->step <= 1.0)
		cam->step += 0.5;
	else if (cam->step <= 10.0)
		cam->step += 1.0;

	sprintf (msgbuf2, "camspeed: %.2f", cam->step);
	addMsg (0, msgbuf2);
}

void decreaseCameraSpeed (void) {
	if (cam->step <= 0.0)
		cam->step = 0.1;
	else if (cam->step <= 1.0)
		cam->step -= 0.1;
	else if (cam->step <= 10.0)
		cam->step -= 1.0;

	sprintf (msgbuf2, "camspeed: %.2f", cam->step);
	addMsg (0, msgbuf2);
}

void setCameraPosition (float x, float y, float z, float lx, float ly, float lz) {
	cam->x = x;
	cam->y = y;
	cam->z = z;
	cam->lx = lx;
	cam->ly = ly;
	cam->lz = lz;
	//gluLookAt (cam->x, cam->y, cam->z,
	//		   cam->lx, cam->ly, cam->lz,
	 // 0.0, 1.0, 0.0);
}

void setCameraSpeed (float newSpeed) {
	cam->step = newSpeed;
	sprintf (msgbuf2, "set camera speed to: %.2f", cam->step);
	addMsg (0, msgbuf2);
}

void look_UD (signed short dir) {
	cam->ly += (dir > 0) ? cam->step/16.0 : -cam->step/16.0;
}

void turnCamUD () {
	cam->ly = sin (cam->angleX/5.73) + cam->y;
	cam->lz = -cos (cam->angleX/5.73) + cam->z;
	if (cam->angleX >= 36.0) cam->angleX = 0.0;
	else if (cam->angleX <= 0.0) cam->angleX = 36.0;
}

/* show a compass? */
void turnCamLR () {
	cam->lx = sin (cam->angle/5.73) + cam->x;
	cam->lz = -cos (cam->angle/5.73) + cam->z;
	if (cam->angle >= 36.0) cam->angle = 0.0;
	else if (cam->angle <= 0.0) cam->angle = 36.0;
}

void moveCamIO (signed short dir) {
	if (dir > 0) {
		cam->z -= cam->step;
		cam->lz -= cam->step;
	}
	else {
		cam->z += cam->step;
		cam->lz += cam->step;
	}
}

void moveCamLR (signed short dir) {
	if (dir > 0) {
		cam->x += cam->step;
		cam->lx += cam->step;
	}
	else {
		cam->x -= cam->step;
		cam->lx -= cam->step;
	}
}

void moveCamUD (signed short dir) {
	if (dir < 0) {
		cam->y += cam->step;
		cam->ly += cam->step;
	}
	else {
		cam->y -= cam->step;
		cam->ly -= cam->step;
	}
}

void storeCameraPosition (Camera *cam) {
	if (mode == MODE_EDITOR) {
		camEditorLastPos[0] = cam->x;
		camEditorLastPos[1] = cam->y;
		camEditorLastPos[2] = cam->z;
		camEditorLastPos[3] = cam->lx;
		camEditorLastPos[4] = cam->ly;
		camEditorLastPos[5] = cam->lz;
		camEditorLastPos[6] = cam->step;
		camEditorLastPos[7] = cam->angle;
		camEditorLastPos[8] = cam->angleX;
	}
	else if (mode == MODE_MAINMENU) {
		camMainmenuLastPos[0] = cam->x;
		camMainmenuLastPos[1] = cam->y;
		camMainmenuLastPos[2] = cam->z;
		camMainmenuLastPos[3] = cam->lx;
		camMainmenuLastPos[4] = cam->ly;
		camMainmenuLastPos[5] = cam->lz;
		camMainmenuLastPos[6] = cam->step;
		camMainmenuLastPos[7] = cam->angle;
		camMainmenuLastPos[8] = cam->angleX;
	}
	else if (mode == MODE_MEM) {
		camMemoryLastPos[0] = cam->x;
		camMemoryLastPos[1] = cam->y;
		camMemoryLastPos[2] = cam->z;
		camMemoryLastPos[3] = cam->lx;
		camMemoryLastPos[4] = cam->ly;
		camMemoryLastPos[5] = cam->lz;
		camMemoryLastPos[6] = cam->step;
		camMemoryLastPos[7] = cam->angle;
		camMemoryLastPos[8] = cam->angleX;
	}
	else if (mode == MODE_BROWSER) {
		camBrowserLastPos[0] = cam->x;
		camBrowserLastPos[1] = cam->y;
		camBrowserLastPos[2] = cam->z;
		camBrowserLastPos[3] = cam->lx;
		camBrowserLastPos[4] = cam->ly;
		camBrowserLastPos[5] = cam->lz;
		camBrowserLastPos[6] = cam->step;
		camBrowserLastPos[7] = cam->angle;
		camBrowserLastPos[8] = cam->angleX;
	}
	else if (mode == MODE_BUFFER) {
		camBufferLastPos[0] = cam->x;
		camBufferLastPos[1] = cam->y;
		camBufferLastPos[2] = cam->z;
		camBufferLastPos[3] = cam->lx;
		camBufferLastPos[4] = cam->ly;
		camBufferLastPos[5] = cam->lz;
		camBufferLastPos[6] = cam->step;
		camBufferLastPos[7] = cam->angle;
		camBufferLastPos[8] = cam->angleX;
	}
}

void restoreCameraPosition (Camera *cam) {
	if (mode == MODE_EDITOR) {
		cam->x = camEditorLastPos[0];
		cam->y = camEditorLastPos[1];
		cam->z = camEditorLastPos[2];
		cam->lx = camEditorLastPos[3];
		cam->ly = camEditorLastPos[4];
		cam->lz = camEditorLastPos[5];
		cam->step = camEditorLastPos[6];
		cam->angle = camEditorLastPos[7];
		cam->angleX = camEditorLastPos[8];
	}
	else if (mode == MODE_MAINMENU) {
		cam->x = camMainmenuLastPos[0];
		cam->y = camMainmenuLastPos[1];
		cam->z = camMainmenuLastPos[2];
		cam->lx = camMainmenuLastPos[3];
		cam->ly = camMainmenuLastPos[4];
		cam->lz = camMainmenuLastPos[5];
		cam->step = camMainmenuLastPos[6];
		cam->angle = camMainmenuLastPos[7];
		cam->angleX = camMainmenuLastPos[8];
	}
	else if (mode == MODE_MEM) {
		cam->x = camMemoryLastPos[0];
		cam->y = camMemoryLastPos[1];
		cam->z = camMemoryLastPos[2];
		cam->lx = camMemoryLastPos[3];
		cam->ly = camMemoryLastPos[4];
		cam->lz = camMemoryLastPos[5];
		cam->step = camMemoryLastPos[6];
		cam->angle = camMemoryLastPos[7];
		cam->angleX = camMemoryLastPos[8];
	}
	else if (mode == MODE_BROWSER) {
		cam->x = camBrowserLastPos[0];
		cam->y = camBrowserLastPos[1];
		cam->z = camBrowserLastPos[2];
		cam->lx = camBrowserLastPos[3];
		cam->ly = camBrowserLastPos[4];
		cam->lz = camBrowserLastPos[5];
		cam->step = camBrowserLastPos[6];
		cam->angle = camBrowserLastPos[7];
		cam->angleX = camBrowserLastPos[8];
	}
	else if (mode == MODE_BUFFER) {
		cam->x = camBufferLastPos[0];
		cam->y = camBufferLastPos[1];
		cam->z = camBufferLastPos[2];
		cam->lx = camBufferLastPos[3];
		cam->ly = camBufferLastPos[4];
		cam->lz = camBufferLastPos[5];
		cam->step = camBufferLastPos[6];
		cam->angle = camBufferLastPos[7];
		cam->angleX = camBufferLastPos[8];
	}
	return;
}

void *camThreadFunc (void *argp) { nice (-25);
	while (1) {
		if (movingCamUD < 0) {		
			needRedraw = 1; moveCamUD (1);	}
		else if (movingCamUD > 0) {	
			needRedraw = 1; moveCamUD (-1);	}
		else if (movingCamLR < 0) {	
			needRedraw = 1; moveCamLR (-1);	}
		else if (movingCamLR > 0) {	
			needRedraw = 1; moveCamLR (1);	}
		else if (movingCamIO < 0) {	
			needRedraw = 1; moveCamIO (-1);	}
		else if (movingCamIO > 0) {	
			needRedraw = 1; moveCamIO (1);	}
		else if (lookUD < 0) {		
			needRedraw = 1; cam->angleX -= cam->step;		turnCamUD ();  }
		else if (lookUD > 0) {
			needRedraw = 1; cam->angleX += cam->step;		turnCamUD ();  }
		else if (lookLR < 0) { 
			needRedraw = 1; cam->angle -= cam->step; 		turnCamLR ();  }
		else if (lookLR > 0) { 
			needRedraw = 1; cam->angle += cam->step;		turnCamLR ();  }
		
		//if (movingSelectionUD != 0 || movingSelectionLR != 0 || movingSelectionIO != 0) {
		//	moveSelection ();
		//	needRedraw = 1;
		//}
		usleep (31250);
	}
	return NULL;
}

