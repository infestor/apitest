#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ecu_class.h"
#include "api.h"


void Ecu::Ecu(const char *ecuName, bool debugEnabled)
{
	strcpy(ECU_NAME, ecuName);
	PRINT_DEBUG = debugEnabled;
}

void Ecu::DisableDebugPrint(void)
{
	PRINT_DEBUG = false;
}

void Ecu::EnableDebugPrint(void)
{
	PRINT_DEBUG = true;
}

void Ecu::SetDebugPrint(bool newState)
{
	PRINT_DEBUG = newState;
}

int Ecu::FatalError(void)
{
	printf("\nEDIABAS error (%d): %s\n", apiErrorCode(), apiErrorText());
	apiEnd();
	exit(-1);
}

void Ecu::SetEcuName(const char *newEcuName)
{
	strcpy(ECU_NAME, newEcuName);
}

char * Ecu::GetEcuName(void)
{
	return ECU_NAME;
}

void Ecu::MakeJob(const char* jobName, const char* params)
{
	unsigned char volatile x;

	if (PRINT_DEBUG) printf("JOB: %s (params: %s)\n", jobName, params);
	apiJob(ECU_NAME, jobName, params, "");

	if (PRINT_DEBUG) printf("Waiting for response..\n");
	while (apiState() == APIBUSY) {
	 x++;
	}

	if (apiState() == APIERROR) FatalError();
}
