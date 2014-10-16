#ifndef GLOBAL_H_
#define GLOBAL_H_

#include "include.h"

/******************************************************************/
/*                         DEFINES                                */
/******************************************************************/
#define NUMBEROFTASKS 4

/******************************************************************/
/*                        CONVENÇÕES                              */
/******************************************************************/
//ID das Tasks
#define SENSOR		0
#define CONTROL		1
#define ACTUATOR	2
#define TEST		3

//Valores para a variável DISCRETIZATION
#define TUSTIN 				1
#define BACKWARD_DIFF 		2

//Valores para a variável TRAJECTORY
#define SINUSOIDAL_POS		1
#define CONSTANT			2
#define SINUSOIDAL_TOR		3
#define CONTROL_ACTIVE		4

/******************************************************************/
/*                        CONVERSÕES                              */
/******************************************************************/
//Conversão do Timer
#define NS2MS 1000000 //Ns para Ms
#define NS2US 1000    //Ns para Us

/******************************************************************/
/*                    VARIÁVEIS GLOBAIS                           */
/******************************************************************/
//Configuração da placa
extern DSCB dscb;
extern DSCCB dsccb;
ERRPARAMS errorParams;       // structure for returning error code and error string

//Opções iniciais
extern short int MONITORING;
extern short int SENSORING;
extern short int TRAJECTORY;
extern short int DISCRETIZATION;

extern double setpoint;

//Valores a ser enviado do Controle para o Atuador [k-2,k-1,k]
double POSITION[3];

//Valores lido do Strain Gage [k-2,k-1,k]
double TORQUE[3];

//Parâmetros do controlador
extern double J;
extern double K;
extern double B;

//Offset do setpoint
extern double setpoint_offset;

#endif /* GLOBAL_H_ */
