/*
 * actuator.h
 *
 *  Created on: Oct 9, 2013
 *      Author: root
 */

#ifndef ACTUATOR_H_
#define ACTUATOR_H_

#include "global.h"
#include "include.h"

BYTE channel;
DSCDACODE output_code;

int A_Init();
int A_Calibration();
int A_Core();
int A_Kill();
void send_setpoint(double value);		//value em graus

#endif /* ACTUATOR_H_ */
