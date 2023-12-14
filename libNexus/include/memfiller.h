#ifndef MEMFILL_H
#define MEMFILL_H 1
// Usesd to select wether filling with a data structure in RAM
// or with a file on the specified media
#define MEMFILL_TYPE_RAM	1
#define MEMFILL_TYPE_DISK	2

extern const char *MEMFILLER_VERSION;
extern const int MEMFILLER_BUILD;

#include <pthread.h>
extern pthread_t memfillThread;

typedef struct sNodeTree {
	struct sNodeTree *next;
	long long val,v2,v3,v4,v5,v6,v7,v8;
} NodeTree;

extern NodeTree *rootNode, *lastNode;

void *memfillThreadFunc (void *argp);

int addNode (unsigned long int qty);
int memfill (int memtype, char *fileLocation);	// 2nd param NULL with RAM

#endif /* MEMFILL_H */
