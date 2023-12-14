#ifndef DATA_H
#define DATA_H 1

#include <sys/sysinfo.h>
//#include <sensors/sensors.h>

/* this is used for fseek(), reading sections of files */
extern const int BLOCK_SIZE; /* initialized at 1024 */

/* this is where we are among sections, increased by 1 each 1024 */
extern int BLOCK_OFFSET; /* initialized at 0 */

extern struct sysinfo sinfo;
extern int lastUpdateDataTime;
extern struct statfs *tmpstat;

extern char *str2;
extern char *tmpstr;
extern const int TMPSTRSIZE;
extern char *CPUtempstr;
extern double CPUtemp;
//const sensors_chip_name *sensorsChipName, sensorsChipName2;
//extern sensors_chip_name sensorsChipName3;

void initOSData (void);
void updateOSData (void);
void updateSensorsInfo (void);

extern pthread_t sensThread;
void *sensThreadFunc (void *argp);

#endif /* DATA_H */
