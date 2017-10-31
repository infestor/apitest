#include "frame_def.h"

//  { Name | Unit | pos | len | * | + }
t_Sensor_Def SENSORS[SENSORS_SIZE]  = {
	{"RPM", "1/min", 3, 2, 1, 0},
	{"Speed", "km/h", 5, 1, 1, 0},
	{"TPS", "deg", 6, 2, 0.00173504, 0},
	{"Air mass", "kg/h", 8, 2, 0.25, 0},
	{"Intake temp", "C", 10, 1, 0.75, -48},
	{"Motor temp", "C", 11, 1, 0.75, -48},
	{"Oil temp", "C", 12, 1, 0.7961, -48},
	{"Radiator outlet temp", "C", 13, 1, 0.75, -48},
	{"Ignition angle", "deg", 14, 1, -0.375, 72},
	{"Injection time", "ms", 15, 2, 0.004, 0},
	{"Idle speed controller", "%", 17, 2, 0.001526, 0},
	{"Idle speed duty cycle", "%", 19, 2, 0.001526, 0}
};

/*
{"", "", , , , },
{"", "", , , , },
{"", "", , , , },
{"", "", , , , },
*/
