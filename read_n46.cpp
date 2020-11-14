#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <ctype.h> 

#include "api.h"
#include "print_result.h"
#include "ecu_class.h"

Ecu ecu("MEV9N46L", false);

char JOBS_TO_MAKE[][3][30] = { //job name, result name, units
	{"STATUS_INT", "STAT_INT_WERT", "-"},
	{"STATUS_ADD", "STAT_ADD_WERT", "-"},
	{"STATUS_MUL", "STAT_MUL_WERT", "-"},
	{"STATUS_MOTORTEMPERATUR", "STAT_MOTORTEMPERATUR_WERT", "C"},
	{"STATUS_OELSENSOR", "STAT_OELNIVEAU_WERT", "hladina"},
	{"STATUS_OELSENSOR", "STAT_OELTEMPERATUR_WERT", "C"}
};

double ecu_result;

//----------------------------------------------------------------------------------
int main (int argc, char *argv[])
{ 
	printf("Using ecu: %s\n", (char*)(ecu.GetEcuName()));
	printf("Init..\n");
	if (apiInit() == APIFALSE) ecu.FatalError();

	for (int i = 0; i < 6; i++) //cycle through JOBS_TO_MAKE
	{
		ecu.MakeJob(JOBS_TO_MAKE[i][0]);
		apiResultReal((APIREAL*)&ecu_result, JOBS_TO_MAKE[i][1], 1);

		printf("%s: %.2f [%s]\n", JOBS_TO_MAKE[i][0], ecu_result, JOBS_TO_MAKE[i][2]);
	}

	printf("\n");
	apiEnd();
	return  0;
}
 
