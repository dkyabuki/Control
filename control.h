#ifndef CONTROL_H_
#define CONTROL_H_

#include "global.h"
#include "include.h"

//Setpoint enviado quando no modo de setpoint constante
#define SET_CTE 0

//Periodo do movimento na trajetória senoidal
#define TRAJ_PERIOD 8000 //ms

//Parâmetros de inicialização do controle
double Tsample;

//Parâmetros de controle discreto
double a;
double b;
double c;
double d;
double e;
double f;

//double a0;
//double a1;
//double a2;
//double b0;
//double b1;
//double b2;

double trajectory_time;

int C_Init();
int C_Kill();
int C_Core();



#endif /* CONTROL_H_ */
