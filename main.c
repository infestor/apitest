#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

#include "api.h"

unsigned char cu_parameter[]= {0x01,0x01,0x01,0x01,0x0F,0x20,0x03,0x64,0x00};
int cu_parameterlength = 9;
 
unsigned char request_mes[]= {0x12,0x05,0x0B,0x03}; 
int request_mes_length = 4; 
unsigned volatile char x;

APIBINARY answer_mes[APIMAXBINARY];      
APIWORD answer_mes_length;

//--------------------------------------------------------------------------------
printf("Starting program..\n");
printf("Init..\n");
if (apiInit() == APIFALSE) {
  printf("Error (%i) during initialisation! Quit..", (int)apiErrorCode());
  apiEnd();
  return(-1);
}

printf("Sending telegram..\n");
apiJobData("TMODE", "SENDE_TELEGRAMM", request_mes, request_mes_length, ""); 

printf("Waiting for response..\n");
while (apiState() == APIBUSY) {
 /* short programme part, e.g. keyboard polling */
 x++;
}

printf("Getting response..\n");
if (apiResultBinary(answer_mes, &answer_mes_length, "SG_ANSWER", 1)) 
{
 /* continue processing answer message */
 printf("Received message len: %i\n", (int)answer_mes_length);
 int i;
 for (i=0; i < answer_mes_length; i++)
 {
   printf("%02X ", answer_mes[i]);
 }
 printf("\n");
}
else
{
 /* error handling, e.g. with apiErrorCode */ 
  int error = apiErrorCode();
  printf("Error occured (%i) - %s\n", error, apiErrorText());
}

printf("End..");
apiEnd();


