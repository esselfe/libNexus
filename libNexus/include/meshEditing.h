#ifndef MESHEDITING_H
#define MESHEDITING_H 1

#include "mesh.h"

extern float pointDefaultColor[4];
extern float pointSelectColor[4];
extern float lineDefaultColor[4];
extern float lineSelectColor[4];
extern float planeDefaultColor[4];
extern float planeSelectColor[4];
extern float selectionMovingStep;
extern float selectionMaxMovingStep;
extern signed short movingSelectionUD;
extern signed short movingSelectionLR;
extern signed short movingSelectionIO;
extern int once16;

void moveSelection (void);

void hoverPlane (Plane *plane, int hoverState);
void hoverPoint (Point *point, int hoverState);
void selectPoint (Point *point);
void selectPlane (Plane *plane);
void deleteSelectedPoint (void);

#endif /* MESHEDITING_H */
