#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buffer.h"
#include "config.h"
#include "util.h"


// generic functions
// *****************
BufferNode *bufferReadFile (char *filename2, int *treeSize) {
	if (debug.buffer) printf ("bufferReadFile(): started\n");
	
	FILE *f1 = fopen (filename2, "r");
	if (f1 == NULL) {
		printf ("bufferReadFile(): Cannot open file named %s\n", filename2);
		perror ("perror");
		return NULL;
	}
	fclose (f1);
	
	if (debug.buffer) printf ("bufferReadFile(): done\n");
	return NULL;
}

BufferNode *bufferReadString (BufferTree *bt, char *str) {
	if (debug.buffer) printf ("bufferReadString() started\n");
	BufferNode *sb;
	if (bt->rootBuffer == NULL) {
		bt->rootBuffer = malloc (sizeof (BufferNode));
		bt->lastBuffer = bt->rootBuffer;
	}
	
	sb = bt->lastBuffer;
	sb->next = malloc (sizeof (BufferNode));
	sb->next->prev = sb;
	sb = sb->next;
	sb->next = NULL;
	bt->lastBuffer = sb;

	sb->size = strlen (str);
	sb->data = malloc (sb->size);
	sprintf (sb->data, "%s", str);

	if (debug.buffer) printf ("bufferReadString(): done, size: %d\n", sb->size);
	return sb;
}

BufferTree *bufferNewTree (char *treeName) {
	if (debug.buffer) printf ("bufferNewTree(): started\n");
	BufferTree *bt = malloc (sizeof (BufferTree));
	bt->rootBuffer = NULL;
	bt->lastBuffer = NULL;
	bt->name = malloc (strlen (treeName));
	sprintf (bt->name, "%s", treeName);

	if (debug.buffer) printf ("bufferNewTree(): done, new buffer tree named %s\n", treeName);

	return bt;
}

// internal functions
// ******************
void initBuffering (void) {
	if (debug.buffer) printf ("initBuffer(): started\n");

	if (debug.buffer) printf ("initBuffer(): done\n");
}


