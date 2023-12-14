#ifndef BROWSER_H
#define BROWSER_H 1

#include <dirent.h>

#include "Ntype.h"

#include "buffer.h"
extern struct SBufferTree *browserBufferTree;

#define FILESHEETNAMESIZE 128

extern float fsheetSizeX, fsheetSizeY;
//#define FSHEET_SIZEY	0.25

extern mbool renderingBrowser;

extern DIR *dir;
extern struct dirent *tmpdir;

typedef struct sFileSheet {
	struct sFileSheet *prev, *next;
	int type;
	int id;
	char *name;
	float color[4];
	float x, y, z;
} FileSheet;

typedef struct sFileTree {
	struct sFileTree *prev, *next;
	struct sFileSheet *rootfsheet, *lastfsheet;
	struct SBufferTree *bufferTree;
	char *name;
	short totalEntrycnt;
	float x, y, z;
} FileTree;
extern FileTree *rootFileTree;

void initBrowser (void);
FileTree *createNewFileTree (char *dirname);
void destroyFiletree (FileTree *ftree);
void showFileTree (FileTree *ftree);
void setFsheetColor (FileSheet *fs, float newColor[4]);
void sortFiletree (FileTree *ftree);
void sortFileTree2 (FileTree *ftree);

#endif /* BROWSER_H */

