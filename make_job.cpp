#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "api.h"

int FatalError(void)
{
	printf("\nEDIABAS error (%d): %s\n", apiErrorCode(), apiErrorText());
	apiEnd();
	exit(-1);
}

void MakeJob(const char* jobName, const char* params, const char * ecuName)
{
	unsigned char volatile x;

	printf("JOB: %s (params: %s)\n", jobName, params);
	apiJob(ecuName, jobName, params, "");

	printf("Waiting for response..\n");
	while (apiState() == APIBUSY) {
	 x++;
	}

	if (apiState() == APIERROR) FatalError();
}
