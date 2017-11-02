#include <stdio.h>
#include "string.h"

#include "api.h"
#include "print_result.h"
#include "ecu_class.h"

#define NUM_JOBS 7
const char * const JOBY[NUM_JOBS] = {
	"IDENT", "COD_LESEN_ALLGEMEIN", "COD_LESEN_SERVOTRONIK", "STATUS_ANALOG_GM3",
	"STATUS_DIGITAL_GM3_INT", "STATUS_DIGITAL_GM3_EA", "STATUS_DIGITAL_GM3_KP"
};

Ecu ecu("ZKE3_GM5", false);

int main(void)
{
	printf("Using ecu: %s\n\n", (char*)(ecu.GetEcuName()));
	printf("Init..\n");
	if (apiInit() == APIFALSE) ecu.FatalError();

	for (int i = 0; i < NUM_JOBS; i++)
	{
		printf("--------------- %s --------------\n", JOBY[i]);
		ecu.MakeJob(JOBY[i]);
		printResultField(stdout);
		printf("\n\n");
	}

apiEnd();
return 0;
}
