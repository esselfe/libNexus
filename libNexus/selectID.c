#include <stdio.h>
#include <stdlib.h>

#include <displayList.h>
#include <mesh.h>
#include <selectID.h>

void initSelectID (void) {
	meshRootSelectID = malloc (sizeof (struct SelectID));
	struct SelectID *sid = meshRootSelectID;
	sid->id = 1;
	sid->type = SID_TYPE_MESH;
	sid->prev = NULL;
	sid->next = NULL;
	meshLastSelectID = sid;

	menuRootSelectID = malloc (sizeof (struct SelectID));
	sid = menuRootSelectID;
	sid->type = SID_TYPE_MENU;
	sid->id = 1;
	sid->prev = NULL;
	sid->next = NULL;
	menuLastSelectID = sid;
}

int newSelectID (int sidType) {
	struct SelectID *sid = NULL;
	switch (sidType) {
	case SID_TYPE_MENU:
		sid = menuLastSelectID;
		sid->next = malloc (sizeof (struct SelectID));
		sid->next->prev = sid;
		sid = sid->next;
		sid->next = NULL;
		sid->type = SID_TYPE_MENU;
		sid->id = sid->prev->id + 1;
		menuLastSelectID = sid;
		break;
	case SID_TYPE_MESH:
		sid = meshLastSelectID;
		sid->next = malloc (sizeof (struct SelectID));
		sid->next->prev = sid;
		sid = sid->next;
		sid->next = NULL;
		sid->type = SID_TYPE_MESH;
		sid->id = sid->prev->id + 1;
		meshLastSelectID = sid;
		break;
	default:
		printf ("newSelectID(): unknown sidType, %d\n", sidType);
		break;
	}
	if (sid != NULL) return sid->id;
	else return 0;
}

void showIds (void) {
	printf ("showIds(): showing registered list Ids\n");
	struct SelectID *sid = menuRootSelectID;
	printf ("* menu:\n");
	while (1) {
		printf ("\t%d ", sid->id);

		if (sid->next == NULL) {
			printf ("\n");
			break;
		}
		else {
			sid = sid->next;
		}
	}
	sid = meshRootSelectID;
	printf ("* mesh:\n");
	while (1) {
		printf ("\t%d ", sid->id);
		if (sid->next == NULL) {
			printf ("\n");
			break;
		}
		else sid = sid->next;
	}
}

