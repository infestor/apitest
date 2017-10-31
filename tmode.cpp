#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <ctype.h> 

#include "api.h"
#include "print_result.h"
#include "make_job.h"
#include "frame_def.h"

double SENSOR_VALUES[SENSORS_SIZE];

void ComputeSensorValues(APIBINARY *data, APIWORD dataLen, double *sensorValues)
{
	for (int i = 0; i < SENSORS_SIZE; i++)
	{
		t_Sensor_Def *pS = &SENSORS[i];
		double value;
		unsigned int rawValue;

		rawValue = data[pS->position];
		if (pS->length == 2) rawValue += (((unsigned int)data[pS->position+1]) >> 8);
		value = ((double)rawValue * pS->multiply) + (double)pS->add;
		sensorValues[i] = value;
	}
}

int main (void)
{ 
	printf("Init..\n");
	if (apiInit() == APIFALSE) FatalError();

	printf("Just asking RPM to initialize ECU:\n");
	MakeJob("STATUS_MESSWERTEBLOCK");
	printResultField(stdout);
	printf("\n");

	// TELEGRAM
	unsigned char request_mes[]= {0x12,0x05,0x0B,0x03};
	int request_mes_length = 4;
	unsigned int volatile x;

	printf("Sending TMODE telegram..\n");
	apiJobData("TMODE", "SENDE_TELEGRAMM", request_mes, request_mes_length, "");

	printf("Waiting for response..\n");
	while (apiState() == APIBUSY) x++;

	printf("Getting response..\n");
	APIBINARY answer_mes[APIMAXBINARY];
	APIWORD answer_mes_length;
	if (apiResultBinary(answer_mes, &answer_mes_length, "SG_ANSWER", 1)) 
	{
	 printf("Received message len: %d\n", (unsigned int)answer_mes_length);
	 unsigned int i;
	 for (i = 0; i < answer_mes_length; i++)
	 {
	   printf("%02X ", answer_mes[i]);
	 }
	 printf("\n");
	}
	else
	{
		FatalError();
	}

	printf("\n\nSensor values:\n");
	ComputeSensorValues(answer_mes, answer_mes_length, SENSOR_VALUES);
	for (int i = 0; i < SENSORS_SIZE; i++)
	{
		printf(" %s = %.1f %s", SENSORS[i].name, SENSOR_VALUES[i], SENSORS[i].unit);
	}

printf("\n");
apiEnd();
return  0;
}

