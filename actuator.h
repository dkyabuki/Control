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

BYTE result;
DSCDACALPARAMS dscdacalparams;  // structure containing auto-calibration settings

int A_Init();
int A_Calibration();
int A_Core();
int A_Kill();
void send_setpoint(double value);

#endif /* ACTUATOR_H_ */
