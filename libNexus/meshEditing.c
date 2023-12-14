#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "drawing.h"
#include "events.h"
#include "mesh.h"

float pointDefaultColor[4] = {0.6, 0.8, 0.7, 1.0};
float pointSelectColor[4] = {0.65, 0.4, 0.2, 1.0};
float lineDefaultColor[4] = {0.0, 0.75, 0.0, 1.0};
float lineSelectColor[4] = {0.75, 0.5, 0.3, 1.0};
float planeDefaultColor[4] = {0.2, 0.3, 0.2, 0.6};
float planeSelectColor[4] = {0.65, 0.4, 0.2, 0.6};
float selectionMovingStep = 0.025;
float selectionMaxMovingStep = 1.0;
signed short movingSelectionUD = 0;
signed short movingSelectionLR = 0;
signed short movingSelectionIO = 0;

void moveSelection (void) {
	Plane *curPlane = drawnMesh->plane;
	Point *curPoint = curPlane->point;
	while (1) {
		while (1) {
			if (curPoint->selected) {
				if (movingSelectionIO < 0)
					curPoint->z -= selectionMovingStep;
				else if (movingSelectionIO > 0)
					curPoint->z += selectionMovingStep;
				
				if (movingSelectionUD < 0)
					curPoint->y -= selectionMovingStep;
				else if (movingSelectionUD > 0)
					curPoint->y += selectionMovingStep;
				
				if (movingSelectionLR < 0)
					curPoint->x -= selectionMovingStep;
				else if (movingSelectionLR > 0)
					curPoint->x += selectionMovingStep;
			}
			
			if (curPoint->next == NULL) break;
			else curPoint = curPoint->next;
		}
		
		if (curPlane->next == NULL) break;
		else {
			curPlane = curPlane->next;
			curPoint = curPlane->point;
		}
	}
}

void hoverPlane (Plane *plane, int hoverState) {
	if (hoverState == 0) {
		curHoveredPlane = NULL;
	}
	else {
		curHoveredPlane = plane;
	}
	plane->hovered = hoverState;

	if (debug.mesh) printf ("hoverPlane(): set plane ID %d hovered%d\n", plane->ID, plane->hovered);
	needRedraw = 1;
}

void hoverPoint (Point *point, int hoverState) {
	if (hoverState == 0) {
		curHoveredPoint = NULL;
	}
	else {
		curHoveredPoint = point;
	}
	point->hovered = hoverState;

	if (debug.mesh) printf ("hoverPoint(): set point ID %d hovered%d\n", point->ID, point->hovered);
	needRedraw = 1;
}

void selectPoint (Point *point) {
	if (point->selected) {
		point->selected = 0;
		
		curSelectedPoint = NULL;
		point->color[0] = pointDefaultColor[0];
		point->color[1] = pointDefaultColor[1];
		point->color[2] = pointDefaultColor[2];
		point->color[3] = pointDefaultColor[3];
	}
	else {
		point->selected = 1;
		if (curSelectedPoint != NULL)
			lastcurSelectedPoint = curSelectedPoint;
		curSelectedPoint = point;
		point->color[0] = pointSelectColor[0];
		point->color[1] = pointSelectColor[1];
		point->color[2] = pointSelectColor[2];
		point->color[3] = pointSelectColor[3];
	}
	needRedraw = 1;
}

void selectPlane (Plane *plane) {
	if (plane->selected) {
		plane->selected = 0;
		plane->color[0] = planeDefaultColor[0];
		plane->color[1] = planeDefaultColor[1];
		plane->color[2] = planeDefaultColor[2];
		plane->color[3] = planeDefaultColor[3];
	}
	else {
		plane->selected = 1;
		plane->color[0] = planeSelectColor[0];
		plane->color[1] = planeSelectColor[1];
		plane->color[2] = planeSelectColor[2];
		plane->color[3] = planeSelectColor[3];
	}
	needRedraw = 1;
}

// Removes all the points found in the 'refSelected' struct_Point from the plane's point tree
void deleteSelectedPoint (void) {
	Plane *curPlane = drawnMesh->plane;
	Point *curPoint = curPlane->lastPoint, *curPoint3;
	while (1) {
		while (1) {
			if (curPoint->prev) {
				curPoint = curPoint->prev;
				if (curPoint->next->selected == 1) {
					if (curPoint->next->next == NULL) {
						curPoint->prev->next = curPoint;
						free (curPoint->next);
						curPoint->next = NULL;
					}
					else {
						curPoint->next->next->prev = curPoint;
						curPoint3 = curPoint->next;
						curPoint->next = curPoint->next->next;
						free (curPoint3);
					}
				}
				continue;
			}
			else break;
		}
		curPoint = drawnMesh->plane->point;
		while (1) {
			if (curPoint->next == NULL)
				break;
			else
				curPoint = curPoint->next;
			curPoint->ID = curPoint->prev->ID + 1;
		}
		if (curPlane->next == NULL) break;
		else {
			curPlane = curPlane->next;
			curPoint = curPlane->lastPoint;
		}
	}
}
