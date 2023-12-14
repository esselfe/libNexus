#ifndef SELECTID_H
#define SELECTID_H 1

struct SelectID {
	struct SelectID *prev, *next;
	int type;	// SID_TYPE_* include/Ntype.h
	int id;
} *meshRootSelectID, *meshLastSelectID, 
*menuRootSelectID, *menuLastSelectID;

void initSelectID (void);
/* Types found in Ntype.h */
int newSelectID (int sidType);
void showIds (void);

#endif /* SELECTID_H */
