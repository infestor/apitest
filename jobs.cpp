#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <ctype.h> 

#include "api.h"
#include "print_result.h"
#include "ecu_class.h"

typedef struct {
	char resultName[150];
	char resultType[50];
	char resultComment[200];
} t_Job_Result;

typedef struct {
	char argName[150];
	char argType[50];
	char argComment[200];
} t_Job_Argument;

typedef struct {
	char jobName[150];
	char jobDesc[150];
	unsigned char numJobArguments;
	t_Job_Argument *jobArguments;
	unsigned char numJobResults;
	t_Job_Result *jobResults;
} t_Job_Record;

unsigned int numJobRecords;
t_Job_Record *jobRecords;
Ecu ecu();

//----------------------------------------------------------------------------------
int main (int argc, char *argv[])
{ 
	APITEXT t[APIMAXTEXT];

	if (argc == 2) { //non default ecu name
		ecu.SetEcuName(argv[1]);
	}
	printf("Using ecu: %s\n\n", ecu.GetEcuName);

	printf("Init..\n");
	if (apiInit() == APIFALSE) FatalError();

	ecu.MakeJob("_JOBS");

	APIWORD sets;
	apiResultSets (&sets);
	numJobRecords = (unsigned int)sets;
	jobRecords = new t_Job_Record[numJobRecords];
	printf("TotalJobSets %d\n\n", numJobRecords);
	printf("*****************************************************************************\n");
	ecu.DisableDebugPrint();

	for (APIWORD set = 1; (set <= numJobRecords) && (apiErrorCode() == EDIABAS_ERR_NONE); set++) {
		apiResultText(t, "JOBNAME", set, "");
		strcpy(jobRecords[set - 1].jobName, t);
	}

	for (unsigned int i = 0; i < numJobRecords; i++ )
	{
		t_Job_Record *pJob = &jobRecords[i];

		ecu.MakeJob("_JOBCOMMENTS", pJob->jobName);
		apiResultText(t, "JOBCOMMENT0", 1, "");
		strcpy(pJob->jobDesc, t);

		//get all JOB ARGUMENTS
		ecu.MakeJob("_ARGUMENTS", pJob->jobName);
		apiResultSets (&sets);
		pJob->numJobArguments = sets;
		pJob->jobArguments = new t_Job_Argument[pJob->numJobArguments];

		for (APIWORD set = 1; (set <= sets) && (apiErrorCode() == EDIABAS_ERR_NONE); set++) {
			apiResultText(t, "ARG", set, "");
			strcpy(pJob->jobArguments[set - 1].argName, t);
			apiResultText(t, "ARGTYPE", set, "");
			strcpy(pJob->jobArguments[set - 1].argType, t);
			apiResultText(t, "ARGCOMMENT0", set, "");
			strcpy(pJob->jobArguments[set - 1].argComment, t);
		}

		//get all JOB RESULTS
		ecu.MakeJob("_RESULTS", pJob->jobName);
		apiResultSets (&sets);
		pJob->numJobResults = sets;
		pJob->jobResults = new t_Job_Result[pJob->numJobResults];

		for (APIWORD set = 1; (set <= sets) && (apiErrorCode() == EDIABAS_ERR_NONE); set++) {
			apiResultText(t, "RESULT", set, "");
			strcpy(pJob->jobResults[set - 1].resultName, t);
			apiResultText(t, "RESULTTYPE", set, "");
			strcpy(pJob->jobResults[set - 1].resultType, t);
			apiResultText(t, "RESULTCOMMENT0", set, "");
			strcpy(pJob->jobResults[set - 1].resultComment, t);
		}
	}

	//--------- OUTPUT OF ALL JOB RECORDS -------------------
	for (unsigned int job = 0; job < numJobRecords; job++)
	{
		printf("%s\n", jobRecords[job].jobName);
		printf("(%s)\n", jobRecords[job].jobDesc);

		if (jobRecords[job].numJobArguments > 0) {
			for (int res = 0; res < jobRecords[job].numJobArguments; res++)
			{
				printf(" << %s (%s) | %s\n", jobRecords[job].jobArguments[res].argName, jobRecords[job].jobArguments[res].argType, jobRecords[job].jobArguments[res].argComment);
			}
			printf("-----------------------------------------------\n")
		}

		for (int res = 0; res < jobRecords[job].numJobResults; res++)
		{
			printf("  > %s (%s) | %s\n", jobRecords[job].jobResults[res].resultName, jobRecords[job].jobResults[res].resultType, jobRecords[job].jobResults[res].resultComment);
		}
		printf("\n");
	}

ecu.EnableDebugPrint();

//remove all allocated memory
for (unsigned int job = 0; job < numJobRecords; job++)
{
	delete[] jobRecords[job].jobResults;
	delete[] jobRecords[job].jobArguments;
}
delete[] jobRecords;

printf("\n");
apiEnd();
return  0;
}
 
