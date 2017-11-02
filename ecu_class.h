#ifndef _ECU_CLASS_H_
#define _ECU_CLASS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_ECU_NAME "MS430DS0"

class Ecu
{
	private:
		bool PRINT_DEBUG;
		char ECU_NAME[50];

	public:
		void Ecu(const char *ecuName = DEFAULT_ECU_NAME, bool debugEnabled = true);
		void SetEcuName(const char *newEcuName);
		char * GetEcuName(void);

		int FatalError(void);
		void MakeJob(const char* jobName, const char* params = "");

		void DisableDebugPrint(void);
		void EnableDebugPrint(void);
		void SetDebugPrint(bool newState);
};

#endif
