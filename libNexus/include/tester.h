
extern int testerDone;
extern pthread_t testerThread;

void *testerThreadFunc (void *argp);
void startTester (void);
int tester (void); // Quick devel / debug testing...
