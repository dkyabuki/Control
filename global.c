#include "global.h"

short int MONITORING;
short int SENSORING;
short int TRAJECTORY;
short int DISCRETIZATION;

double J;
double K;
double B;

//saída do controle
double setpoint;

//entrada do controle
double position_reading;
double torque_reading;

double potentiometerOffset;
double extensometerOffset;
DSCB dscb;
DSCCB dsccb;
