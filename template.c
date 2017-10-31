#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <ctype.h> 

#include "api.h"
#include "print_result.h"

unsigned char volatile x;

int main (void) 
{ 
	printf("Init..\n");
	if (apiInit() == APIFALSE) {
	  printf("Error (%d) during initialisation!\n%s\nQuit..\n", (int)apiErrorCode(), apiErrorText());
	  apiEnd();
	  return(-1);
	}

	apiJob("MS430DS0", "_JOBS", "", "");

	printf("Waiting for response..\n");
	while (apiState() == APIBUSY) {
	 x++;
	}

	if (apiState() == APIERROR) { 
	  printf("\nEDIABAS error %d: %s\n", apiErrorCode(), apiErrorText());
	  apiEnd();
	  return(-1);
	}   

	printResultField(stdout);   

	apiEnd();
	return  0;   
}
 
