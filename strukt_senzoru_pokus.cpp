#include <stdio.h>
#include "frame_def.h"

/*
 * Test moznosti definovani pole senzoru a jejich vlastnostni pro pozdejsi hromadne
 * zpracovani celeho paketu
 */

t_Sensor_Def xN = {"RPM", "1/min", 3, 2, 1, 0};

int main(void)
{
	printf("Nazdar");
	printf("%s|%s|%d|%d|%f|%i\n", xN.name, xN.unit, xN.position, xN.length, xN.multiply, xN.add);
}

