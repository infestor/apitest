#ifndef _MAKE_JOB_H_
#define _MAKE_JOB_H_

// Default setting
#define DEFAULT_ECU_NAME "MS430DS0"

int FatalError(void);
void MakeJob(const char* jobName, const char* params = "", const char * ecuName = DEFAULT_ECU_NAME);


#endif
