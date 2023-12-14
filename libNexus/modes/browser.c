#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>

#include "browser.h"
#include "buffer.h"
#include "camera.h"
#include "config.h"

float fsheetSizeX = 1.0, fsheetSizeY = 0.25;

mbool renderingBrowser = 0;

DIR *dir;
struct dirent *dirEntry;

struct SBufferTree *browserBufferTree;
FileTree *rootFileTree;

void initBrowser (void) {
	if (debug.browser) printf ("initBrowser(): buffering files\n");
	rootFileTree = createNewFileTree ("/");
	showFileTree (rootFileTree);
	sortFileTree2 (rootFileTree);
	showFileTree (rootFileTree);

	rootFileTree->bufferTree = bufferNewTree ("browserBufferTree");

	FileSheet *fs = rootFileTree->rootfsheet;
	while (1) {
		if (debug.browser) printf ("%s\n", fs->name);
		
		if (fs->next == NULL) break;
		else fs = fs->next;
	}
	if (debug.browser) printf ("initBrowser(): done\n");
}

FileTree *createNewFileTree (char *dirname) {
	dir = opendir (dirname);
	if (dir == NULL) {
		printf ("createNewFileTree(): opendir(\"%s\") == NULL!\n", dirname);
		perror ("perror");
		return NULL;
	}
	
	FileTree *ftree = malloc (sizeof (FileTree));
	ftree->name = malloc (FILESHEETNAMESIZE);
	memset (ftree->name, ' ', FILESHEETNAMESIZE-1);
	sprintf (ftree->name, "%s", "/");
	ftree->x = -1.0;
	ftree->y = cam->y;
	ftree->z = 0.0;
	ftree->prev = NULL;
	ftree->next = NULL;
	ftree->rootfsheet = malloc (sizeof (FileSheet));
	FileSheet *fsheet = ftree->rootfsheet;
	fsheet->next = NULL;
	fsheet->prev = NULL;
	fsheet->name = NULL;
	fsheet->id = 1; // newSelectID()
	float tmpcolor[4] = {0.2,0.35,0.65,0.5};
	setFsheetColor (fsheet, tmpcolor);
	fsheet->x = 0.0;
	fsheet->y = 0.0;
	fsheet->z = 0.0;
	short entrycnt = 0;
	//printf ("extracted:\n");
	while (1) {
		dirEntry = readdir (dir);
		if (dirEntry == NULL) {
			break;
		}
		++entrycnt;
		
	//	if (strcmp (".", dirEntry->d_name) == 0) continue;
		
		if (fsheet->name != NULL) {
			fsheet->next = malloc (sizeof (FileSheet));
			fsheet->next->prev = fsheet;
			fsheet->next->next = NULL;
			fsheet = fsheet->next;
		}
		
		fsheet->name = malloc (strlen (dirEntry->d_name)+1);
		sprintf (fsheet->name, "%s", dirEntry->d_name);
		fsheet->type = dirEntry->d_type;

		if (fsheet->prev == NULL) fsheet->x = 0.0;
		else fsheet->x = fsheet->prev->x + fsheetSizeX + 0.2;
		fsheet->y = 0.0;
		fsheet->z = 0.0;
		//printf ("\t%s, type %d\n", fsheet->name, fsheet->type);
	}
	ftree->lastfsheet = fsheet;
	ftree->totalEntrycnt = entrycnt;
	closedir (dir);
	return ftree;
}

void destroyFiletree (FileTree *ftree) {
	FileSheet *fsheet = ftree->lastfsheet;
	while (1) {
		if (fsheet->name != NULL) free (fsheet->name);
		fsheet->name = NULL;
		if (fsheet->prev == NULL) break;
		else {
			fsheet = fsheet->prev;
			fsheet->next->prev = NULL;
			if (fsheet->next != NULL) free (fsheet->next);
			fsheet->next = NULL;
		}
	}
	ftree->totalEntrycnt = 0;
	free (ftree);
	ftree = NULL;
}

/* This function replaces dirent.h::scandir() since I just don't understand it. lazy me ;D */
void sortFiletree (FileTree *ftree) {
	int cntindex[256];	// Each element is a count of how many times the letter appears in the tree
	int cnt = 0;
	while (1) {
		cntindex[cnt] = 0;
		++cnt;
		if (cnt >= 256) break;
	}
	
	FileSheet *fsheet = ftree->rootfsheet;
	while (1) {	// get the letter counts
		++cntindex[(int)*fsheet->name];
		if (fsheet->next == NULL) break;
		else fsheet = fsheet->next;
	}
	
	FileTree *nftree = malloc (sizeof (FileTree));	// New tree
	nftree->name = malloc (FILESHEETNAMESIZE);
	memset (nftree->name, '#', FILESHEETNAMESIZE-1);
	sprintf (nftree->name, "%s", "nftree");
	nftree->prev = NULL;
	nftree->next = NULL;
	nftree->rootfsheet = malloc (sizeof (FileSheet));
	FileSheet *nfsheet = nftree->rootfsheet;
	nfsheet->name = NULL;
	nfsheet->prev = NULL;
	nfsheet->next = NULL;
	cnt = 0;
	fsheet = ftree->rootfsheet;
	while (1) { // set the new tree
		if (*fsheet->name == cnt) {
			if (nfsheet->name != NULL) {
				nfsheet->next = malloc (sizeof (FileSheet));
				nfsheet->next->prev = nfsheet;
				nfsheet = nfsheet->next;
			}
			nfsheet->name = malloc (FILESHEETNAMESIZE);
			memset (nftree->name, '$', FILESHEETNAMESIZE-1);
			sprintf (nfsheet->name, "%s", fsheet->name);
			nfsheet->type = fsheet->type;
		}
		
		if (fsheet->next == NULL) {
			++cnt;
			if (cnt == 256) break;
			fsheet = ftree->rootfsheet;
		}
		else {
			fsheet = fsheet->next;
		}
	}
	nftree->lastfsheet = nfsheet;
	
	fsheet = ftree->rootfsheet;
	nfsheet = nftree->rootfsheet;
	while (1) {	// Rearrange the original tree from the new one
		fsheet->type = nfsheet->type;
		if (fsheet->next == NULL) break;
		else fsheet = fsheet->next;
		if (nfsheet->next == NULL) break;
		else nfsheet = nfsheet->next;
	}
	
	destroyFiletree (nftree);
}

void sortFileTree2 (FileTree *ftree) {
	FileSheet *tsheet = ftree->rootfsheet;
//	FileSheet *fs1, *
	char *c;
	while (1) {
		c = tsheet->name;
		if (debug.browser) printf ("%c %d\n", *c, *c);

		if (tsheet->next == NULL) break;
		else tsheet = tsheet->next;
	}

}

void setFsheetColor (FileSheet *fs, float newColor[4]) {
	fs->color[0] = newColor[0];
	fs->color[1] = newColor[1];
	fs->color[2] = newColor[2];
	fs->color[3] = newColor[3];
}

void showFileTree (FileTree *ftree) {
	if (debug.browser == 0) return;
	printf ("\nshowFiletree(): showing %s, %d total entries\n", ftree->name, ftree->totalEntrycnt);
	FileSheet *fsheet = ftree->rootfsheet;
	while (1) {
		printf ("\t%s, type %d\n", fsheet->name, fsheet->type);
		if (fsheet->next == NULL) break;
		else fsheet = fsheet->next;
	}
	printf ("showFiletree(): done\n");
}

