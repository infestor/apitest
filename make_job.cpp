#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "api.h"

static bool volatile PRINT_DEBUG = true;

int FatalError(void)
{
	printf("\nEDIABAS error (%d): %s\n", apiErrorCode(), apiErrorText());
	apiEnd();
	exit(-1);
}

void DisableDebugPrint(void)
{
	PRINT_DEBUG = false;
}

void EnableDebugPrint(void)
{
	PRINT_DEBUG = true;
}

void SetDebugPrint(bool newState)
{
	PRINT_DEBUG = newState;
}

void MakeJob(const char* jobName, const char* params, const char * ecuName)
{
	unsigned char volatile x;

	if (PRINT_DEBUG) printf("JOB: %s (params: %s)\n", jobName, params);
	apiJob(ecuName, jobName, params, "");

	if (PRINT_DEBUG) printf("Waiting for response..\n");
	while (apiState() == APIBUSY) {
	 x++;
	}

	if (apiState() == APIERROR) FatalError();
}
