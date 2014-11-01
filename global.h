#ifndef GLOBAL_H_
#define GLOBAL_H_

#include "include.h"

/******************************************************************/
/*                         DEFINES                                */
/******************************************************************/
#define NUMBEROFTASKS 5

/******************************************************************/
/*                        CONVENÇÕES                              */
/******************************************************************/
//ID das Tasks
#define SENSOR		0
#define CONTROL		1
#define ACTUATOR	2
#define COMM		3
#define TEST		4

//Valores para a variável DISCRETIZATION
#define TUSTIN 				1
#define BACKWARD_DIFF 		2

//Valores para a variável TRAJECTORY
#define SINUSOIDAL_POS		1
#define CONSTANT			2
#define SINUSOIDAL_TOR		3
#define CONTROL_ACTIVE		4
#define RANDOM_EXCITATION	5

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

//saida do controle
extern double setpoint;

//entrada do controle
extern double torque_reading;
extern double position_reading;

//Parâmetros do controlador
extern double J;
extern double K;
extern double B;
extern double coef;

//Offsets dos sensores
extern double extensometerOffset;
extern double potentiometerOffset;

//Valores armazenados para o controle
double POSITION[3];
double TORQUE[3];

#endif /* GLOBAL_H_ */
