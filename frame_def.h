#ifndef _FRAME_DEF_H_
#define _FRAME_DEF_H_

#include <float.h>
#include <ctype.h>

typedef struct {
	char name[100];
	char unit[10];
	unsigned char position;
	unsigned char length;
	double multiply;
	int add;
} t_Sensor_Def;

#define SENSORS_SIZE 12

extern t_Sensor_Def SENSORS[SENSORS_SIZE];

/*
union {
	unsigned char B;
	unsigned int W;
} rawData;
*/

#endif
