#ifndef AREA_H
#define AREA_H 1
// This file defines areas to structure floors and scene

#include "mesh.h"
#include "Ntype.h"

typedef struct Area {
	int id;
	mbool active;
	struct SScene *scene;
} sArea;

typedef struct AreaList {
	struct Area *rootArea, *lastArea;
} sAreaList;



#endif /* AREA_H */
