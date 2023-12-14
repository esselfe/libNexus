#ifndef CAMERA_H
#define CAMERA_H 1

typedef struct sCamera {
	float x, y, z,
			lx, ly, lz;
	float step;
	float angle, angleX;
	float minSpeed, maxSpeed;
} Camera;
extern Camera *cam; // rem, points to nexus.c::ProgramVariables{}::camera

extern float camMainmenuDefaultPos[9];
extern float camMainmenuLastPos[9];
extern float camEditorDefaultPos[9];
extern float camEditorLastPos[9];
extern float camMemoryDefaultPos[9];
extern float camMemoryLastPos[9];
extern signed short movingCamUD;
extern signed short movingCamLR;
extern signed short movingCamIO;
extern signed short lookUD;
extern signed short lookLR;

void initCamera (void);
void increaseCameraSpeed (void);
void decreaseCameraSpeed (void);
void setCameraPosition (float x, float y, float z, float lx, float ly, float lz);
void setCameraSpeed (float newSpeed);
void moveIO (signed short step);	/* the arg is 1 or -1 */
void moveUD (signed short step);	/* idem */
void moveLR (signed short step);	/* idem */
void look_UD (signed short dir);
void turnCamUD ();
void turnCamLR ();
void moveCamIO (signed short dir);
void moveCamLR (signed short dir);
void moveCamUD (signed short dir);
void storeCameraPosition (Camera *cam);		// save position according to 'mode'
void restoreCameraPosition (Camera *cam);	// set idem

#include <pthread.h>
extern pthread_t camThread;
void *camThreadFunc (void *argp);

#endif /* CAMERA_H */
