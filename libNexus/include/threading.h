#ifndef THREADING_H
#define THREADING_H 1

#include <pthread.h>

extern int timerTime, timerLastTime;

extern pthread_t timerThread;
extern pthread_attr_t timerThreadAttr;

extern pthread_t drawingThread;

void initThreading (void);
void *timerThreadFunc (void *arg);
void *drawingThreadFunc (void *arg);

#endif /* THREADING_H */
