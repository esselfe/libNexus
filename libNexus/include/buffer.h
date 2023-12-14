#ifndef BUFFER_H
#define BUFFER_H 1
/* This file is for the browser to show contents of desired files,
 * or to handle and gather any form of fata for genericity */
	
#include "Ntype.h"

#define BUFFER_TYPE_STRING 		0x01
#define BUFFER_TYPE_FILETEXT	0x08

typedef struct SBuffer {
	struct SBuffer *prev, *next;
	int size;
	char *data;
} BufferNode;
//typedef struct SBuffer TextBuffer;
//typedef struct SBuffer DataBuffer;

typedef struct SBufferTree {
	struct SBuffer *rootBuffer, *lastBuffer;
	char *name;
} BufferTree;



// generic functions
// *****************

/* Will add a block of 1024 in the buffers tree */
/* 2nd param returns the size needed by the returned bufferTree */
struct SBuffer *bufferReadFile (char *filename2, int *treeSize);
struct SBuffer *bufferReadString (BufferTree *bt, char *str);
struct SBufferTree *bufferNewTree (char *treeName);




// internal functions
// ******************

void initBuffering (void);

#endif	/* BUFFER_H */
