#ifndef MESH_H
#define MESH_H 1
#include <Ntype.h>

//#ifndef HAVE_SELECTID_H
//#include <selectID.h>
//#endif
#include "area.h"

FILE *inputFile, *outputFile;
#define MESH_NAME_SIZE 64

struct MeshNameList {
	char *name;
	int type;
	struct MeshNameList *prev, *next;
};

struct ptSelectID {
	int ID;
	struct ptSelectID *prev, *next;
};

struct plSelectID {
	int ID;
	struct ptSelectID ptSID, *lastptSID;
	struct plSelectID *prev, *next;
} *lastplSID;

struct SPoint {
	int ID;
	mbool selected;
	mbool hovered;
	float color[4];
	float x, y, z;
	struct SPoint *next, *prev;
};
typedef struct SPoint Point;

struct SPlane {
	int ID;
	mbool selected;
	mbool hovered;
	float color[4];
	//struct ptSelectID ptSID, *lastptSID;
	Point *point, *lastPoint;
	struct SPlane *prev, *next;
};
typedef struct SPlane Plane;

typedef struct Mesh {
	struct Mesh *prev, *next;
	int ID;
	char *name;
	float x, y, z, rotation;
	//struct plSelectID *plSID, *lastplSID;
	Plane *plane, *lastPlane;
} sMesh;
sMesh *drawnMesh;

typedef struct SScene {
	int ID;
	char *name;
	float x, y, z;	// center position
	float w, h, d;	// width height and depth size
	sMesh *dMesh, *lastMesh;
	sArea *area;
	struct SScene *prev, *next;
} Scene;
//typedef struct SScene Scene;

struct SWorld {
	int ID;
	char *name;
	Scene *scene, *lastScene;
	//Floor *firstFloor, *lastFloor;
	struct SWorld *prev, *next;
} *dWorld, *lastWorld;
typedef struct SWorld World;

void initDefaultMesh (void);
void initTruckMesh (void);
void destroyMesh (sMesh *mesh);

struct MeshNameList *createMeshNameList (void);
void destroyMeshNameList (struct MeshNameList *list);
void showMeshNameList (struct MeshNameList *list);

void saveMesh (sMesh *mesh);
sMesh *loadMesh (char *meshName);

Point *pointByID (int ID);
Plane *planeByID (int ID);
//Plane *planeByPointID (int ID);
sMesh *meshByID (int ID);
sMesh *meshByMeshName (char *meshName);
int typeByID (int ID);

World *addWorld (char *name);
Scene *addScene (World *world, char *name);
//Floor *addFloor (World *world)
//Scene *addScene (Floor *floor, char *name);
sMesh *addMesh (Scene *scene, char *name);
Plane *addPlane (sMesh *mesh);
Point *addPoint (Plane *plane, float x, float y, float z);
Point *addPointAfter (Plane *plane, Point *point);

void showMeshTree (sMesh *mesh);

#endif /* MESH_H */
