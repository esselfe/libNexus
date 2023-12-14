#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

#include "hud.h"
#include "config.h"
#include "drawing.h"
#include "events.h"
#include "menu.h"
#include "mesh.h"
#include "meshEditing.h"
#include "messages.h"
#include "libNexus.h"

struct MeshNameList *createMeshNameList (void) {
	char *cwd = malloc (strlen ("mesh") + 1);
	
	printf ("createMeshNameList(): reading from /root/.nexus/mesh\n");
	
	DIR *dir = opendir ("/root/.nexus/mesh");
	if (dir == NULL) {
		printf ("listExistingMeshes(): Cannot open dir named %s\n", cwd);
		perror ("perror() says");
		free (cwd);
		closedir (dir);
		return NULL;
	}
	free (cwd);
	
	struct dirent *dir2;
	struct MeshNameList *list = malloc (sizeof (struct MeshNameList));
	list->name = NULL;
	list->prev = NULL;
	list->next = NULL;
	struct MeshNameList *list2 = list;
	while (1) {
		dir2 = readdir (dir);
		if (dir2 == NULL) break;
		else if (strcmp (dir2->d_name, ".") == 0) continue;
		else if (strcmp (dir2->d_name, "..") == 0) continue;
		
		printf ("\t%s, type %d\n", dir2->d_name, dir2->d_type);
		
		if (list2->name != NULL) {
			list2->next = malloc (sizeof (struct MeshNameList));
			list2->next->prev = list;
			list2->next->next = NULL;
			list2 = list2->next;
		}
		list2->name = malloc (strlen (dir2->d_name) + 1);
		sprintf (list2->name, "%s", dir2->d_name);
		list2->type = dir2->d_type;
	}
	
	closedir (dir);
	return list;
}

void showMeshNameList (struct MeshNameList *list) {
	if (list == NULL) {
		printf ("showMeshNameList(): list is NULL, abort\n");
		return;
	}
	printf ("showMeshNameList(): started\n");
	struct MeshNameList *list2 = list;
	while (1) {
		printf ("\t%s, type %d\n", list2->name, list2->type);
		if (list2->next == NULL) break;
		list2 = list2->next;
	}
	printf ("showMeshNameList(): done\n");
}

void destroyMeshNameList (struct MeshNameList *list) {
	if (list == NULL) {
		printf ("destroyMeshNameList(): list is NULL, abort\n");
		return;
	}
	struct MeshNameList *list2 = list;
	while (1) {	// set the pointer to the last element
		if (list2->next == NULL) break;
		list2 = list2->next;
	}
	while (1) { //	destroy from the last
		free (list2->name);
		list2->type = 0;
		if (list2->prev == NULL) break;
		list2 = list2->prev;
		list2->next->prev = NULL;
		free (list2->next);
		list2->next = NULL;
	}
	free (list);
}

// From Mesh structure to file as ordered data with special delims for mesh,
// plane and point descriptions (#, $, @)
void saveMesh (sMesh *mesh) {
	char *filename = malloc (MESH_NAME_SIZE+4);
	sprintf (filename, "/root/.nexus/mesh/%s.mesh", mesh->name);
	
	outputFile = fopen (filename, "w");
	if (outputFile == NULL) {
		printf ("Err saving mesh named %s\n", mesh->name);
		perror ("perror");
		free (filename);
		return;
	}
	else { 
		sprintf (msgbuf, "Saving mesh named %s...", mesh->name);
		addMsg (0, msgbuf);
	}
	
	// Format recognition
	fprintf (outputFile, "!NexusMesh!");
	
	// Write mesh data
	fprintf (outputFile, "#%s,%.2f,%.2f,%.2f,%.2f", 
			 mesh->name, mesh->x, mesh->y, mesh->z, mesh->rotation);
	
	Plane *curPlane = mesh->plane;
	Point *curPoint = curPlane->point;
	while (1) {	// All planes
		fprintf (outputFile, "$%d,%d,%.2f,%.2f,%.2f", 
				curPlane->ID, curPlane->selected, 
	 			curPlane->color[0], curPlane->color[1], curPlane->color[2]);
		while (1) {	// All plane's point
			fprintf (outputFile, "@%d,%d,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f",
					curPoint->ID, curPoint->selected,
	  				curPoint->color[0],	curPoint->color[1],	curPoint->color[2],curPoint->color[3],
					curPoint->x, curPoint->y, curPoint->z);
			
			if (curPoint->next == NULL) break;
			else curPoint = curPoint->next;
		}
		if (curPlane->next == NULL) break;
		else {
			curPlane = curPlane->next;
			curPoint = curPlane->point;
		}
	}
	
	fclose (outputFile);
}

// Parse a file to reconstruct a mesh struct (btw it will be destroyed 
long fileSize = 1024;
sMesh *loadMesh (char *meshName) {
	char c = 0;	// Parsing
	int cnt = 0;
	int wcnt = 0;
	int cntDataField = 0;	// incr through all the mesh's data, planes and points
	
	char *filename = malloc (MESH_NAME_SIZE+10);
	memset (filename, 0, MESH_NAME_SIZE+9);
	sprintf (filename, "/root/.nexus/mesh/%s.mesh", meshName);
	inputFile = fopen (filename, "r");
	if (inputFile == NULL) {
		printf ("Err loading mesh named %s\n", meshName);
		printf ("while loading filename :%s:\n", filename);
		perror ("perror");
		free (filename);
		return NULL;
	}
	
	fseek (inputFile, 0, SEEK_END);
	fileSize = ftell (inputFile);
	fseek (inputFile, 0, SEEK_SET);
	
	// Prepare parsing
	// todo: -build mesh from block of file data instead of the whole file.
	// 			The mesh file could well get way bigger than the 512 bytes or so
	// 			that the actual meshes take... 1k block?
	char *w = malloc (fileSize);
	memset (w, 0, fileSize);
	
	// Validate format
	w[cnt] = fgetc (inputFile);
	if (w[cnt] != '!') {
		printf ("loadMesh(): invalid format or empty file\n");
		free (w);
		free (filename);
		fclose (inputFile);
		return NULL;
	}
	++cnt;
	while (1) {
		w[cnt] = fgetc (inputFile);
		if (w[cnt] == EOF) break;
		if (cnt == 10) {
			if (strcmp ("!NexusMesh!", w) != 0) {
				printf ("File format tag not found, aborting\n");
				free (w);
				free (filename);
				fclose (inputFile);
				return NULL;
			}
			else {
				//printf ("Format ok\n");
				break;
			}
		}
		++cnt;
	}
	
	// Parse the file into the mesh data struct
	cnt = 0;
	char *w2 = malloc (fileSize);
	memset (w2, 0, fileSize);
	wcnt = 0;
	mbool readMesh = 0;
	mbool lastMeshData = 0;
	mbool readPlane = 0;
	mbool lastPlaneData = 0;
	mbool readPoint = 0;
	mbool lastPointData = 0;
	mbool nextIsPlane = 0;
	mbool nextIsPoint = 0;
	mbool done = 0;
	
	sMesh *mesh = meshByMeshName (meshName);
	if (mesh != NULL) {
		printf ("loadMesh(): Found mesh already loaded\n");
		return mesh;
	}
	mesh = addMesh (dWorld->scene, "blankMesh");
	Plane *curPlane = mesh->plane;
	Point *curPoint = curPlane->point;
	
	while (!done) {
		memset (w2, 0, fileSize);
		wcnt = 0;
		// for data field
		while (!done) {
			c = fgetc (inputFile);
			if (c == '#') {
				readMesh = 1;
				memset (w2, 0, fileSize);
				wcnt = 0;
				continue;
			}
			else if (c == ',') {
				break;
			}
			else if (c == '$') {
				if (readMesh) {
					lastMeshData = 1;
				}
				else if (readPoint) {
					lastPointData = 1;
					nextIsPlane = 1;
				}
				break;
			}
			else if (c == '@') {
				if (readPlane) {
					lastPlaneData = 1;
				}
				else if (readPoint) {
					lastPointData = 1;
					nextIsPoint = 1;
				}
				break;
			}
			else if (c == EOF) {
				lastPointData = 1;
				done = 1;
				break;
			}
			sprintf (w2, "%s%c", w2, c);
			++wcnt;
		}
		wcnt = 0;
		if (readMesh) {
			switch (cntDataField) {
				case 0:
					sprintf (mesh->name, "%s", w2);
					break;
				case 1:
					mesh->x = atof (w2);
					break;
				case 2:
					mesh->y = atof (w2);
					break;
				case 3:
					mesh->z = atof (w2);
					break;
				case 4:
					mesh->rotation = atof (w2);
					break;
			}
			if (lastMeshData) {
				lastMeshData = 0;
				readMesh = 0;
				readPlane = 1;
				cntDataField = 0;
				continue;
			}
			else {
				++cntDataField;
				continue;
			}
		}
		else if (readPlane) {
			switch (cntDataField) {
				case 0:
					//curPlane->ID = atoi (w2);	// IDs are now dynamic
					break;
				case 1:
					curPlane->selected = atoi (w2);
					break;
				case 2:
					curPlane->color[0] = atof (w2);
					break;
				case 3:
					curPlane->color[1] = atof (w2);
					break;
				case 4:
					curPlane->color[2] = atof (w2);
					break;
			}
			curPlane->hovered = 0;
			if (lastPlaneData) {
				curPlane->color[3] = planeDefaultColor[3];
				lastPlaneData = 0;
				readPlane = 0;
				readPoint = 1;
				cntDataField = 0;
				curPoint = curPlane->lastPoint;
			}
			else ++cntDataField;
		}
		else if (readPoint) {
			switch (cntDataField) {
				case 0:
					//curPoint->ID = atoi (w2);
					break;
				case 1:
					curPoint->selected = atoi (w2);
					break;
				case 2:
					curPoint->color[0] = pointDefaultColor[0];//atof (w2);
					break;
				case 3:
					curPoint->color[1] = pointDefaultColor[1];//atof (w2);
					break;
				case 4:
					curPoint->color[2] = pointDefaultColor[2];//atof (w2);
					break;
				case 5:
					curPoint->color[3] = pointDefaultColor[3];//atof (w2);
					break;
				case 6:
					curPoint->x = atof (w2);
					break;
				case 7:
					curPoint->y = atof (w2);
					break;
				case 8:
					curPoint->z = atof (w2);
					break;
			}
			if (lastPointData) {
				curPoint->hovered = 0;
				if (done) {
					cntDataField = 0;
					lastPointData = 0;
					readPoint = 0;
					break;
				}
				else if (nextIsPoint) {
					cntDataField = 0;
					lastPointData = 0;
					nextIsPoint = 0;
					curPoint = addPoint (curPlane, 0.0,1.0,0.0);
				}
				else if (nextIsPlane) {
					readPlane = 1;
					cntDataField = 0;
					lastPointData = 0;
					nextIsPlane = 0;
					curPlane = addPlane (mesh);
					curPoint = curPlane->lastPoint;
				}
			}
			else
				++cntDataField;
		}
	}
	
	free (w);
	free (w2);
	free (filename);
	fclose (inputFile);
	sprintf (msgbuf2, "Loaded mesh ::%s:: ID %d\n", mesh->name, mesh->ID);
	addMsg (0, msgbuf2);
	return mesh;
}

Point *pointByID (int ID) {
	Plane *curPlane = drawnMesh->plane;
	Point *curPoint = curPlane->point;
	while (1) {
		while (1) {
			if (curPoint->ID == ID) {
				return curPoint;
			}
			else if (curPoint->next != NULL && (curPoint->next->ID > curPoint->ID)) 
				curPoint = curPoint->next;
			else break;
		}
		
		if (curPlane->next == NULL) break;
		else {
			curPlane = curPlane->next;
			curPoint = curPlane->point;
		}
	}
	return NULL;
}

Plane *planeByID (int ID) {
	Plane *curPlane = drawnMesh->plane;
	while (1) {
		if (curPlane->ID == ID) return curPlane;
		if (curPlane->next != NULL) curPlane = curPlane->next;
		else break;
	}
	return NULL;
}

sMesh *meshByID (int ID) {
	sMesh *mesh = dWorld->scene->dMesh;
	while (1) {
		if (mesh->ID == ID) {
			return mesh;
		}
		if (mesh->next == NULL) break;
		else mesh = mesh->next;
	}
	return NULL;
}

sMesh *meshByMeshName (char *meshName) {
	sMesh *mesh = dWorld->scene->dMesh;
	while (1) {
		if (strcmp (mesh->name, meshName) == 0) return mesh;
		if (mesh->next == NULL) break;
		else mesh = mesh->next;
	}
	return NULL;
}

int typeByID (int ID) {
	Plane *plane = drawnMesh->plane;
	Point *point = plane->point;
	while (1) {
		if (plane->ID == ID) return SID_TYPE_PLANE;
		while (1) {
			if (point->ID == ID) return SID_TYPE_POINT;
			if (point->next != NULL) {
				point = point->next;
			}
			else break;
		}
		if (plane->next != NULL) {
			plane = plane->next;
			point = plane->point;
		}
		else break;
	}
	return 0;
}

World *addWorld (char *name) {
	World *world;
	if (lastWorld == NULL) {
		dWorld = malloc (sizeof (World));
		world = dWorld;
		world->ID = 1;	// 1 - 10 are world IDs
		world->prev = NULL;
	}
	else {
		world = lastWorld;
		world->next = malloc (sizeof (World));
		world->next->prev = world;
		world = world->next;
		world->ID = world->prev->ID + 1;
	}
	world->next = NULL;
	lastWorld = world;
	world->name = malloc (sizeof(strlen(name)+1));
	sprintf (world->name, "%s", name);
	//world->firstFloor = NULL;
	//world->lastFloor = NULL;
	world->scene = NULL;
	world->lastScene = NULL;
	
	return world;
}
/*
Floor *addFloor (World *world) {
	Floor *floor = NULL;
	if (dWorld->firstFloor == NULL) {
		dWorld->firstFloor = malloc (sizeof (Floor));
		floor = dWorld->firstFloor;
		dWorld->lastFloor = floor;
		floor->prev = NULL;
		floor->next = NULL;
		floor->ID = 1;
	}
	
	return NULL;
}*/

Scene *addScene (World *world, char *name) {
	Scene *scene;
	if (world->lastScene == NULL) {
		world->scene = malloc (sizeof (Scene));
		scene = world->scene;
		scene->ID = world->ID * 100000;
		scene->z = 0.0;
		scene->prev = NULL;
	}
	else {
		scene = world->lastScene;
		scene->next = malloc (sizeof (Scene));
		scene->next->prev = scene;
		scene = scene->next;
		scene->ID = scene->prev->ID +100000;
		scene->z = scene->prev->z + 20.0;
	}
	scene->next = NULL;
	world->lastScene = scene;
	scene->name = malloc (strlen (name)+1);
	sprintf (scene->name, "%s", name);
	scene->lastMesh = NULL;
	scene->dMesh = addMesh (scene, "dummyMesh");
	scene->x = 0.0;
	scene->y = 0.0;
	scene->w = 20.0;
	scene->h = 5.0;
	scene->d = 20.0;
	
	return scene;
}

sMesh *addMesh (Scene *scene, char *name) {
	sMesh *mesh;
	if (scene->lastMesh == NULL) {
		scene->dMesh = malloc (sizeof (sMesh));
		mesh = scene->dMesh;
		mesh->ID = scene->ID + 10000;
		mesh->x = 0.0;
		mesh->prev = NULL;
	}
	else {
		mesh = scene->lastMesh;
		mesh->next = malloc (sizeof (sMesh));
		mesh->next->prev = mesh;
		mesh = mesh->next;
		mesh->ID = mesh->prev->ID + 10000;
		mesh->x = mesh->prev->x + 4.0;
	}
	mesh->next = NULL;
	scene->lastMesh = mesh;
	mesh->name = malloc (strlen (name)+1);
	sprintf (mesh->name, "%s", name);
	mesh->y = 0.0;
	mesh->z = 0.0;
	mesh->rotation = 0.0;
	mesh->lastPlane = NULL;
	mesh->plane = addPlane (mesh);
	
	return mesh;
}

Plane *addPlane (sMesh *mesh) {
	Plane *plane;
	if (mesh->lastPlane == NULL) {
		mesh->plane = malloc (sizeof (Plane));
		plane = mesh->plane;
		plane->ID = mesh->ID + 1000;
		plane->prev = NULL;
	}
	else {
		plane = mesh->lastPlane;
		plane->next = malloc (sizeof (Plane));
		plane->next->prev = plane;
		plane = plane->next;
		plane->ID = plane->prev->ID + 1000;
	}
	plane->next = NULL;
	mesh->lastPlane = plane;
	plane->selected = 0;
	plane->hovered = 0;
	plane->color[0] = planeDefaultColor[0];
	plane->color[1] = planeDefaultColor[1];
	plane->color[2] = planeDefaultColor[2];
	plane->color[3] = planeDefaultColor[3];
	plane->lastPoint = NULL;
	plane->point = addPoint (plane, 0.0,0.0,0.0);
	
	return plane;
}

Point *addPoint (Plane *plane, float x, float y, float z) {
	Point *point;
	if (plane->lastPoint == NULL) {
		plane->point = malloc (sizeof (Point));
		point = plane->point;
		point->ID = plane->ID + 100;
		point->prev = NULL;
	}
	else {
		point = plane->lastPoint;
		point->next = malloc (sizeof (Point));
		point->next->prev = point;
		point = point->next;
		point->ID = point->prev->ID + 1;
	}
	point->next = NULL;
	plane->lastPoint = point;
	point->selected = 0;
	point->hovered = 0;
	point->color[0] = pointDefaultColor[0];
	point->color[1] = pointDefaultColor[1];
	point->color[2] = pointDefaultColor[2];
	point->color[3] = pointDefaultColor[3];
	point->x = x;
	point->y = y;
	point->z = z;
	
	return point;
}

Point *addPointAfter (Plane *plane, Point *point) {
	Point *curPoint = malloc (sizeof (Point));
	if (point->next == NULL) {
		curPoint->x = (point->x + plane->point->x)/2;
		curPoint->y = (point->y + plane->point->y)/2;
		curPoint->z = (point->z + plane->point->z)/2;
		curPoint->next = NULL;
	}
	else {
		curPoint->x = (point->x + point->next->x)/2;
		curPoint->y = (point->y + point->next->y)/2;
		curPoint->z = (point->z + point->next->z)/2;
		
		// Decay the selectIDs after *point
		Point *curPoint3 = point->next;
		while (1) {
			++curPoint3->ID;
			if (curPoint3->next == NULL) break;
			else curPoint3 = curPoint3->next;
		}
		
		curPoint->next = point->next;
		point->next->prev = curPoint;
	}
	curPoint->ID = point->ID + 1;
	curPoint->selected = 0;
	curPoint->hovered = 0;
	curPoint->color[0] = pointDefaultColor[0];
	curPoint->color[1] = pointDefaultColor[1];
	curPoint->color[2] = pointDefaultColor[2];
	curPoint->color[3] = pointDefaultColor[3];
	curPoint->prev = point;
	point->next = curPoint;
	plane->lastPoint = curPoint;
	return curPoint;
}

void showMeshTree (sMesh *mesh) {
	if (mesh == NULL) {
		printf ("showMeshTree(): mesh is NULL, abort\n");
		return;
	}
	printf ("showMeshTree (): showing mesh ::%s:: ID %d\n", mesh->name, mesh->ID);
	
	if (mesh->plane == NULL) {
		printf ("mesh->plane == NULL\n");
		return;
	}
	Plane *curPlane = mesh->plane;
	if (mesh->plane->point == NULL) {
		printf ("mesh->plane->point == NULL\n");
		return;
	}
	Point *curPoint = curPlane->point;
	while (1) { // All planes
		printf ("\tcurPlane->selectID: %d\n", curPlane->ID);
		while (1) { // All plane's points
			printf ("\t\tcurPoint->selectID: %d\n", curPoint->ID);
			if (curPoint->next == NULL) break;
			else curPoint = curPoint->next;
		}
		if (curPlane->next == NULL) break;
		else curPlane = curPlane->next;
	}
	printf ("showMeshTree(): done\n");
}
