#include <stdio.h>

extern char *str;
extern char *runcmd;

char *askNewPassword (void);
char *chkPassword (FILE *file);
void runit (void);
void vmsg2 (char *str2);
