#include <pthread.h>

extern pthread_t timerThread;

void *timerThreadFunc (void *arg);
