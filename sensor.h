#ifndef SENSOR_H_
#define SENSOR_H_


#include "global.h"
#include "include.h"

#define BILLION 1E9
#define OSC_PERIOD 8000

/******************************************************/
/*                 BOARD VARIABLES                    */
/******************************************************/
BYTE result; 					// returned0f error code
DSCSAMPLE sample; 				// sample reading
DSCADCALPARAMS dscadcalparams;
DSCADSETTINGS dscadsettings; 	// structure containing A/D conversion settings
DSCAUTOCAL dscautocal; 			// structure containing auto-calibration settings
DSCSAMPLE* samples;          	// sample readings
DSCADSCAN dscadscan;         	// structure containing A/D scan settings

/******************************************************/
/*                  TASK VARIABLES                    */
/******************************************************/
//Time tracking
struct timespec start;
struct timespec end;

//Sensor reading
double osc_time;



/******************************************************/
/*                     FUNCTIONS                      */
/******************************************************/
//Task functions
int S_Init();
int S_Calibration();
int S_Kill();
int S_Core();

//Tool functions
//void set_reference_voltage();
void S_CalcOffset();
double S_getTorque();

#endif /* SENSOR_H_ */
